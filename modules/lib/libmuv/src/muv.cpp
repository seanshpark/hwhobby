/*
 * Copyright 2019 saehie.park@gmail.com
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <muv/muv.h>

#include <dlog/dlog.h>

#include <string.h>
#include <unistd.h>
#include <sys/epoll.h>
#include <sys/timerfd.h>

#include <algorithm>
#include <cassert>

namespace muv
{

bool Loop::init(void)
{
  fd_backend_ = ::epoll_create(NUM_EPOLL);

  handles_.clear();

  return true;
}

bool Loop::close(void)
{
  if (!handles_.empty())
  {
    DLOG("Loop::close called but handles not empty");
    return false;
  }

  if (fd_backend_ > 0)
  {
    ::close(fd_backend_);
    fd_backend_ = -1;
  }

  return true;
}

Loop* Loop::default_loop(void)
{
  static Loop l;

  return &l;
}

namespace
{

void loop_timer_cb(Timer* timer)
{
  timer->reset();
}

} // namespace

void Loop::run(RunMode mode)
{
  bool active_exist = false;
  struct epoll_event ev[NUM_EPOLL];

  ::memset(ev, 0, sizeof(ev) * NUM_EPOLL);

  h_timer_ = new Timer();
  h_timer_->start(loop_timer_cb, 10); // 10 msec for system timer

  ev[0].events = EPOLLIN;
  if (::epoll_ctl(fd_backend_, EPOLL_CTL_ADD, h_timer_->fd(), &ev[0]) == -1)
  {
    DLOG("Loop: epoll_ctrl failed to add default timer");
    h_timer_->stop();
    delete h_timer_;
    return;
  }
  manage(h_timer_);

  do
  {
    active_exist = false;

    DLOG("Loop: before epoll_wait");
    int num_events = epoll_wait(fd_backend_, ev, NUM_EPOLL, 2000);
    DPRINT("Loop: after epoll_wait : %d\n", num_events);

    struct itimerspec ts;
    timerfd_gettime(h_timer_->fd(), &ts);
    DPRINT("timer %d %d / %d %d\n", ts.it_interval.tv_sec, ts.it_interval.tv_nsec,
	                                  ts.it_value.tv_sec, ts.it_value.tv_nsec);

    for (auto h : handles_)
    {
      if (h->active())
      {
        h->tick();
        active_exist = true;

        if (mode == RunMode::ONCE)
        {
          h->stop();
        }
      }
    }
    if (mode == RunMode::ONCE)
    {
      active_exist = false;
    }

    if (stop_req_)
    {
      for (auto h : handles_)
      {
        if (h->active())
          h->stop();
      }
      active_exist = false;
    }

  } while (active_exist);

  release(h_timer_);

  h_timer_->stop();
  delete h_timer_;
}

void Loop::stop(void)
{
  stop_req_ = true;
}

bool Loop::manage(Handle* h)
{
  handles_.push_back(h);

  return true;
}

bool Loop::release(Handle* h)
{
  Handles::iterator it = std::find(handles_.begin(), handles_.end(), h);
  assert(it != handles_.end());

  handles_.erase(it);

  return true;
}

//-----------------------------------------------------------------------------

bool Handle::init(Loop* pl)
{
  loop_ = pl;

  loop_->manage(this);

  on_init();

  return true;
}

bool Handle::close(void)
{
  if (active())
  {
    DLOG("Idle::close called but still active, call stop first: error");
    return false;
  }

  on_close();

  loop_->release(this);

  return true;
}

void Handle::tick(void)
{
  if (active())
  {
    on_tick();
  }
}

//-----------------------------------------------------------------------------

Idle::Idle()
{
}

bool Idle::active(void) const
{
  if (idle_cb_)
  {
    // this idle is active when callback exist
    return true;
  }

  return Handle::active();
}

bool Idle::start(IdleCB cb)
{
  if (active())
  {
    DLOG("Idle::start called but already active: warning");
    return false;
  }

  if (Handle::start())
  {
    idle_cb_ = cb;

    return true;
  }

  DLOG("Idle::start called: failed to start: error");

  return false;
}

bool Idle::stop(void)
{
  if (!active())
  {
    DLOG("Idle::stop called but currently not active: warning");
    return false;
  }

  if (Handle::stop())
  {
    DLOG("Idle::stop called: clear internal");
    idle_cb_ = nullptr;

    return true;
  }

  DLOG("Idle::stop called: failed to stop: error");

  return false;
}

void Idle::on_tick(void)
{
  assert(active());

  if (idle_cb_)
  {
    idle_cb_(this);
  }
}

//-----------------------------------------------------------------------------

Timer::Timer()
{
}

bool Timer::active(void) const
{
  if (timer_cb_)
  {
    // this idle is active when callback exist
    return true;
  }

  return Handle::active();
}

/**
 * @note period uint is msec
 */
bool Timer::start(TimerCB cb, uint32_t period)
{
  if (active())
  {
    DLOG("Timer::start called but already active: warning");
    return false;
  }

  if (Handle::start())
  {
    timer_cb_ = cb;

    fd_timer_ = ::timerfd_create(CLOCK_MONOTONIC, 0);

    return begin(period);
  }

  DLOG("Timer::start called: failed to start: error");

  return false;
}

void Timer::reset(void)
{
  begin(period_);
}

bool Timer::begin(uint32_t period)
{
  struct itimerspec ts;

  period_ = period;

  auto p_sec = period / 1000;
  auto p_nsec = (period % 1000) * 1000000;

  ts.it_interval.tv_sec = p_sec;
  ts.it_interval.tv_nsec = p_nsec;
  ts.it_value.tv_sec = p_sec;;
  ts.it_value.tv_nsec = p_nsec;

  if (timerfd_settime(fd_timer_, 0, &ts, NULL) < 0)
  {
    DLOG("Timer: timerfd_settime() failed");
    ::close(fd_timer_);
    fd_timer_ = -1;
    return false;
  }
  return true;
}

bool Timer::stop(void)
{
  if (!active())
  {
    DLOG("Timer::stop called but currently not active: warning");
    return false;
  }

  if (Handle::stop())
  {
    DLOG("Timer::stop called: clear internal");

    ::close(fd_timer_);
    fd_timer_ = -1;

    timer_cb_ = nullptr;

    return true;
  }

  DLOG("Timer::stop called: failed to stop: error");

  return false;
}

void Timer::on_tick(void)
{
  assert(active());

  if (timer_cb_)
  {
    timer_cb_(this);
  }
}

} // namespace muv

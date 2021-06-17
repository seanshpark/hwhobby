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

#include <muv/muvport.h>

#include <dlog/dlog.h>

#include <gpio/gpio_interface.h>

#include <cassert>

namespace muv
{

bool Port::init(Loop* pl, Line* line)
{
  if (Handle::init(pl))
  {
    line_ = line;

    return true;
  }

  return false;
}

bool Port::active(void) const
{
  if (port_cb_)
  {
    // this idle is active when callback exist
    return true;
  }

  return Handle::active();
}

bool Port::start(PortCB cb, Event event)
{
  if (active())
  {
    DLOG("Port::start called but already active: error");
    return false;
  }

  if (Handle::start())
  {
    char name[64];
    sprintf(name, "line%p", line_);
    switch (event)
    {
      case Event::FALLING_EDGE:
        gpiod_line_request_falling_edge_events(line_, name);
        break;
    }

    port_cb_ = cb;

    return true;
  }

  DLOG("Port::start called: failed to start: error");

  return false;
}

bool Port::stop(void)
{
  if (!active())
  {
    DLOG("Port::stop called but currently not active: error");
    return false;
  }

  if (Handle::stop())
  {
    DLOG("Port::stop called: clear internal");
    port_cb_ = nullptr;

    return true;
  }

  DLOG("Port::stop called: failed to stop: error");

  return false;
}

void Port::on_tick(void)
{
  assert(active());

  if (port_cb_)
  {
    struct timespec ts = { 0, 1000 };  // 1 micro sec

    // wait for event
    int rv = gpiod_line_event_wait(line_, &ts);
    if (rv == 1)
    {
      struct gpiod_line_event ev;
      rv = gpiod_line_event_read(line_, &ev);
      if (rv == 0 && ev.event_type == GPIOD_LINE_EVENT_FALLING_EDGE)
      {
        hwboard_delay(100); // 0.1 msec
        rv = gpiod_line_get_value(line_);
        if (rv == 0)
        {
          port_cb_(this);
        }
      }
    }
  }
}

} // namespace muv

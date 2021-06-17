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

#ifndef __LIBMUV_MUV_H__
#define __LIBMUV_MUV_H__

#include <hwabc/standard.h>

#include <vector>

namespace muv
{

class Handle;
class Timer;

#define NUM_EPOLL 256

class Loop
{
public:
  enum class RunMode
  {
    DEFAULT = 0,
    ONCE,
    NOWAIT,
  };

public:
  Loop() = default;

public:
  bool init(void);
  bool close(void);

  static Loop* default_loop(void);

public:
  void run(RunMode mode);
  void stop(void);

public:
  bool manage(Handle*);
  bool release(Handle*);

private:
  typedef std::vector<Handle*> Handles;

  int fd_backend_{-1};     // epoll fd
  Timer* h_timer_{nullptr}; // 10 msec loop timer

  Handles handles_;
  bool stop_req_{false};
};

class Handle
{
private:
  enum {
    HANDLE_ACTIVE = 0x4000,
  };

public:
  Handle() = default;

public:
  bool init(Loop*);
  bool close(void);

protected:
  void tick(void);

protected:
  virtual void on_init(void) {};
  virtual void on_close(void) {};
  virtual void on_tick(void) {};

public:
  virtual bool active(void) const { return flag_ & HANDLE_ACTIVE ? true : false; }

public:
  virtual bool start(void) { flag_ |= HANDLE_ACTIVE; return true; }
  virtual bool stop(void) { flag_ &= ~HANDLE_ACTIVE; return true; }

public:
  Loop* loop(void) { return loop_; }

protected:
  Loop* loop_;
  uint32_t flag_{0};

  friend class Loop;
};

class Idle : public Handle
{
public:
  typedef void (*IdleCB) (Idle*);

public:
  Idle();

protected:
  bool active(void) const override;

protected:
  void on_tick(void) override;

public:
  bool start(IdleCB);
  bool stop(void) override;

private:
  IdleCB idle_cb_{nullptr};
};

class Timer : public Handle
{
public:
  typedef void (*TimerCB) (Timer*);

public:
  Timer();

protected:
  bool active(void) const override;

protected:
  void on_tick(void) override;

public:
  bool start(TimerCB, uint32_t period);
  void reset(void);
  bool stop(void) override;

public:
  int fd(void) const { return fd_timer_; }

private:
  bool begin(uint32_t period);

private:
  TimerCB timer_cb_{nullptr};
  uint32_t period_{0};
  int fd_timer_{-1};
};

} // namespace muv

#endif // __LIBMUV_MUV_H__

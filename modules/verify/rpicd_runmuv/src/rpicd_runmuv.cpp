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

#include <stdio.h>
#include <signal.h>

#include <cassert>

volatile sig_atomic_t quit = 0;

void signal_handler(int sig)
{
  signal(sig, signal_handler);
  quit = 1;
}

int _counter = 0;
int _long_cnt = 0;

void my_idle_cb(muv::Idle* idle)
{
  _counter++;
  DPRINT("my_idle_cb %d\r\n", _counter);

  if (_counter >= 100)
  {
    idle->loop()->stop();
  }
}

void run_muv(void)
{
  muv::Loop loop;
  muv::Idle idle;

  if (!loop.init())
  {
    DLOG("failed to init muv loop");
    return;
  }

  idle.init(&loop);

  idle.start(my_idle_cb);
  DLOG("loop run DEFAULT before");
  loop.run(muv::Loop::RunMode::DEFAULT);
  DLOG("loop run after");

  _counter = 0;

  idle.start(my_idle_cb);
  DLOG("loop run ONCE before");
  loop.run(muv::Loop::RunMode::ONCE);
  DLOG("loop run after");

  auto closed = idle.close();
  assert(closed);

  loop.close();
}

int main()
{
  signal(SIGINT, signal_handler);
  signal(SIGTERM, signal_handler);

  run_muv();
  return 0;
}
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
#include <muv/muvport.h>
#include <dlog/dlog.h>

#include <stdio.h>
#include <signal.h>

#include <iostream>
#include <cassert>

volatile sig_atomic_t quit = 0;

void signal_handler(int sig)
{
  signal(sig, signal_handler);
  quit = 1;
}

int _counter = 0;

void my_idle_cb(muv::Idle* idle)
{
}

struct gpiod_chip* chip;
struct gpiod_line* line17;

void my_port_cb(muv::Port* port)
{
  _counter++;
  printf("my_port_cb %d\r\n", _counter);

  if (_counter >= 10)
  {
    port->loop()->stop();
  }
}

void run_muv(void)
{
  chip = gpiod_chip_open("/dev/gpiochip0");
  if (!chip)
  {
    std::cout << "gpiod_chip_open failed" << std::endl;
    return;
  }

  line17 = gpiod_chip_get_line(chip, 17);

  muv::Loop loop;
  muv::Idle idle;
  muv::Port port;

  if (!loop.init())
  {
    DLOG("failed to init muv loop");
    return;
  }

  idle.init(&loop);
  port.init(&loop, line17);

  idle.start(my_idle_cb);
  port.start(my_port_cb, muv::Port::Event::FALLING_EDGE);

  printf("loop run before\r\n");
  loop.run(muv::Loop::RunMode::DEFAULT);
  printf("loop run after\r\n");

  port.close();
  auto closed = idle.close();
  assert(closed);

  loop.close();

  gpiod_line_release(line17);
  gpiod_chip_close(chip);
}

int main()
{
  signal(SIGINT, signal_handler);
  signal(SIGTERM, signal_handler);

  run_muv();
  return 0;
}

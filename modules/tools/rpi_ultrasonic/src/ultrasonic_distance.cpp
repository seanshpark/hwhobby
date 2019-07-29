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

#include <hwmodule/hc-sr04.h>

#include <gpio/gpio_interface.h>

#include <signal.h>
#include <stdio.h>
#include <unistd.h>

// RPi
static HW_GPIO_t _gpio_trig = { 0, 23 }; // GPIO23
static HW_GPIO_t _gpio_echo = { 0, 24 }; // GPIO24

volatile sig_atomic_t quit;

void signal_handler(int sig)
{
  signal(sig, signal_handler);
  quit = 1;
}

int hwhobby_counter(void)
{
  if (hcsr04_init(&_gpio_trig, &_gpio_echo) != HWRESULT_SUCCESS)
    return -1;

  for (int i = 0; i < 9999; i++)
  {
    if (quit) break;

    hwusec_t et = hcsr04_measure_time();
    double dist = (double)et * 17150.0 / 1000000.0;
    printf("Time = %6llu usec --> Distance = %10f cm\r\n", et, dist);
    hwboard_delay(100*1000); // 100 msec
  }

  hcsr04_close();

  return 0;
}

int main()
{
  signal(SIGINT, signal_handler);
  signal(SIGTERM, signal_handler);

  return hwhobby_counter();
}

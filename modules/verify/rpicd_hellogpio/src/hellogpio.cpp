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

#include <gpio/gpio_interface.h>

#include <signal.h>
#include <stdio.h>

volatile sig_atomic_t quit = 0;

void signal_handler(int sig)
{
  signal(sig, signal_handler);
  quit = 1;
}

int hwhobby_hellogpio(void)
{
  if (hwboard_gpio_init() != HWRESULT_SUCCESS)
  {
    fprintf(stderr, "hwboard_gpio_init() failed\r\n");
    return -1;
  }

  // Pin is GPIO#
  HW_GPIO_CFG_t cfg17 = { 0, 17, HWBOARD_GPIO_PUD_UP, HWBOARD_GPIO_FSEL_OUT };
  HW_GPIO_CFG_t cfg18 = { 0, 18, HWBOARD_GPIO_PUD_UP, HWBOARD_GPIO_FSEL_OUT };

  hwboard_gpio_cfg(&cfg17);
  hwboard_gpio_cfg(&cfg18);

  HW_GPIO_t gpio17 = { 0, 17 };
  HW_GPIO_t gpio18 = { 0, 18 };

  while (!quit)
  {
    hwboard_gpio_set(&gpio17);
    hwboard_delay_ms(500);
    hwboard_gpio_set(&gpio18);
    hwboard_delay_ms(500);
    hwboard_delay_ms(500);
    
    hwboard_gpio_clr(&gpio17);
    hwboard_delay_ms(500);
    hwboard_gpio_clr(&gpio18);
    hwboard_delay_ms(500);
    hwboard_delay_ms(500);
  }

  hwboard_gpio_close();

  return 0;
}

int main()
{
  signal(SIGINT, signal_handler);
  signal(SIGTERM, signal_handler);

  return hwhobby_hellogpio();
}

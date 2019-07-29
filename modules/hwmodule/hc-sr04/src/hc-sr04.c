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

static HW_GPIO_t _gpio_trig;
static HW_GPIO_t _gpio_echo;

HWRESULT hcsr04_init(HW_GPIO_t* gpio_trig, HW_GPIO_t* gpio_echo)
{
  if (hwboard_gpio_init() != HWRESULT_SUCCESS)
    return 0;

  _gpio_trig = *gpio_trig;
  _gpio_echo = *gpio_echo;

  HW_GPIO_CFG_t gpiocfg_trig = {
    _gpio_trig.port,
    _gpio_trig.pin,
    HWBOARD_GPIO_PUD_UP,
    HWBOARD_GPIO_FSEL_OUT
  };
  HW_GPIO_CFG_t gpiocfg_echo = {
    _gpio_echo.port,
    _gpio_echo.pin,
    HWBOARD_GPIO_PUD_UP,
    HWBOARD_GPIO_FSEL_OUT
  };

  // set output to trigger pin
  hwboard_gpio_cfg(&gpiocfg_trig);
  // set input to echo pin
  hwboard_gpio_cfg(&gpiocfg_echo);

  hwboard_gpio_clr(&_gpio_trig);

  return HWRESULT_SUCCESS;
}

void hcsr04_close(void)
{
  hwboard_gpio_clr(&_gpio_trig);

  HW_GPIO_CFG_t gpiocfg_trig = {
    _gpio_trig.port,
    _gpio_trig.pin,
    HWBOARD_GPIO_PUD_OFF,
    HWBOARD_GPIO_FSEL_INP
  };
  HW_GPIO_CFG_t gpiocfg_echo = {
    _gpio_echo.port,
    _gpio_echo.pin,
    HWBOARD_GPIO_PUD_OFF,
    HWBOARD_GPIO_FSEL_INP
  };

  // set 'pull up down' to off, function select to input(high impedance)
  hwboard_gpio_cfg(&gpiocfg_trig);
  hwboard_gpio_cfg(&gpiocfg_echo);

  hwboard_gpio_close();
}

int hcsr04_measure_time(void)
{
  //
  // set trig from low to high and maintain at least 10usec
  // wait while echo is L
  // when echo goes H, read start time
  // wait while echo is H
  // when echo goes L, read end time
  //
  // distance is (end time - start time) * 17150, time in msec
  //
  // where speed = distance / time
  //       343(m/s) = distance / (time / 2), '/2' is to divide time for bounce back
  //       171.5 = distance / time
  //       171.5 * time = distance
  //       171500 * time = distance in cm.
  //

  hwusec_t time_begin;
  hwusec_t time_end;

  int v = hwboard_gpio_get(&_gpio_echo);
  if (v)
    return 0;

  // send trigger
  hwboard_gpio_set(&_gpio_trig);
  hwboard_delay(20);
  hwboard_gpio_clr(&_gpio_trig);

  v = hwboard_gpio_get(&_gpio_echo);
  if (v)
    return 0;

  while (v == 0)
  {
    v = hwboard_gpio_get(&_gpio_echo);
    hwboard_delay(1);
  };
  time_begin = hwboard_time();

  while (v)
  {
    v = hwboard_gpio_get(&_gpio_echo);
    hwboard_delay(1);
  };
  time_end = hwboard_time();

  return time_end - time_begin;
}

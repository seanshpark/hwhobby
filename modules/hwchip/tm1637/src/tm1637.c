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

#include <hwchip/tm1637.h>
#include <gpio/gpio_interface.h>

static const uint32_t CLOCK_DELAY = 20;
static const uint32_t DATA_DELAY  = 10;

static HW_GPIO_t _gpio_clock;
static HW_GPIO_t _gpio_data;

static void start(void)
{
  // CLK 1111
  // DIO 1100
  //
  // set both high to enter start
  hwboard_gpio_set(&_gpio_clock);
  hwboard_gpio_set(&_gpio_data);
  hwboard_delay(CLOCK_DELAY);

  hwboard_gpio_clr(&_gpio_data);
  hwboard_delay(CLOCK_DELAY);
}

static void stop(void)
{
  // CLK 0111
  // DIO 0011
  hwboard_gpio_clr(&_gpio_data);
  hwboard_delay(CLOCK_DELAY);

  hwboard_gpio_set(&_gpio_clock);
  hwboard_delay(CLOCK_DELAY);
  hwboard_gpio_set(&_gpio_data);
  hwboard_delay(CLOCK_DELAY);
}

static void write_byte(uint8_t b)
{
  for (int i = 0; i < 8; i++)
  {
    hwboard_gpio_clr(&_gpio_clock);
    hwboard_delay(DATA_DELAY);
    if (b & 1) // send LSB to MSB
      hwboard_gpio_set(&_gpio_data);
    else
      hwboard_gpio_clr(&_gpio_data);
    hwboard_delay(CLOCK_DELAY);

    hwboard_gpio_set(&_gpio_clock);
    hwboard_delay(CLOCK_DELAY);
    b >>= 1; // next LSB
  }
}

static void skip_ack(void)
{
  hwboard_gpio_clr(&_gpio_clock);
  hwboard_gpio_set(&_gpio_data);

  hwboard_delay(CLOCK_DELAY);

  hwboard_gpio_set(&_gpio_clock);
  hwboard_delay(CLOCK_DELAY);

  hwboard_gpio_clr(&_gpio_clock);
  hwboard_delay(CLOCK_DELAY);
}

HWRESULT tm1637_init(HW_GPIO_t* gpio_clock, HW_GPIO_t* gpio_data)
{
  _gpio_clock = *gpio_clock;
  _gpio_data = *gpio_data;

  HW_GPIO_CFG_t gpiocfg_clock = {
    _gpio_clock.port,
    _gpio_clock.pin,
    HWBOARD_GPIO_PUD_UP,
    HWBOARD_GPIO_FSEL_OUT
  };
  HW_GPIO_CFG_t gpiocfg_data = {
    _gpio_data.port,
    _gpio_data.pin,
    HWBOARD_GPIO_PUD_UP,
    HWBOARD_GPIO_FSEL_OUT
  };

  // set 'pull up down' to up, function select to output
  hwboard_gpio_cfg(&gpiocfg_clock);
  hwboard_gpio_cfg(&gpiocfg_data);

  hwboard_gpio_clr(&_gpio_clock);
  hwboard_gpio_clr(&_gpio_data);
  
  return HWRESULT_SUCCESS;
}

void tm1637_close(void)
{
  hwboard_gpio_clr(&_gpio_clock);
  hwboard_gpio_clr(&_gpio_data);

  HW_GPIO_CFG_t gpiocfg_clock = {
    _gpio_clock.port,
    _gpio_clock.pin,
    HWBOARD_GPIO_PUD_OFF,
    HWBOARD_GPIO_FSEL_INP
  };
  HW_GPIO_CFG_t gpiocfg_data = {
    _gpio_data.port,
    _gpio_data.pin,
    HWBOARD_GPIO_PUD_OFF,
    HWBOARD_GPIO_FSEL_INP
  };

  // set 'pull up down' to off, function select to input(high impedance)
  hwboard_gpio_cfg(&gpiocfg_clock);
  hwboard_gpio_cfg(&gpiocfg_data);
}

void tm1637_writes(uint8_t *data, int length)
{
  start();

  for (int b = 0; b < length; b++)
  {
    write_byte(data[b]);
    skip_ack();
  }

  stop();
}

void tm1637_write(uint8_t data)
{
  start();

  write_byte(data);
  skip_ack();
  
  stop();
}

void tm1637_bright(uint8_t value)
{
  uint8_t command;

  if (value == 0) // display off
    command = TM1637_CMD_DISPLAY | TM1637_DISPLAY_OFF;
  else
  {
    value = value > 8 ? 8 : value;
    command = TM1637_CMD_DISPLAY | TM1637_DISPLAY_ON;
    command |= (value - 1);
  }
  tm1637_write(command);
}

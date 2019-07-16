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

#include <unistd.h>

static const uint8_t CLOCK_DELAY = 20;
static const uint8_t DATA_DELAY = 5;

static ioport_t _port_clock = 0;
static ioport_t _port_data = 0;
static ioport_t _pin_clock = 0;
static ioport_t _pin_data = 0;

void start(void)
{
  // set both high to enter start
  hwboard_gpio_set(_port_data, _pin_data);
  hwboard_gpio_set(_port_clock, _pin_clock);
  hwboard_delay(CLOCK_DELAY);

  hwboard_gpio_clr(_port_data, _pin_data);
  hwboard_delay(CLOCK_DELAY);
}

void stop(void)
{
  hwboard_gpio_clr(_port_clock, _pin_clock);
  hwboard_delay(CLOCK_DELAY);
  hwboard_gpio_clr(_port_data,_pin_data);
  hwboard_delay(CLOCK_DELAY);

  hwboard_gpio_set(_port_clock, _pin_clock);
  hwboard_delay(CLOCK_DELAY);
  hwboard_gpio_set(_port_data, _pin_data);
  hwboard_delay(CLOCK_DELAY);
}

void write_byte(uint8_t b)
{
  for (int i = 0; i < 8; i++)
  {
    hwboard_gpio_clr(_port_clock, _pin_clock);
    hwboard_delay(DATA_DELAY);
    if (b & 1) // send LSB to MSB
      hwboard_gpio_set(_port_data, _pin_data);
    else
      hwboard_gpio_clr(_port_data, _pin_data);
    hwboard_delay(CLOCK_DELAY);

    hwboard_gpio_set(_port_clock, _pin_clock);
    hwboard_delay(CLOCK_DELAY);
    b >>= 1; // next LSB
  }
}

void skip_ack(void)
{
  hwboard_gpio_clr(_port_clock, _pin_clock);
  hwboard_gpio_clr(_port_data, _pin_data);
  hwboard_delay(CLOCK_DELAY);
   
  hwboard_gpio_set(_port_clock,_pin_clock);
  hwboard_delay(CLOCK_DELAY);

  hwboard_gpio_clr(_port_clock, _pin_clock);
  hwboard_delay(CLOCK_DELAY);
}

int tm1637_init(ioport_t clock_port, ioport_t clock_pin, ioport_t data_port, ioport_t data_pin)
{
  if (!hwboard_gpio_init())
    return 0;

  _port_clock = clock_port;
  _pin_clock = clock_pin;
  _port_data = data_port;
  _pin_data = data_pin;

  // set 'pull up down' to up, function select to output
  hwboard_gpio_cfg(_port_clock, _pin_clock, HWBOARD_GPIO_PUD_UP, HWBOARD_GPIO_FSEL_OUT);
  hwboard_gpio_cfg(_port_data, _pin_data, HWBOARD_GPIO_PUD_UP, HWBOARD_GPIO_FSEL_OUT);

  hwboard_gpio_clr(_port_clock, _pin_clock);
  hwboard_gpio_clr(_port_data, _pin_data);
  
  return 1;
}

void tm1637_close(void)
{
  hwboard_gpio_clr(_port_clock, _pin_clock);
  hwboard_gpio_clr(_port_data, _pin_data);

  // set 'pull up down' to off, function select to input(high impedance)
  hwboard_gpio_cfg(_port_clock, _pin_clock, HWBOARD_GPIO_PUD_OFF, HWBOARD_GPIO_FSEL_INP);
  hwboard_gpio_cfg(_port_data, _pin_data, HWBOARD_GPIO_PUD_OFF, HWBOARD_GPIO_FSEL_INP);

  hwboard_gpio_close();
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

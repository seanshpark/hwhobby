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
#include <gpiod.h>

#include <stdio.h>
#include <assert.h>
#include <time.h>
#include <unistd.h> // for usleep

typedef struct gpiod_chip GpioChip_t;
typedef struct gpiod_line GpioLine_t;

typedef struct __GpioContext_t
{
  GpioChip_t* chip;
  uint32_t num_lines;

  // I2C only one can be use for now
  // TODO provide multiple
  GpioLine_t* i2c_scl;
  GpioLine_t* i2c_sda;
  ioport_t i2c_addr;

} GpioContext_t;

static GpioContext_t __gpio_context =
{ 
  NULL, 
  0,
  NULL,
  NULL,
  0
};

const char* _consumer_name = "hwhobby";

HWRESULT hwboard_gpio_init(void)
{
  if (__gpio_context.chip == NULL)
  {
    __gpio_context.chip = gpiod_chip_open_by_number(0);
    if (__gpio_context.chip == NULL)
    {
      return HWRESULT_FAILED;
    }
  }
  __gpio_context.num_lines = gpiod_chip_num_lines(__gpio_context.chip);

  fprintf(stdout, "hwboard_gpio_init: %d lines\r\n", __gpio_context.num_lines);

  return HWRESULT_SUCCESS;
}

void hwboard_gpio_close(void)
{
  if (__gpio_context.chip)
  {
    gpiod_chip_close(__gpio_context.chip);
    __gpio_context.chip = NULL;
  }
}

static GpioLine_t* _get_line(ioport_t pin)
{
  uint32_t offset = pin;
  if (offset >= __gpio_context.num_lines)
    return NULL;

  return gpiod_chip_get_line(__gpio_context.chip, offset);
}

void hwboard_gpio_set(HW_GPIO_t* gpio)
{
  GpioLine_t* line = _get_line(gpio->pin);
  if (line != NULL)
  {
    gpiod_line_set_value(line, 1);
  }
}

void hwboard_gpio_clr(HW_GPIO_t* gpio)
{
  GpioLine_t* line = _get_line(gpio->pin);
  if (line != NULL)
  {
    gpiod_line_set_value(line, 0);
  }
}

ioport_t hwboard_gpio_get(HW_GPIO_t* gpio)
{
  GpioLine_t* line = _get_line(gpio->pin);
  if (line != NULL)
  {
    return gpiod_line_get_value(line);
  }
  return 0;
}

void hwboard_gpio_fen(HW_GPIO_t* gpio, uint8_t enable)
{
  GpioLine_t* line = _get_line(gpio->pin);
  if (line != NULL)
  {
    if (enable)
    {
      char pin_name[32];
      sprintf(pin_name, "pname_%d", gpio->pin);
      gpiod_line_request_falling_edge_events(line, pin_name);
    }
    // don't have disable
  }
}

uint8_t hwboard_gpio_eds(HW_GPIO_t* gpio, uint8_t clear)
{
  // TODO implement this
  return 0;
}

void hwboard_gpio_cfg(HW_GPIO_CFG_t* gpiocfg)
{
  GpioLine_t* line = _get_line(gpiocfg->pin);
  if (line == NULL)
    return;

  // TODO implement this
  switch (gpiocfg->pud)
  {
  case HWBOARD_GPIO_PUD_OFF: break;
  case HWBOARD_GPIO_PUD_DN: break;
  case HWBOARD_GPIO_PUD_UP: break;
  }

  switch (gpiocfg->fsel)
  {
  case HWBOARD_GPIO_FSEL_INP:
    gpiod_line_request_input(line, _consumer_name);
    break;

  case HWBOARD_GPIO_FSEL_OUT:
    gpiod_line_request_output(line, _consumer_name, 0);
    break;
  }
}

void hwboard_delay(int usec)
{
  usleep(usec);
}

void hwboard_delay_ms(int msec)
{
  uint32_t usec = msec * 1000ull;
  usleep(usec);
}

///@ elapased time in usec
hwusec_t hwboard_time(void)
{
  struct timespec time;
  hwusec_t time_val;

  clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &time);

  time_val = time.tv_nsec / 1000ull;
  time_val += time.tv_sec * 1000000ull;

  return time_val;
}

HWRESULT hwboard_i2c_open(HW_I2C_INIT_t* pi2cinit)
{
  __gpio_context.i2c_scl = _get_line(pi2cinit->i2cpin_scl);
  __gpio_context.i2c_sda = _get_line(pi2cinit->i2cpin_sda);

  if (__gpio_context.i2c_scl == NULL || __gpio_context.i2c_sda == NULL)
    return HWRESULT_FAILED;

  __gpio_context.i2c_addr = pi2cinit->i2caddr;

  gpiod_line_request_output(__gpio_context.i2c_scl, _consumer_name, 1);
  gpiod_line_request_output(__gpio_context.i2c_sda, _consumer_name, 1);

  return HWRESULT_SUCCESS;
}

void hwboard_i2c_close(void)
{
  __gpio_context.i2c_scl = NULL;
  __gpio_context.i2c_sda = NULL;
  __gpio_context.i2c_addr = 0;
}

#define I2C_DELAY  1

static void _i2c_send_start(GpioLine_t* i2c_sda, GpioLine_t* i2c_scl)
{
  gpiod_line_set_value(i2c_sda, 1);
  gpiod_line_set_value(i2c_scl, 1);
  gpiod_line_set_value(i2c_sda, 0);
  hwboard_delay(I2C_DELAY);
  gpiod_line_set_value(i2c_scl, 0);
}

static void _i2c_send_stop(GpioLine_t* i2c_sda, GpioLine_t* i2c_scl)
{
  gpiod_line_set_value(i2c_sda, 0);
  gpiod_line_set_value(i2c_scl, 1);
  hwboard_delay(I2C_DELAY);
  gpiod_line_set_value(i2c_sda, 1);
  hwboard_delay(I2C_DELAY);
}

static void _i2c_send(GpioLine_t* i2c_sda, GpioLine_t* i2c_scl, uint8_t data, uint8_t mask)
{
  ioport_t data_bit = mask;

  while (data_bit)
  {
    int sda_val = 0;
    if (data & data_bit)
    {
      sda_val = 1;
    }
    gpiod_line_set_value(i2c_sda, sda_val);
    hwboard_delay(I2C_DELAY);
    gpiod_line_set_value(i2c_scl, 1);
    hwboard_delay(I2C_DELAY);
    gpiod_line_set_value(i2c_scl, 0);
    hwboard_delay(I2C_DELAY);

    data_bit = data_bit >> 1;
  }

  // TODO: read and wait ACK. Skip ACK for now.
  gpiod_line_set_value(i2c_sda, 1);
  hwboard_delay(I2C_DELAY);
  gpiod_line_set_value(i2c_scl, 1);
  hwboard_delay(I2C_DELAY);
  gpiod_line_set_value(i2c_scl, 0);
}

static void _i2c_send_byte(GpioLine_t* i2c_sda, GpioLine_t* i2c_scl, uint8_t data)
{
  _i2c_send(i2c_sda, i2c_scl, data, 0x80);
}

void hwboard_i2c_send_byte(uint8_t data)
{
  GpioLine_t* i2c_sda = __gpio_context.i2c_sda;
  GpioLine_t* i2c_scl = __gpio_context.i2c_scl;
  uint8_t i2c_addr = __gpio_context.i2c_addr;

  _i2c_send_start(i2c_sda, i2c_scl);

  // send 7bit address and R/-W(which is -W, 0 for send) in LSB
  i2c_addr = i2c_addr << 1;
  _i2c_send_byte(i2c_sda, i2c_scl, i2c_addr);

  // send data
  _i2c_send_byte(i2c_sda, i2c_scl, data);

  _i2c_send_stop(i2c_sda, i2c_scl);
}

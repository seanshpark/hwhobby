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
#include <gpio_stm32f103c8t6.h>

#include <stm32f10x_lib.h>

#include "stm32f103c8t6.h"

HWRESULT hwboard_gpio_init(void)
{
  RCC_Configuration();
  NVIC_Configuration();

  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);

  return HWRESULT_SUCCESS;
}

void hwboard_gpio_close(void)
{
  // nothing to do
}

static GPIO_TypeDef* _hwport_from_port(ioport_t port)
{
  static GPIO_TypeDef* hwport_table[] = {
    GPIOA, GPIOB, GPIOC
  };
  static const int count = 3;
  if (port >= count)
    port = 0;

  return hwport_table[port];
}

/**
 * @brief return STM32F103 H/W pin code from logical pin number (0 ~ )
 */
static  ioport_t _hwpin_from_pin(ioport_t pin)
{
  static const ioport_t hwpin_table[] = {
    GPIO_Pin_0, GPIO_Pin_1, GPIO_Pin_2, GPIO_Pin_3,
    GPIO_Pin_4, GPIO_Pin_5, GPIO_Pin_6, GPIO_Pin_7,
    GPIO_Pin_8, GPIO_Pin_9, GPIO_Pin_10, GPIO_Pin_11,
    GPIO_Pin_12, GPIO_Pin_13, GPIO_Pin_14, GPIO_Pin_15
  };
  static const int count = 16;

  if (pin >= count)
    pin = 0;

  return hwpin_table[pin];
}

void hwboard_gpio_set(HW_GPIO_t* gpio)
{
  GPIO_TypeDef* hwport = _hwport_from_port(gpio->port);
  ioport_t hwpin = _hwpin_from_pin(gpio->pin);

  GPIO_SetBits(hwport, hwpin);
}

void hwboard_gpio_clr(HW_GPIO_t* gpio)
{
  GPIO_TypeDef* hwport = _hwport_from_port(gpio->port);
  ioport_t hwpin = _hwpin_from_pin(gpio->pin);

  GPIO_ResetBits(hwport, hwpin);
}

void hwboard_gpio_cfg(HW_GPIO_CFG_t* gpiocfg)
{
  GPIO_TypeDef* hwport = _hwport_from_port(gpiocfg->port);
  GPIOMode_TypeDef mode = GPIO_Mode_AIN;
  GPIO_InitTypeDef GPIO_InitStructure;
  ioport_t hwpin = _hwpin_from_pin(gpiocfg->pin);

  switch (gpiocfg->pud)
  {
  case HWBOARD_GPIO_PUD_OFF: break;
  case HWBOARD_GPIO_PUD_DN: break;
  case HWBOARD_GPIO_PUD_UP: break;
  }

  switch (gpiocfg->fsel)
  {
  case HWBOARD_GPIO_FSEL_INP: mode = GPIO_Mode_IN_FLOATING; break;
  case HWBOARD_GPIO_FSEL_OUT: mode = GPIO_Mode_Out_PP; break;
  }

  GPIO_InitStructure.GPIO_Pin = hwpin;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
  GPIO_InitStructure.GPIO_Mode = mode;

  GPIO_Init(hwport, &GPIO_InitStructure);
}

void hwboard_delay(int usec)
{
  int loop = 5;
  while (loop > 0)
  {
    Delay(usec);
    loop--;
  }
}

void hwboard_delay_ms(int msec)
{
  int loop = 1000;
  while (loop > 0)
  {
    hwboard_delay(msec);
    loop--;
  }
}

#ifdef  DEBUG
void assert_failed(u8* file, u32 line)
{
  while (1)
  {
    hwboard_gpio_set(STM32F103_PORT_C, 13);
    hwboard_delay(100 * 1000);
    hwboard_gpio_clr(STM32F103_PORT_C, 13);
    hwboard_delay(100 * 1000);
  }
}
#endif

//
// I2C
//
static I2C_TypeDef* _current_i2c_num; // I2C-1, I2C-2
static uint8_t _current_i2c_addr;

static void _i2c_start()
{
  while (I2C_GetFlagStatus(_current_i2c_num, I2C_FLAG_BUSY))
  {
  }

  I2C_GenerateSTART(_current_i2c_num, ENABLE);

  while (!I2C_CheckEvent(_current_i2c_num, I2C_EVENT_MASTER_MODE_SELECT))
  {
  }
}

static void _i2c_addr(uint8_t direction)
{
  I2C_Send7bitAddress(_current_i2c_num, _current_i2c_addr, direction);

  if (direction == I2C_Direction_Transmitter)
  {
    while (!I2C_CheckEvent(_current_i2c_num, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED))
    {
    }
  }
  else if (direction == I2C_Direction_Receiver)
  {
    while (!I2C_CheckEvent(_current_i2c_num, I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED))
    {
    }
  }
}

static void _i2c_send(uint8_t data)
{
  I2C_SendData(_current_i2c_num, data);

  while (!I2C_CheckEvent(_current_i2c_num, I2C_EVENT_MASTER_BYTE_TRANSMITTED))
  {
  }
}

static void _i2c_stop()
{
  I2C_GenerateSTOP(_current_i2c_num, ENABLE);

  while (I2C_GetFlagStatus(_current_i2c_num, I2C_FLAG_STOPF))
  {
  }
}

static I2C_TypeDef *_hwi2c_from_i2c(ioport_t num)
{
  static I2C_TypeDef* hwi2c_table[] = {
    I2C1, I2C2
  };
  static const int count = 2;
  if (num >= count)
    num = 0;

  return hwi2c_table[num];
}

HWRESULT hwboard_i2c_open(HW_I2C_INIT_t* pi2cinit)
{
  GPIO_InitTypeDef GPIO_InitStruct;
  I2C_InitTypeDef I2C_InitStruct;

  GPIO_TypeDef* hwport = _hwport_from_port(pi2cinit->i2cport);
  ioport_t hwpin_scl = _hwpin_from_pin(pi2cinit->i2cpin_scl);
  ioport_t hwpin_sda = _hwpin_from_pin(pi2cinit->i2cpin_sda);
  I2C_TypeDef *i2cnum = _hwi2c_from_i2c(pi2cinit->i2c_num);

  switch (pi2cinit->i2c_num)
  {
  case STM32F103_I2C_1:
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C1, ENABLE);
    break;
  case STM32F103_I2C_2:
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C2, ENABLE);
    break;
  }

  GPIO_InitStruct.GPIO_Pin = hwpin_scl | hwpin_sda;
  GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_OD;
  GPIO_InitStruct.GPIO_Speed = GPIO_Speed_2MHz;
  GPIO_Init(hwport, &GPIO_InitStruct);

  I2C_StructInit(&I2C_InitStruct);
  I2C_InitStruct.I2C_ClockSpeed = 100000;
  I2C_InitStruct.I2C_OwnAddress1 = 0x10;

  I2C_Init(i2cnum, &I2C_InitStruct);
  I2C_Cmd(i2cnum, ENABLE);

  _current_i2c_num = i2cnum;
  _current_i2c_addr = (uint8_t)(pi2cinit->i2caddr) << 1;

  return HWRESULT_SUCCESS;
}

void hwboard_i2c_close(void)
{
  // nothing to do
}

void hwboard_i2c_send_byte(uint8_t data)
{
  _i2c_start();
  _i2c_addr(I2C_Direction_Transmitter);
  _i2c_send(data);
  _i2c_stop();
}

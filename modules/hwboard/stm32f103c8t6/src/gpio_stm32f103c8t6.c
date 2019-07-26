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

GPIO_TypeDef* hwport_from_port(ioport_t port)
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
ioport_t hwpin_from_pin(ioport_t pin)
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

void hwboard_gpio_set(ioport_t port, ioport_t pin)
{
  GPIO_TypeDef* hwport = hwport_from_port(port);
  ioport_t hwpin = hwpin_from_pin(pin);

  GPIO_SetBits(hwport, hwpin);
}

void hwboard_gpio_clr(ioport_t port, ioport_t pin)
{
  GPIO_TypeDef* hwport = hwport_from_port(port);
  ioport_t hwpin = hwpin_from_pin(pin);

  GPIO_ResetBits(hwport, hwpin);
}

void hwboard_gpio_cfg(ioport_t port, ioport_t pin, uint8_t pudin, uint8_t fselin)
{
  GPIO_TypeDef* hwport = hwport_from_port(port);
  GPIOMode_TypeDef mode = GPIO_Mode_AIN;
  GPIO_InitTypeDef GPIO_InitStructure;
  ioport_t hwpin = hwpin_from_pin(pin);

  switch (pudin)
  {
  case HWBOARD_GPIO_PUD_OFF: break;
  case HWBOARD_GPIO_PUD_DN: break;
  case HWBOARD_GPIO_PUD_UP: break;
  }

  switch (fselin)
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

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

#include <hwmodule/led4x7seg.h>

ioport_t gpio_clock  = 14; // pin 14
ioport_t gpio_dataio = 13; // pin 13

void fill_data(LED4X7SEG_t* data, int value)
{
  char display[8];
  int v1 = value / 100;
  int v2 = value % 100;
  int v11 = v1 / 10;
  int v12 = v1 % 10;
  int v21 = v2 / 10;
  int v22 = v2 % 10;

  display[0] = v11 + '0';
  display[1] = v12 + '0';
  display[2] = v21 + '0';
  display[3] = v22 + '0';

  for (int i = 0; i < 4; i++)
    data->digit[i] = display[i];

  data->colon = value & 1;
}

int hwhobby_counter(void)
{
  if (!led4x7seg_init(STM32F103_PORT_C, gpio_clock, STM32F103_PORT_C, gpio_dataio))
    return -1;

  hwboard_gpio_cfg(STM32F103_PORT_C, gpio_clock, HWBOARD_GPIO_PUD_UP, HWBOARD_GPIO_FSEL_OUT);
  hwboard_gpio_cfg(STM32F103_PORT_C, gpio_dataio, HWBOARD_GPIO_PUD_UP, HWBOARD_GPIO_FSEL_OUT);

  uint8_t brightness = 3;

  led4x7seg_bright(brightness);

  for (int i = 0; i < 9999; i++)
  {
    LED4X7SEG_t display;
    fill_data(&display, i);
    led4x7seg_show(&display);
    hwboard_delay_ms(1000);
  }

  return 0;
}

int main()
{
  return hwhobby_counter();
}

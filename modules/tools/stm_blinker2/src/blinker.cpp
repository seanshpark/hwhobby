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

int main(void)
{
  hwboard_gpio_init();

  hwboard_gpio_cfg(STM32F103_PORT_C, 13, HWBOARD_GPIO_PUD_UP, HWBOARD_GPIO_FSEL_OUT);
  
  while (1)
  {
    for (int i = 0; i < 5; i++)
    {
      /* Turn on led connected to PC.13 pin */
      hwboard_gpio_set(STM32F103_PORT_C, 13);
      /* Insert delay */
      hwboard_delay(500 * 1000);

      /* Turn off led connected to PC.13 pin */
      hwboard_gpio_clr(STM32F103_PORT_C, 13);
      /* Insert delay */
      hwboard_delay(500 * 1000);
    }
  }
}


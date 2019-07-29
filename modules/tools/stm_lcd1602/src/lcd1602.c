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

#include <hwmodule/lcd1602.h>

#include <gpio_stm32f103c8t6.h>

#define I2C_ADDR_BLUE   0x3F            // my blue backlight LCD is 0x3f
#define I2C_ADDR_YELLOW 0x27            // my yellow backlight LCD is 0x27
#define I2C_ADDR        I2C_ADDR_YELLOW

static HW_GPIO_t _gpio_led = { STM32F103_PORT_C, 13 };

HWRESULT hwhobby_lcd1602_init(void)
{
  HW_I2C_INIT_t i2cinit;

  // using I2C-2  
  i2cinit.i2c_num = STM32F103_I2C_2;
  i2cinit.i2cport = STM32F103_PORT_B;
  i2cinit.i2cpin_scl = 10;
  i2cinit.i2cpin_sda = 11;
  i2cinit.i2caddr = I2C_ADDR;

  // using I2C-1
  i2cinit.i2c_num = STM32F103_I2C_1;
  i2cinit.i2cpin_scl = 6;
  i2cinit.i2cpin_sda = 7;
  
  return lcd1602_init(&i2cinit);
}

void hwhobby_show(void)
{
  hwboard_gpio_set(&_gpio_led);
  lcd1602_moverc(0, 1);
  lcd1602_puts("Hello !!!");
  lcd1602_moverc(1, 3);
  lcd1602_puts("World !!!");
  hwboard_delay(2500 * 1000);
  lcd1602_clear();

  hwboard_gpio_clr(&_gpio_led);
  lcd1602_moverc(0, 1);
  lcd1602_puts("LCD1602 with");
  lcd1602_moverc(1, 5);
  lcd1602_puts("STM32F103");
  hwboard_delay(2500 * 1000);
  lcd1602_clear();
}

void hwhobby_blink(uint32_t delay)
{
  hwboard_gpio_set(&_gpio_led);
  hwboard_delay(delay * 1000);
  hwboard_gpio_clr(&_gpio_led);
  hwboard_delay(delay * 1000);
}

int main()
{
  HW_GPIO_CFG_t gpiocfg_led = {
    _gpio_led.port,
    _gpio_led.pin,
    HWBOARD_GPIO_PUD_UP,
    HWBOARD_GPIO_FSEL_OUT
  };

  hwboard_gpio_init();
  hwboard_gpio_cfg(&gpiocfg_led);

  hwhobby_blink(500);
  hwhobby_blink(500);
  hwhobby_blink(500);

  if (hwhobby_lcd1602_init() != HWRESULT_SUCCESS)
  {
    while (1)
    {
      hwhobby_blink(50);
    }
  }

  while (1)
  {
    hwhobby_show();
  }

  return 0;
}

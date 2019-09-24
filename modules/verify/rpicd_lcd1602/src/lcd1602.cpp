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

#include <hwmodule/lcd1602.h>

#include <gpio/gpio_interface.h>

#include <signal.h>
#include <stdio.h>

volatile sig_atomic_t quit = 0;

void signal_handler(int sig)
{
  signal(sig, signal_handler);
  quit = 1;
}

#define I2C_ADDR_BLUE   0x3F            // my blue backlight LCD is 0x3f
#define I2C_ADDR_YELLOW 0x27            // my yellow backlight LCD is 0x27
#define I2C_ADDR        I2C_ADDR_BLUE

int hwhobby_lcd1602(void)
{
  if (hwboard_gpio_init() != HWRESULT_SUCCESS)
    return HWRESULT_FAILED;

  HW_I2C_INIT_t i2cinit;

  i2cinit.i2caddr = I2C_ADDR;
  i2cinit.i2cpin_scl = 3; // GPIO03: SCL1
  i2cinit.i2cpin_sda = 2; // GPIO02: SDA1
  if (lcd1602_init(&i2cinit) != HWRESULT_SUCCESS)
  {
    printf("lcd1602_init failed\r\n");
    return -1;
  }

  lcd1602_moverc(0, 1);
  printf("moverc 0,1 ok\r\n");
  lcd1602_puts("Hello !!!");
  printf("puts ok\r\n");
  
  lcd1602_moverc(1, 3);
  lcd1602_puts("World !!!");

  lcd1602_close();

  hwboard_gpio_close();

  printf("lcd1602_init ok\r\n");

  return 0;
}

int main()
{
  signal(SIGINT, signal_handler);
  signal(SIGTERM, signal_handler);

  return hwhobby_lcd1602();
}

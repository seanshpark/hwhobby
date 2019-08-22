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
#include <hwmodule/encoder.h>

#include <gpio/gpio_interface.h>

#include <signal.h>
#include <stdio.h>

volatile sig_atomic_t __quit = 0;

void signal_handler(int sig)
{
  signal(sig, signal_handler);
  __quit = 1;
}

#define I2C_ADDR_BLUE   0x3F            // my blue backlight LCD is 0x3f
#define I2C_ADDR_YELLOW 0x27            // my yellow backlight LCD is 0x27
#define I2C_ADDR        I2C_ADDR_BLUE

HWRESULT lcd_init(void)
{
  HW_I2C_INIT_t i2cinit;

  i2cinit.i2caddr = I2C_ADDR;
  if (lcd1602_init(&i2cinit) != HWRESULT_SUCCESS)
    return HWRESULT_FAILED;

  lcd1602_home();
  lcd1602_clear();

  return HWRESULT_SUCCESS;
}

void lcd_show_encoder(int value)
{
  char buffer[32];

  sprintf(buffer, "Encoder %3d", value);
  lcd1602_moverc(0, 1);
  lcd1602_puts(buffer);

  printf("LCD: %s\r\n", buffer);
}

int main()
{
  signal(SIGINT, signal_handler);
  signal(SIGTERM, signal_handler);

  if (hwboard_gpio_init() != HWRESULT_SUCCESS)
    return -1;

  if (lcd_init() != HWRESULT_SUCCESS)
    return -1;

  HWM_ENCODER_t encoder_cfg = {{0, 17}, {0, 18}};
  if (encoder_init(&encoder_cfg) != HWRESULT_SUCCESS)
    return -1;

  hwboard_delay(10*1000); // 10 msec

  int encoder_value = 100;

  lcd_show_encoder(encoder_value);

  while (!__quit)
  {
    ENCVAL_t encval = encoder_read(&encoder_cfg);
    if (encval != ENCVAL_NONE)
    {
      encoder_value += encval == ENCVAL_CW ? 1 : -1;

      lcd_show_encoder(encoder_value);
      hwboard_delay(1*1000); // 10 msec
    }
  }

  encoder_close(&encoder_cfg);

  lcd1602_close();

  hwboard_gpio_close();

  return 0;
}

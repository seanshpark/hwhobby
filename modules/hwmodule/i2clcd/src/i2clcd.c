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

#include <hwmodule/i2clcd.h>

#include <hwchip/pcf8574.h>
#include <hwchip/hd44780.h>
#include <gpio/gpio_interface.h>

/**
 * @note  PCF8574_LCD1604_XX describe wire connection of PCF8574 module to
 *        LCD1604/LCD1602 control pins
 */
#define PCF8574_LCD1604_RS  0b00000001
#define PCF8574_LCD1604_RW  0b00000010
#define PCF8574_LCD1604_EN  0b00000100
#define PCF8574_LCD1604_BL  0b00001000

static void _send_byte(uint8_t lcddata, uint32_t delay)
{
  lcddata |= PCF8574_LCD1604_EN;
  pcf8574_send_byte(lcddata);
  hwboard_delay(2);
  lcddata &= ~PCF8574_LCD1604_EN;
  pcf8574_send_byte(lcddata);
  hwboard_delay(delay);
}

HWRESULT i2clcd_init(HW_I2C_INIT_t* pi2cinit)
{
  if (hwboard_gpio_init() != HWRESULT_SUCCESS)
    return HWRESULT_FAILED;

  if (pcf8574_init(pi2cinit) != HWRESULT_SUCCESS)
    return HWRESULT_FAILED;

  return HWRESULT_SUCCESS;
}

void i2clcd_close(void)
{
  pcf8574_close();
  hwboard_gpio_close();
}

void i2clcd_init_4bit(void)
{
  uint8_t lcddata;
  uint8_t data;

  // send RS=0, RW=0, DB7~DB4=0011 as 8bit 3 times
  data = HD44780_LCD_CMD_FUNCSET | HD44780_LCD_FUNCSET_8BIT;

  lcddata = data & 0xf0;
  _send_byte(lcddata, 4500);

  lcddata = data & 0xf0;
  _send_byte(lcddata, 150);

  lcddata = data & 0xf0;
  _send_byte(lcddata, 150);

  // send RS=0, RW=0, DB7~DB4=0010 as 4bit 1 time
  data = HD44780_LCD_CMD_FUNCSET;
  lcddata = data & 0xf0;
  _send_byte(lcddata, 150);
}

void i2clcd_send_ctrl(uint8_t data)
{
  uint8_t lcddata;

  // bit 7~4
  lcddata = (data & 0xf0) | PCF8574_LCD1604_BL;
  _send_byte(lcddata, 10);

  // bit 3~0
  lcddata = ((data & 0x0f) << 4) | PCF8574_LCD1604_BL;
  _send_byte(lcddata, 10);
}

void i2clcd_send_data(uint8_t data)
{
  uint8_t lcddata;

  // bit 7~4
  lcddata = (data & 0xf0) | PCF8574_LCD1604_RS | PCF8574_LCD1604_BL;
  _send_byte(lcddata, 10);

  // bit 3~0
  lcddata = ((data & 0x0f) << 4) | PCF8574_LCD1604_RS | PCF8574_LCD1604_BL;
  _send_byte(lcddata, 10);
}

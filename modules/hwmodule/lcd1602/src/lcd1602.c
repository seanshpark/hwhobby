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

#include <hwmodule/i2clcd.h>
#include <hwchip/hd44780.h>
#include <gpio/gpio_interface.h>

static void _function_set(uint8_t data)
{
  uint8_t cmd = HD44780_LCD_CMD_FUNCSET;
  cmd |= data;
  i2clcd_send_ctrl(cmd);
}

static void _cursor_set(uint8_t data)
{
  uint8_t cmd = HD44780_LCD_CMD_CURSOR;
  cmd |= data;
  i2clcd_send_ctrl(cmd);
}

static void _display_set(uint8_t data)
{
  uint8_t cmd = HD44780_LCD_CMD_DISPLAY;
  cmd |= data;
  i2clcd_send_ctrl(cmd);
}

static void _entrymode_set(uint8_t data)
{
  uint8_t cmd = HD44780_LCD_CMD_ENTRY;
  cmd |= data;
  i2clcd_send_ctrl(cmd);
}

//=============================================================================

HWRESULT lcd1602_init(HW_I2C_INIT_t* pi2cinit)
{
  if (i2clcd_init(pi2cinit) != HWRESULT_SUCCESS)
    return HWRESULT_FAILED;

  // initialize lcd
  i2clcd_init_4bit();
  hwboard_delay(100);

  _function_set(HD44780_LCD_FUNCSET_4BIT | HD44780_LCD_FUNCSET_2LINES |
                HD44780_LCD_FUNCSET_5x8);
  hwboard_delay(100);

  _cursor_set(HD44780_LCD_CURSOR_SHIFT_CUR | HD44780_LCD_CURSOR_RIGHT);
  hwboard_delay(100);

  _display_set(HD44780_LCD_DISPLAY_ON | HD44780_LCD_DISPLAY_CUR |
               HD44780_LCD_DISPLAY_BLINK);
  hwboard_delay(100);

  _entrymode_set(HD44780_LCD_ENTRY_INC);
  hwboard_delay(100);

  lcd1602_clear();
  hwboard_delay(100);

  return HWRESULT_SUCCESS;
}

void lcd1602_close(void)
{
  i2clcd_close();
}

//=============================================================================

void lcd1602_home(void)
{
  uint8_t cmd = HD44780_LCD_CMD_RETHOME;
  i2clcd_send_ctrl(cmd);
  hwboard_delay(5000); // max 5msec
}

void lcd1602_clear(void)
{
  uint8_t cmd = HD44780_LCD_CMD_CLEAR;
  i2clcd_send_ctrl(cmd);
  hwboard_delay(5000); // max 5msec
}

void lcd1602_putc(const char c)
{
  i2clcd_send_data(c);
  hwboard_delay(120);
}

void lcd1602_puts(const char* str)
{
  while (*str != '\x0')
  {
    lcd1602_putc(*str++);
  }
}

void lcd1602_moverc(uint8_t row, uint8_t col)
{
  uint8_t cmd = HD44780_LCD_CMD_DDRAMADDR;
  uint8_t ram_offset;

  // row 0 : 0x00 ~ 0x27
  // row 1 : 0x40 ~ 0x67
  if (row > 0x01) row = 0x01;
  if (col > 0x27) col = 0x27;

  ram_offset = row * 0x40 + col;
  ram_offset &= 0b01111111;

  i2clcd_send_ctrl(cmd + ram_offset);
  hwboard_delay(120);
}

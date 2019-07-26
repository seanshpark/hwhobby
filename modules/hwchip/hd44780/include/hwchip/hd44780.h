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

#ifndef __HWCHIP_HD44780_H__
#define __HWCHIP_HD44780_H__

#include <hwabc/standard.h>

#ifdef __cplusplus
extern "C" {
#endif

#define HD44780_LCD_CMD_DDRAMADDR     0b10000000
#define HD44780_LCD_CMD_CGRAMADDR     0b01000000
#define HD44780_LCD_CMD_FUNCSET       0b00100000
#define HD44780_LCD_CMD_CURSOR        0b00010000
#define HD44780_LCD_CMD_DISPLAY       0b00001000
#define HD44780_LCD_CMD_ENTRY         0b00000100
#define HD44780_LCD_CMD_RETHOME       0b00000010
#define HD44780_LCD_CMD_CLEAR         0b00000001

// HD44780_LCD_CMD_FUNCSET
#define HD44780_LCD_FUNCSET_8BIT      0b00010000
#define HD44780_LCD_FUNCSET_4BIT      0b00000000
#define HD44780_LCD_FUNCSET_2LINES    0b00001000
#define HD44780_LCD_FUNCSET_1LINES    0b00000000
#define HD44780_LCD_FUNCSET_5x11      0b00000100
#define HD44780_LCD_FUNCSET_5x8       0b00000000

// HD44780_LCD_CMD_CURSOR
#define HD44780_LCD_CURSOR_SHIFT      0b00001000  // display shift vs cursor move
#define HD44780_LCD_CURSOR_RIGHT      0b00000100  // shift to the right vs left

// HD44780_LCD_CMD_DISPLAY
#define HD44780_LCD_DISPLAY_ON        0b00000100  // vs display off
#define HD44780_LCD_DISPLAY_CUR       0b00000010  // vs cursor off
#define HD44780_LCD_DISPLAY_BLINK     0b00000001  // cs blink off

// HD44780_LCD_CMD_ENTRY
#define HD44780_LCD_ENTRY_INC         0b00000010  // increment vs decrement
#define HD44780_LCD_ENTRY_SHIFT       0b00000001  // entire shift on vs off

#ifdef __cplusplus
}
#endif

#endif // __HWCHIP_HD44780_H__

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

#ifndef __MODULE_LCD1602_H__
#define __MODULE_LCD1602_H__

#include <hwabc/standard.h>

#ifdef __cplusplus
extern "C" {
#endif

HWRESULT lcd1602_init(ioport_t i2caddr);
void lcd1602_close(void);

void lcd1602_home(void);
void lcd1602_clear(void);
void lcd1602_putc(const char c);
void lcd1602_puts(const char* str);
void lcd1602_moverc(uint8_t row, uint8_t col);

#ifdef __cplusplus
}
#endif

#endif // __MODULE_LCD1602_H__

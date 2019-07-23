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

#ifndef __MODULE_I2C_TO_LCD_H__
#define __MODULE_I2C_TO_LCD_H__

#include <hwabc/standard.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief  This module is used as a I/O path from I2C to LCD
 *
 * @note   Converts I2C to 8 bit
 *         High 4 bits are used as data signals to LCD1602 module
 *         Low 4 bits are used as control signals
 */

void i2clcd_init_4bit(void);
void i2clcd_send_ctrl(uint8_t data);
void i2clcd_send_data(uint8_t data);

#ifdef __cplusplus
}
#endif

#endif // __MODULE_I2C_TO_LCD_H__

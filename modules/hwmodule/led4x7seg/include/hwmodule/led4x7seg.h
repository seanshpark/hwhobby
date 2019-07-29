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

#ifndef __MODULE_LED4x7SEG_H__
#define __MODULE_LED4x7SEG_H__

#include <hwabc/standard.h>

#ifdef __cplusplus
extern "C" {
#endif

HWRESULT led4x7seg_init(HW_GPIO_t* gpio_clock, HW_GPIO_t* gpio_data);
void led4x7seg_close(void);

typedef struct tag_LED4X7SEG
{
  uint8_t digit[4]; // 4 digit MSB is index 0 LSB is index 3
  uint8_t colon;    // 0 to hide ':' other to show ':'

} LED4X7SEG_t;

void led4x7seg_show(LED4X7SEG_t* data);

void led4x7seg_bright(uint8_t value);

// brightness LED4x7SEG_BR_01 is 1/16 brigh
// maximum is LED4x7SEG_BR_14 which is 14/16 bright
#define LED4x7SEG_BR_01  0
#define LED4x7SEG_BR_02  1
#define LED4x7SEG_BR_04  3
#define LED4x7SEG_BR_10  4
#define LED4x7SEG_BR_11  5
#define LED4x7SEG_BR_12  6
#define LED4x7SEG_BR_13  7
#define LED4x7SEG_BR_14  8

// 7 Segment LED bit
//
//      A
//     ---
//  F |   | B
//     -G-
//  E |   | C
//     ---
//      D
//
//   :GFEDCBA
// 0b00111111,  0
// 0b00000110,  1
// 0b01011011,  2
// 0b01001111,  3 
// 0b01100110,  4
// 0b01101101,  5
// 0b01111101,  6
// 0b00000111,  7
// 0b01111111,  8
// 0b01101111,  9
// 0b01110111,  A
// 0b01111100,  b
// 0b00111001,  C
// 0b01011110,  d
// 0b01111001,  E
// 0b01110001   F

#ifdef __cplusplus
}
#endif

#endif // __MODULE_LED4x7SEG_H__
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

#ifndef __HWHOBBY_STANDARD_H__
#define __HWHOBBY_STANDARD_H__

//
// types for common
//
#ifdef __cplusplus

#include <cstdint>

#else // __cplusplus

typedef unsigned char uint8_t;
typedef unsigned short int uint16_t;
typedef unsigned int uint32_t;

#ifdef __arm__
typedef unsigned long long uint64_t;
#endif

#ifdef __aarch64__
typedef unsigned long uint64_t;
#endif

#endif // __cplusplus

//
// types for H/W
//
typedef uint16_t ioport_t;
typedef uint64_t hwusec_t;

// device result of commands, init and others with return
typedef enum {

  HWRESULT_FAILED = 0,
  HWRESULT_SUCCESS = 1

} HWRESULT;

// GPIO interface
typedef struct _HW_GPIO_t
{
  ioport_t port;
  ioport_t pin;

} HW_GPIO_t;

typedef struct _HW_GPIO_CFG_t
{
  ioport_t port;
  ioport_t pin;

  uint8_t pud;
  uint8_t fsel;

} HW_GPIO_CFG_t;

//
// I2C initialization structure
//
typedef struct _HW_I2C_INIT_t
{
  ioport_t i2c_num; // i2c number, 0 start

  ioport_t i2cport; // port for i2c: assume port should be the same
  ioport_t i2cpin_scl; // pin for clock
  ioport_t i2cpin_sda; // pin for data

  ioport_t i2caddr; // slave addr

} HW_I2C_INIT_t;

#endif // __HWHOBBY_STANDARD_H__

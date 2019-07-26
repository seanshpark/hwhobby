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
typedef unsigned long long uint64_t;

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

#endif // __HWHOBBY_STANDARD_H__

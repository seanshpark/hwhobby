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

#ifndef __MODULE_ENCODER_H__
#define __MODULE_ENCODER_H__

#include <hwabc/standard.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct _HWM_ENCODER_t
{
  HW_GPIO_t port_a; // Port A
  HW_GPIO_t port_b; // Port B

} HWM_ENCODER_t;

HWRESULT encoder_init(HWM_ENCODER_t* gpio_encoder);
void encoder_close(HWM_ENCODER_t* gpio_encoder);

typedef enum _ENCVAL_t
{
  ENCVAL_NONE = 0,
  ENCVAL_CW = 1,
  ENCVAL_CCW = 2,

} ENCVAL_t;

ENCVAL_t encoder_read(HWM_ENCODER_t* gpio_encoder);


#ifdef __cplusplus
}
#endif

#endif // __MODULE_ENCODER_H__

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

#ifndef __MODULE_HC_SR04_H__
#define __MODULE_HC_SR04_H__

#include <hwabc/standard.h>

#ifdef __cplusplus
extern "C" {
#endif

int hcsr04_init(ioport_t trig_port, ioport_t trig_pin, ioport_t echo_port, ioport_t echo_pin);
void hcsr04_close(void);

int hcsr04_measure_time(void);

#ifdef __cplusplus
}
#endif

#endif // __MODULE_HC_SR04_H__

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

#include <hwchip/pcf8574.h>

#include <gpio/gpio_interface.h>

/**
 * @brief  PCF8574 is I2C to 8 bit I/O expander
 *
 * @note   Configuring output P0~P7 is user defined
 */

HWRESULT pcf8574_init(HW_I2C_INIT_t* pi2cinit)
{
  if (hwboard_i2c_open(pi2cinit) != HWRESULT_SUCCESS)
    return HWRESULT_FAILED;

  return HWRESULT_SUCCESS;
}

void pcf8574_close(void)
{
  hwboard_i2c_close();
}

void pcf8574_send_byte(uint8_t one)
{
  hwboard_i2c_send_byte(one);
}

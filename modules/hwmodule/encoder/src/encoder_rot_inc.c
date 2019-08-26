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

#include <hwmodule/encoder.h>

#include <gpio/gpio_interface.h>

HWRESULT encoder_init(HWM_ENCODER_t* gpio_encoder)
{
  HW_GPIO_CFG_t gpiocfg_a = {
    gpio_encoder->port_a.port,
    gpio_encoder->port_a.pin,
    HWBOARD_GPIO_PUD_OFF,
    HWBOARD_GPIO_FSEL_INP
  };
  HW_GPIO_CFG_t gpiocfg_b = {
    gpio_encoder->port_b.port,
    gpio_encoder->port_b.pin,
    HWBOARD_GPIO_PUD_OFF,
    HWBOARD_GPIO_FSEL_INP
  };

  // set 'pull up' to off: external pull up should exist
  // function select to input
  hwboard_gpio_cfg(&gpiocfg_a);
  hwboard_gpio_cfg(&gpiocfg_b);

  // we want to use falling edge detection of port_a
  hwboard_gpio_fen(&gpio_encoder->port_a, 1);
  // wait for a while
  hwboard_delay(3*1000); // 3 msec

  return HWRESULT_SUCCESS;
}

void encoder_close(HWM_ENCODER_t* gpio_encoder)
{
  hwboard_gpio_fen(&gpio_encoder->port_a, 0);
}

ENCVAL_t encoder_read(HWM_ENCODER_t* gpio_encoder)
{
  if (hwboard_gpio_eds(&gpio_encoder->port_a, 0))
  {
    hwboard_gpio_eds(&gpio_encoder->port_a, 1);
    hwboard_delay(1*1000); // 1 msec

    ioport_t enc_a_val = hwboard_gpio_get(&gpio_encoder->port_a);
    // we want to be sure port_a is low
    if (enc_a_val == 0)
    {
      // CW or CCW depends on port_b value
      ioport_t enc_b_val = hwboard_gpio_get(&gpio_encoder->port_b);
      return enc_b_val ? ENCVAL_CCW : ENCVAL_CW;
    }
  }
  return ENCVAL_NONE;
}

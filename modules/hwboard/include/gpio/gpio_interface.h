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

#ifndef __HWBOARD_GPIO_INTERFACE_H__
#define __HWBOARD_GPIO_INTERFACE_H__

#include <hwabc/standard.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Initialize gpio
 */
HWRESULT hwboard_gpio_init(void);

/**
 * @brief Close gpio
 */
void hwboard_gpio_close(void);

/**
 * @brief Set port to High
 */
void hwboard_gpio_set(HW_GPIO_t* gpio);

/**
 * @brief Clear port to Low
 */
void hwboard_gpio_clr(HW_GPIO_t* gpio);

/**
 * @brief Get port value
 */
ioport_t hwboard_gpio_get(HW_GPIO_t* gpio);

/**
 * @brief Set falling edge detect enable
 *
 * @note  Experimental
 */
void hwboard_gpio_fen(HW_GPIO_t* gpio, uint8_t enable);
uint8_t hwboard_gpio_eds(HW_GPIO_t* gpio, uint8_t clear);

/**
 * @brief Configure port
 * 
 * @note  This may change. RPi for now.
 */
void hwboard_gpio_cfg(HW_GPIO_CFG_t* gpiocfg);

#define HWBOARD_GPIO_PUD_OFF 0x00
#define HWBOARD_GPIO_PUD_DN 0x01
#define HWBOARD_GPIO_PUD_UP 0x02

#define HWBOARD_GPIO_FSEL_INP 0x00
#define HWBOARD_GPIO_FSEL_OUT 0x01

void hwboard_delay(int usec);
void hwboard_delay_ms(int msec);

/**
 * current system time in usec
 */
hwusec_t hwboard_time(void);

//
// Experiments with I2C
//  - build up with RPi
//  - need to test with STM32F103
//

HWRESULT hwboard_i2c_open(HW_I2C_INIT_t* pi2cinit);
void hwboard_i2c_close(void);
void hwboard_i2c_send_byte(uint8_t data);

#ifdef __cplusplus
}
#endif


#endif // __HWBOARD_GPIO_INTERFACE_H__

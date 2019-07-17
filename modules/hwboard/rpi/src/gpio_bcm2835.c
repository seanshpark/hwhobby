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

#include <gpio/gpio_interface.h>
#include <bcm2835/bcm2835.h>

#include <assert.h>
#include <unistd.h> // for usleep

int hwboard_gpio_init(void)
{
  if (!bcm2835_init())
  {
    return 0;
  }
  return 1;
}

void hwboard_gpio_close(void)
{
  bcm2835_close();
}

#define RPI_V2_GPIO_P1_XX 0xffff

static RPiGPIOPin hwpin_from_gpio(ioport_t gpio)
{
  // These are actual gpio number in real world.
  // gpio 0 is GPIO 0, which is pin #3
  // non existance gpio is marked as RPI_V2_GPIO_P1_XX
  static const ioport_t hwpin_table[] = {
    RPI_V2_GPIO_P1_XX, // 0
    RPI_V2_GPIO_P1_XX, // 1
    RPI_V2_GPIO_P1_03, // GPIO2  I2C-SDA
    RPI_V2_GPIO_P1_05, // GPIO3  I2C-SCL
    RPI_V2_GPIO_P1_07, // GPIO4
    RPI_V2_GPIO_P1_XX, // 5
    RPI_V2_GPIO_P1_XX, // 6
    RPI_V2_GPIO_P1_26, // GPIO7  SPI0-CE1
    RPI_V2_GPIO_P1_24, // GPIO8  SPI0-CE0
    RPI_V2_GPIO_P1_21, // GPIO9  SPI0-MISO
    RPI_V2_GPIO_P1_19, // GPIO10 SPI0-MOSI
    RPI_V2_GPIO_P1_23, // GPIO11 SPI0-CLK
    RPI_V2_GPIO_P1_XX, // 12
    RPI_V2_GPIO_P1_XX, // 13
    RPI_V2_GPIO_P1_08, // GPIO14 UART0-TXD
    RPI_V2_GPIO_P1_10, // GPIO15 UART0-RXD
    RPI_V2_GPIO_P1_XX, // 16
    RPI_V2_GPIO_P1_11, // GPIO17
    RPI_V2_GPIO_P1_12, // GPIO18 PWM
    RPI_V2_GPIO_P1_XX, // 19
    RPI_V2_GPIO_P1_XX, // 20
    RPI_V2_GPIO_P1_XX, // 21
    RPI_V2_GPIO_P1_15, // GPIO22
    RPI_V2_GPIO_P1_16, // GPIO23
    RPI_V2_GPIO_P1_18, // GPIO24
    RPI_V2_GPIO_P1_22, // GPIO25
  };
  static const int count = 26;

  if (gpio >= count)
    return RPI_V2_GPIO_P1_XX;

  return hwpin_table[gpio];
}

void hwboard_gpio_set(ioport_t port, ioport_t gpio)
{
  RPiGPIOPin hwpin = hwpin_from_gpio(gpio);

  if (hwpin != RPI_V2_GPIO_P1_XX)
    bcm2835_gpio_set(hwpin);
}

void hwboard_gpio_clr(ioport_t port, ioport_t gpio)
{
  RPiGPIOPin hwpin = hwpin_from_gpio(gpio);

  if (hwpin != RPI_V2_GPIO_P1_XX)
    bcm2835_gpio_clr(hwpin);
}

void hwboard_gpio_cfg(ioport_t port, ioport_t gpio, uint8_t pudin, uint8_t fselin)
{
  RPiGPIOPin hwpin = hwpin_from_gpio(gpio);
  bcm2835PUDControl pud = BCM2835_GPIO_PUD_OFF;
  bcm2835FunctionSelect fsel = BCM2835_GPIO_FSEL_INPT;

  if (hwpin == RPI_V2_GPIO_P1_XX)
    return;
  
  switch (pudin)
  {
  case HWBOARD_GPIO_PUD_OFF: pud = BCM2835_GPIO_PUD_OFF; break;
  case HWBOARD_GPIO_PUD_DN: pud = BCM2835_GPIO_PUD_DOWN; break;
  case HWBOARD_GPIO_PUD_UP: pud = BCM2835_GPIO_PUD_UP; break;
  }
  bcm2835_gpio_set_pud(hwpin, pud);

  switch (fselin)
  {
  case HWBOARD_GPIO_FSEL_INP: fsel = BCM2835_GPIO_FSEL_INPT; break;
  case HWBOARD_GPIO_FSEL_OUT: fsel = BCM2835_GPIO_FSEL_OUTP; break;
  }
  bcm2835_gpio_fsel(hwpin, fsel);
}

void hwboard_delay(int usec)
{
  usleep(usec);
}

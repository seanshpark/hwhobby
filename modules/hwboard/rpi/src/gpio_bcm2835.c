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

#define RPI_GPIO_P1_XX 0xffff

RPiGPIOPin hwpin_from_pin(ioport_t pin)
{
  // These are actual pin number in real world.
  // 'pin' is 0 base but doesn't exist in real world, marked as RPI_GPIO_P1_XX
  // 'pin' 1 is 3.3V power, 2 is 5V power, also marked as RPI_GPIO_P1_XX,
  // and 'pin' that isn't GPIO is marked as RPI_GPIO_P1_XX.
  static const ioport_t hwpin_table[] = {
    RPI_GPIO_P1_XX, RPI_GPIO_P1_XX, RPI_GPIO_P1_XX, RPI_GPIO_P1_03, 
    RPI_GPIO_P1_XX, RPI_GPIO_P1_05, RPI_GPIO_P1_XX, RPI_GPIO_P1_07,
    RPI_GPIO_P1_08, RPI_GPIO_P1_XX, RPI_GPIO_P1_10, RPI_GPIO_P1_11,
    RPI_GPIO_P1_12, RPI_GPIO_P1_13, RPI_GPIO_P1_XX, RPI_GPIO_P1_15,
    RPI_GPIO_P1_16, RPI_GPIO_P1_XX, RPI_GPIO_P1_18, RPI_GPIO_P1_19,
    RPI_GPIO_P1_XX, RPI_GPIO_P1_21, RPI_GPIO_P1_22, RPI_GPIO_P1_23,
    RPI_GPIO_P1_24, RPI_GPIO_P1_XX, RPI_GPIO_P1_26
  };
  static const int count = 27;

  if (pin >= count)
    return RPI_GPIO_P1_XX;

  return hwpin_table[pin];
}

void hwboard_gpio_set(ioport_t port,  ioport_t pin)
{
  RPiGPIOPin hwpin = hwpin_from_pin(pin);

  if (hwpin != RPI_GPIO_P1_XX)
    bcm2835_gpio_set(hwpin);
}

void hwboard_gpio_clr(ioport_t port, ioport_t pin)
{
  RPiGPIOPin hwpin = hwpin_from_pin(pin);

  if (hwpin != RPI_GPIO_P1_XX)
    bcm2835_gpio_clr(hwpin);
}

void hwboard_gpio_cfg(ioport_t port, ioport_t pin, uint8_t pudin, uint8_t fselin)
{
  RPiGPIOPin hwpin = hwpin_from_pin(pin);
  bcm2835PUDControl pud = BCM2835_GPIO_PUD_OFF;
  bcm2835FunctionSelect fsel = BCM2835_GPIO_FSEL_INPT;

  if (hwpin == RPI_GPIO_P1_XX)
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

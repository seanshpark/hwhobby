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
#include <time.h>
#include <unistd.h> // for usleep

// multiple call of init
static uint8_t __gpio_init = 0;

HWRESULT hwboard_gpio_init(void)
{
  if (!__gpio_init)
  {
    if (bcm2835_init() != HWRESULT_SUCCESS)
    {
      return HWRESULT_FAILED;
    }
    __gpio_init = 1;
  }
  return HWRESULT_SUCCESS;
}

void hwboard_gpio_close(void)
{
  if (__gpio_init)
  {
    bcm2835_close();
    __gpio_init = 0;
  }
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

void hwboard_gpio_set(HW_GPIO_t* gpio)
{
  RPiGPIOPin hwpin = hwpin_from_gpio(gpio->pin);

  if (hwpin != RPI_V2_GPIO_P1_XX)
    bcm2835_gpio_set(hwpin);
}

void hwboard_gpio_clr(HW_GPIO_t* gpio)
{
  RPiGPIOPin hwpin = hwpin_from_gpio(gpio->pin);

  if (hwpin != RPI_V2_GPIO_P1_XX)
    bcm2835_gpio_clr(hwpin);
}

ioport_t hwboard_gpio_get(HW_GPIO_t* gpio)
{
  RPiGPIOPin hwpin = hwpin_from_gpio(gpio->pin);

  if (hwpin != RPI_V2_GPIO_P1_XX)
    return bcm2835_gpio_lev(hwpin);

  return 0;
}

void hwboard_gpio_cfg(HW_GPIO_CFG_t* gpiocfg)
{
  RPiGPIOPin hwpin = hwpin_from_gpio(gpiocfg->pin);
  bcm2835PUDControl pud = BCM2835_GPIO_PUD_OFF;
  bcm2835FunctionSelect fsel = BCM2835_GPIO_FSEL_INPT;

  if (hwpin == RPI_V2_GPIO_P1_XX)
    return;
  
  switch (gpiocfg->pud)
  {
  case HWBOARD_GPIO_PUD_OFF: pud = BCM2835_GPIO_PUD_OFF; break;
  case HWBOARD_GPIO_PUD_DN: pud = BCM2835_GPIO_PUD_DOWN; break;
  case HWBOARD_GPIO_PUD_UP: pud = BCM2835_GPIO_PUD_UP; break;
  }
  bcm2835_gpio_set_pud(hwpin, pud);

  switch (gpiocfg->fsel)
  {
  case HWBOARD_GPIO_FSEL_INP: fsel = BCM2835_GPIO_FSEL_INPT; break;
  case HWBOARD_GPIO_FSEL_OUT: fsel = BCM2835_GPIO_FSEL_OUTP; break;
  }
  bcm2835_gpio_fsel(hwpin, fsel);
}

void hwboard_delay(int usec)
{
  // usleep(usec);
  bcm2835_delayMicroseconds(usec);
}

void hwboard_delay_ms(int msec)
{
  uint64_t usec = msec * 1000ull;
  bcm2835_delayMicroseconds(usec);
}

///@ elapased time in usec
hwusec_t hwboard_time(void)
{
  struct timespec time;
  hwusec_t time_val;

  clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &time);

  time_val = time.tv_nsec / 1000ull;
  time_val += time.tv_sec * 1000000ull;

  return time_val;
}

HWRESULT hwboard_i2c_open(HW_I2C_INIT_t* pi2cinit)
{
  bcm2835_i2c_begin();
  bcm2835_i2c_setSlaveAddress(pi2cinit->i2caddr);

  return HWRESULT_SUCCESS;
}

void hwboard_i2c_close(void)
{
  bcm2835_i2c_end();
}

void hwboard_i2c_send_byte(uint8_t data)
{
  char buf[1];
  buf[0] = (char)data;
  bcm2835_i2c_write(buf, 1);
}

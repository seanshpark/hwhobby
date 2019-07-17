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

#include <hwmodule/led4x7seg.h>
#include <bcm2835/bcm2835.h>

#include <signal.h>
#include <stdio.h>
#include <unistd.h>

// RPi
ioport_t gpio_clock  = 23; // GPIO23
ioport_t gpio_dataio = 24; // GPIO24

volatile sig_atomic_t quit;

void signal_handler(int sig)
{
  signal(sig, signal_handler);
  quit = 1;
}

void fill_data(LED4X7SEG_t* data, int value)
{
  char display[16]; // enough amount of buffer

  sprintf(display, "%02d%02d", value / 100, value % 100);
  printf("-- %s\n", display);

  for (int i = 0; i < 4; i++)
    data->digit[i] = display[i];

  data->colon = value & 1;
}

int hwhobby_counter(void)
{
  if (!led4x7seg_init(0, gpio_clock, 0, gpio_dataio))
    return -1;

  uint8_t brightness = LED4x7SEG_BR_04;

  led4x7seg_bright(brightness);

  for (int i = 0; i < 9999; i++)
  {
    if (quit) break;

    LED4X7SEG_t display;
    fill_data(&display, i);
    led4x7seg_show(&display);

    usleep(500000);
    if (quit) break;

    // led4x7seg_bright(brightness);
    // brightness++;
    // brightness = brightness > LED4x7SEG_BR_14 ? LED4x7SEG_BR_02 : brightness;
  }

  led4x7seg_close();

  return 0;
}

int main()
{
  signal(SIGINT, signal_handler);
  signal(SIGTERM, signal_handler);

  return hwhobby_counter();
}

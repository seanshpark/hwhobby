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

#include <hwchip/tm1637.h>

/**
 * @note led4x7seg uses 2 pins for clock and data 
 */
HWRESULT led4x7seg_init(ioport_t clock_port, ioport_t clock_pin, ioport_t data_port,
                        ioport_t data_pin)
{
  if (tm1637_init(clock_port, clock_pin, data_port, data_pin) != HWRESULT_SUCCESS)
    return HWRESULT_FAILED;

  return HWRESULT_SUCCESS;
}

void led4x7seg_close(void)
{
  led4x7seg_bright(0);

  tm1637_close();
}

static const unsigned char base10_to_segment[] = {
  0b00111111, 0b00000110, 0b01011011, 0b01001111, 0b01100110,
  0b01101101, 0b01111101, 0b00000111, 0b01111111, 0b01101111
};

static uint8_t segment_value(uint8_t digit)
{
  return base10_to_segment[digit & 0x0f];
}

static void validate_base10(LED4X7SEG_t* data)
{
  for (int i = 0; i < 4; i++)
  {
    if (data->digit[i] < '0' || data->digit[i] > '9')
      data->digit[i] = '0';
  }
}

void led4x7seg_show(LED4X7SEG_t* data)
{
  validate_base10(data);

  uint8_t command;
  uint8_t segdata[6];
  int src, dst;
   
  command = TM1637_CMD_DATA | TM1637_DATA_AUTOINC | TM1637_DATA_NORMAL;
  tm1637_write(command);

  dst = src = 0;
  segdata[dst++] = TM1637_CMD_ADDR | TM1637_ADDR_C0H;
  segdata[dst++] = segment_value(data->digit[src++]);
  segdata[dst++] = segment_value(data->digit[src++]);
  segdata[dst++] = segment_value(data->digit[src++]);
  segdata[dst++] = segment_value(data->digit[src++]);
  
  segdata[2] |= data->colon ? TM1637_DBIT_COLON : 0x00;

  tm1637_writes(segdata, dst);
}

void led4x7seg_bright(uint8_t value)
{
  tm1637_bright(value);
}

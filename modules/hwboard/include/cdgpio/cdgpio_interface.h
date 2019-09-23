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

#ifndef __HWBOARD_CDGPIO_INTERFACE_H__
#define __HWBOARD_CDGPIO_INTERFACE_H__

#include <string>
#include <map>

#include <time.h>

#include <gpiod.h>

namespace hwboard
{

using CdDevice = std::string;
using CdNumber = uint32_t;

class CdGpio;

class CdLine
{
public:
  enum class InputType
  {
    Normal,
    FallingEdge,
  };

  enum class EventType
  {
    Timeout,
    FallingEdge,
  };

public:
  CdLine(CdGpio* cdgpio, struct gpiod_line* gline)
    : _cdgpio(cdgpio), _gline(gline), _error(false)
  {
    // do nothing
  };

public:
  bool as_input(InputType type);
  bool as_output(void);

public:
  bool wait(struct timespec& ts);
  EventType wait_edge(struct timespec& ts);

public:
  bool read(void);
  bool error(void) const { return _error; }

private:
  CdGpio* _cdgpio;
  struct gpiod_line* _gline;
  bool _error;
};

class CdGpio
{
public:
  CdGpio() = default;

public:
  bool open(CdNumber number);
  void close(void);

public:
  CdLine* line(CdNumber number);

private:
  typedef std::map<CdNumber, CdLine*> CdMapNumberLine;

  struct gpiod_chip* _chip{nullptr};
  CdNumber _num_lines;
  CdMapNumberLine _lines;
};

void hwboard_delay(int usec);
void hwboard_delay_ms(int msec);

} // namespace hwboard

#endif // __HWBOARD_CDGPIO_INTERFACE_H__

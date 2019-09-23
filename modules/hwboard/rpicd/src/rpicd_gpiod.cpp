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

#include <cdgpio/cdgpio_interface.h>

#include <cassert>
#include <iostream>

 #include <unistd.h>

namespace
{

const char* _consumer_name = "hwhobby";

} // namespace

namespace hwboard
{

bool CdGpio::open(CdNumber number)
{
  assert(_chip == nullptr);

  _chip = gpiod_chip_open_by_number(number);
  if (!_chip)
  {
    return false;
  }

  std::cout << "CdGpio::open(" << number << ") --> " << gpiod_chip_name(_chip)
            << " : " << gpiod_chip_label(_chip) << std::endl;

  _num_lines = gpiod_chip_num_lines(_chip);

  return true;
}

void CdGpio::close(void)
{
  for (auto iter = _lines.begin(); iter != _lines.end(); iter++)
  {
    CdLine* cdline = iter->second;
    delete cdline;
    iter->second = nullptr;
  }

  if (_chip)
  {
    gpiod_chip_close(_chip);
  }
  _chip = nullptr;
}

CdLine* CdGpio::line(CdNumber number)
{
  if (number >= _num_lines)
    return nullptr;

  auto iter = _lines.find(number);
  if (iter == _lines.end())
  {
    struct gpiod_line* gline = gpiod_chip_get_line(_chip, number);
    if (gline != NULL)
    {
      CdLine* cdline = new CdLine(this, gline);
      _lines[number] = cdline;
      return cdline;
    }
  }
  return _lines[number];
}

bool CdLine::as_input(InputType type)
{
  switch (type)
  {
    case InputType::Normal:
      return 0 == gpiod_line_request_input(_gline, _consumer_name);

    case InputType::FallingEdge:
      return 0 == gpiod_line_request_falling_edge_events(_gline, _consumer_name);
  }
  return false;
}

bool CdLine::as_output(void)
{
  return 0 == gpiod_line_request_output(_gline, _consumer_name, 0);
}

bool CdLine::wait(struct timespec& ts)
{
  return 1 == gpiod_line_event_wait(_gline, &ts);
}

CdLine::EventType CdLine::wait_edge(struct timespec& ts)
{
  if (1 == gpiod_line_event_wait(_gline, &ts))
  {
    struct gpiod_line_event ev;
    int rv = gpiod_line_event_read(_gline, &ev);

    if (0 == rv && ev.event_type == GPIOD_LINE_EVENT_FALLING_EDGE);
      return EventType::FallingEdge;
  }
  return EventType::Timeout;
}

bool CdLine::read(void)
{
  int value = gpiod_line_get_value(_gline);
  if (value < 0)
    _error = true;

  return 1 == value;
}

void hwboard_delay(int usec)
{
  usleep(usec);
}

void hwboard_delay_ms(int msec)
{
  usleep(msec * 1000);
}

} // namespace hwboard

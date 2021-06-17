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

#ifndef __MUVMODULE_MUVPORT_H__
#define __MUVMODULE_MUVPORT_H__

#include <hwabc/standard.h>

#include <gpiod.h>

#include <muv/muv.h>

namespace muv
{

class Port : public Handle
{
public:
  enum class Event
  {
    FALLING_EDGE,
  };
  using Line = gpiod_line;

public:
  typedef void (*PortCB) (Port*);

public:
  Port() = default;

public:
  bool init(Loop*, Line* line);

protected:
  void on_tick(void) override;

protected:
  bool active(void) const override;

public:
  bool start(PortCB, Event);
  bool stop(void) override;

public:
  gpiod_line* line(void) const { return line_; }

private:
  PortCB port_cb_{nullptr};
  Line* line_{nullptr};
};

} // namespace muv

#endif // __MUVMODULE_MUVPORT_H__

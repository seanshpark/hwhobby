#
# Copyright 2019 saehie.park@gmail.com
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
#

#
# define target 'hwgpiodef'
#
add_library(hwgpiodef INTERFACE)
target_include_directories(hwgpiodef INTERFACE include)

#
# define target 'hwboardgpio' for the board 
#
message(STATUS "TARGET_BOARD = ${TARGET_BOARD}")

if("${TARGET_BOARD}" STREQUAL "rpi")
  add_subdirectory(rpi)
endif()

if("${TARGET_BOARD}" STREQUAL "rpi64")
  add_subdirectory(rpi)
endif()

if("${TARGET_BOARD}" STREQUAL "stm32f103c8t6")
  add_subdirectory(stm32f103c8t6)
  set(STM32F103_LINKER_FLAGS ${STM32F103_LINKER_FLAGS} PARENT_SCOPE)
endif()

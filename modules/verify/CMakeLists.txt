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
# verifiy projects depending on the target board
#

if("${TARGET_BOARD}" STREQUAL "rpi")

endif()

if("${TARGET_BOARD}" STREQUAL "rpi64")
  add_subdirectory(rpi64_hello)
  add_subdirectory(rpi64_libgpiod)
endif()

if("${TARGET_BOARD}" STREQUAL "rpicd")
  add_subdirectory(rpicd_lcd1602)
  add_subdirectory(rpicd_hellogpio)
  add_subdirectory(rpicd_runmuv)
  add_subdirectory(rpicd_runmuvport)
endif()

if("${TARGET_BOARD}" STREQUAL "stm32f103c8t6")

endif()

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

add_subdirectory(libstm)

#
# interface target from local target
#
add_library(hwbstm32f103 INTERFACE)
target_link_libraries(hwbstm32f103 INTERFACE hwbstm32f103_)

#
# hwboardgpio for STM32F103
#
set(SRC src/stm32f103c8t6.c
        src/gpio_stm32f103c8t6.c
)

add_library(hwboardgpio STATIC ${SRC})
target_include_directories(hwboardgpio PUBLIC include)
target_include_directories(hwboardgpio PRIVATE libstm/conf)
target_include_directories(hwboardgpio PRIVATE libstm/it)
target_include_directories(hwboardgpio PRIVATE libstm/include)
target_link_libraries(hwboardgpio PUBLIC hwabc)
target_link_libraries(hwboardgpio PUBLIC hwgpiodef)
target_link_libraries(hwboardgpio PUBLIC hwbstm32f103)

set(LF "")
set(LF "${LF} --specs=nosys.specs")
set(LF "${LF} -Wl,--gc-sections,-Map=executable.map,-cref,-u,Reset_Handler")
set(LF "${LF} -I ${CMAKE_CURRENT_SOURCE_DIR}/include")
set(LF "${LF} -L ${CMAKE_CURRENT_SOURCE_DIR}/libstm/lib")
set(LF "${LF} -T ${CMAKE_CURRENT_SOURCE_DIR}/libstm/lib/stm32.ld")

set(STM32F103_LINKER_FLAGS ${LF})
set(STM32F103_LINKER_FLAGS ${LF} PARENT_SCOPE)

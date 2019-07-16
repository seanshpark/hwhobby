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
# target stm_blinker is a blinker example from the source code as-is released from ST-M
#
set(SRC src/main.c)

set(CMAKE_EXE_LINKER_FLAGS "${STM32F103_LINKER_FLAGS}")

add_executable(stm_blinker ${SRC})
target_link_libraries(stm_blinker hwboardgpio)

# convert elf file to binary file
add_custom_command(TARGET stm_blinker
                   POST_BUILD
                   COMMAND ${CMAKE_OBJCOPY} -O binary 
                      ${CMAKE_CURRENT_BINARY_DIR}/stm_blinker
                      ${CMAKE_CURRENT_BINARY_DIR}/stm_blinker.bin
                   WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
)

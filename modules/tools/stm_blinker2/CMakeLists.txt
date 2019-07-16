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
# target stm_blinker2 is a blinker example base on the libraries of this project
#
set(SRC src/blinker.cpp)

set(CMAKE_EXE_LINKER_FLAGS "${STM32F103_LINKER_FLAGS}")

add_executable(stm_blinker2 ${SRC})
target_link_libraries(stm_blinker2 hwboardgpio)

# convert elf file to binary file
add_custom_command(TARGET stm_blinker2
                   POST_BUILD
                   COMMAND ${CMAKE_OBJCOPY} -O binary 
                      ${CMAKE_CURRENT_BINARY_DIR}/stm_blinker2
                      ${CMAKE_CURRENT_BINARY_DIR}/stm_blinker2.bin
                   WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
)

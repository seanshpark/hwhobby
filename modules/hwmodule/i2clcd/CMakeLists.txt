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
# target hwmodi2clcd for I2C I/O expander to control LCD1602
#
set(SRC src/i2clcd.c)

add_library(hwmodi2clcd ${SRC})
target_include_directories(hwmodi2clcd PUBLIC include)
target_link_libraries(hwmodi2clcd PRIVATE hwcpcf8574)
target_link_libraries(hwmodi2clcd PRIVATE hwchd44780)

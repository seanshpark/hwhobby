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
# target hwmodled4x7seg for 4 x 7seg LED module using TM1637 chip
#
set(SRC src/lcd1602.c)

add_library(hwmodlcd1602 ${SRC})
target_include_directories(hwmodlcd1602 PUBLIC include)
target_link_libraries(hwmodlcd1602 PUBLIC hwchd44780)
target_link_libraries(hwmodlcd1602 PUBLIC hwmodi2clcd)


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
# target hwctm1637 for HW Chip TM1637
#
set(SRC src/tm1637.c)

add_library(hwctm1637 ${SRC})
target_include_directories(hwctm1637 PUBLIC include)
target_link_libraries(hwctm1637 PUBLIC hwboardgpio)

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
# target hwcpcf8574 for HW Chip PCF8574
#
set(SRC src/pcf8574.c)

add_library(hwcpcf8574 ${SRC})
target_include_directories(hwcpcf8574 PUBLIC include)
target_link_libraries(hwcpcf8574 PUBLIC hwboardgpio)

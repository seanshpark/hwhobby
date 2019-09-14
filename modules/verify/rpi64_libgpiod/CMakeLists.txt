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
# target hello world for libgpiod
#
set(SRC src/hellogpiod.cpp)

add_executable(rpi64hellogpiod ${SRC})
target_link_libraries(rpi64hellogpiod libgpiod)
target_link_libraries(rpi64hellogpiod hwboardgpio)
target_link_libraries(rpi64hellogpiod hwmodlcd1602)

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
# armv7l linux compile options
#

message(STATUS "Building for Cortex-M3 Generic")

set(FLAGS_COMMON "-Wall -mcpu=cortex-m3 -mthumb")

set(FLAGS_CONLY "")
set(FLAGS_CXXONLY "-fno-unwind-tables -fno-exceptions -fno-rtti")

# c flags for debug, release build
# do not define 'DEBUG' for stm32f103
# it'll just eat up the memory and there is no debugging methods
set(CMAKE_C_FLAGS_DEBUG     "-O0")
set(CMAKE_CXX_FLAGS_DEBUG   "-O0")
set(CMAKE_C_FLAGS_RELEASE   "-Os")
set(CMAKE_CXX_FLAGS_RELEASE "-Os")

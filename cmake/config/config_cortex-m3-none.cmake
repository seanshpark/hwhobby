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
# config for cortex-m3, raw embedded
#
include(CMakeForceCompiler)

set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_SYSTEM_PROCESSOR cortex-m3)

set(CMAKE_C_COMPILER   arm-none-eabi-gcc CACHE INTERNAL "CMAKE_C_COMPILER")
set(CMAKE_CXX_COMPILER arm-none-eabi-g++ CACHE INTERNAL "CMAKE_CXX_COMPILER")
set(CMAKE_ASM_COMPILER arm-none-eabi-as CACHE INTERNAL "CMAKE_ASM_COMPILER")
set(CMAKE_AR           arm-none-eabi-ar CACHE INTERNAL "CMAKE_AR")
set(CMAKE_RANLIB       arm-none-eabi-ranlib CACHE INTERNAL "CMAKE_RANLIB")
SET(CMAKE_OBJCOPY      arm-none-eabi-objcopy CACHE INTERNAL "CMAKE_OBJCOPY")

# to pass 'undefined _exit' at compiler check
set(CMAKE_EXE_LINKER_FLAGS "--specs=nosys.specs" CACHE INTERNAL "CMAKE_EXE_LINKER_FLAGS")

# search for programs in the build host directories
SET(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)

# for libraries and headers in the target directories
SET(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
SET(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)

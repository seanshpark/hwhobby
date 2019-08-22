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
# aarch64 linux compile options
#

#
# TODO enable this
#
# check native or cross build
#if(NOT EXISTS "/lib/aarch64-linux-gnu")
#  # ROOTFS_ARM should be defined
#  if(NOT EXISTS $ENV{ROOTFS_ARM})
#    message(FATAL_ERROR "Please set ROOTFS_ARM environment to target ARM ROOTFS to build")
#  endif()
#endif()

message("-- Building for aarch64 Linux")

# set linux common flags
include("cmake/option/option_linux-common.cmake")

# addition for arm-linux
set(FLAGS_COMMON ${FLAGS_COMMON}
                 "-march=armv8-a"
)

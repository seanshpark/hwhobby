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
# config for aarch64-linux
#
include(CMakeForceCompiler)

set(CMAKE_SYSTEM_NAME Linux)
set(CMAKE_SYSTEM_PROCESSOR aarch64)

SET(CMAKE_C_COMPILER   aarch64-linux-gnu-gcc)
SET(CMAKE_CXX_COMPILER aarch64-linux-gnu-g++)

#
# TODO enable this
#
# where is the target environment
#if(NOT ROOTFS_ARM)
#  if(DEFINED ENV{ROOTFS_ARM})
#    set(ROOTFS_ARM $ENV{ROOTFS_ARM})
#  endif()
#endif()
#message(STATUS "ROOTFS_ARM = ${ROOTFS_ARM}")
#
#set(CMAKE_FIND_ROOT_PATH ${ROOTFS_ARM})
#set(CMAKE_SHARED_LINKER_FLAGS
#    "${CMAKE_SHARED_LINKER_FLAGS} --sysroot=${ROOTFS_ARM}"
#    CACHE INTERNAL "" FORCE)
#set(CMAKE_EXE_LINKER_FLAGS
#    "${CMAKE_EXE_LINKER_FLAGS} --sysroot=${ROOTFS_ARM}"
#    CACHE INTERNAL "" FORCE)

# search for programs in the build host directories
SET(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)

# for libraries and headers in the target directories
SET(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
SET(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)

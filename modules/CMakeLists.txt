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
# include sub directoies in orders of dependency
#
add_subdirectory(hwabc) # H/W abc (too lazy to give a better name)
add_subdirectory(hwboard)
add_subdirectory(hwchip)
add_subdirectory(hwmodule)

add_subdirectory(lib)
add_subdirectory(muvmodule)

add_subdirectory(tools)
add_subdirectory(verify)

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

set(STMSRC
      it/stm32f10x_it.c
      stmsrc/stm32f10x_adc.c
      stmsrc/stm32f10x_bkp.c
      stmsrc/stm32f10x_can.c
      stmsrc/stm32f10x_dma.c
      stmsrc/stm32f10x_exti.c
      stmsrc/stm32f10x_flash.c
      stmsrc/stm32f10x_gpio.c
      stmsrc/stm32f10x_i2c.c
      stmsrc/stm32f10x_iwdg.c
      stmsrc/stm32f10x_lib.c
      stmsrc/stm32f10x_nvic.c
      stmsrc/stm32f10x_pwr.c
      stmsrc/stm32f10x_rcc.c
      stmsrc/stm32f10x_rtc.c
      stmsrc/stm32f10x_spi.c
      stmsrc/stm32f10x_systick.c
      stmsrc/stm32f10x_tim1.c
      stmsrc/stm32f10x_tim.c
      stmsrc/stm32f10x_usart.c
      stmsrc/stm32f10x_vector.c
      stmsrc/stm32f10x_wwdg.c
      stmsrc/cortexm3_macro.s
)

set_property(SOURCE stmsrc/cortexm3_macro.s PROPERTY LANGUAGE C)

add_library(hwbstm32f103_ ${STMSRC})
target_include_directories(hwbstm32f103_ PUBLIC conf)
target_include_directories(hwbstm32f103_ PUBLIC include)

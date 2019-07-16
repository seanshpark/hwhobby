# stm_counter

Four 7 segment LED module with STM32F103 board

## Connection

```
STM         LED module
=======================
C14   ----  Clock
C13   ----  DIO
+3.3  ----  VCC
GND   ----  GND
```
`VCC` of `LED` can be connected to `USB-to-Serial` `3.3V` if it has one.
Connecting to `STM` board may heat up the regulator on the board.

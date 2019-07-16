# hwhobby

HW related small programs for my hobby
- Build on Ubuntu x86-64 Linux with CMake and GNU cross compilers

## Target boards as controllers

Support target boards.
- Raspberry Pi
- STM32F103C8T6

Maybe support someday
- Arduino Pro mini
- Raspberry Pi 2B / 3B / 3B+
- 2051

## Modules to play with

Support modules
- Onboard LED blink
- Four 7-segment LED display module on TM1637 chip

Maybe support someday
- LCD 1602 with I2C on HD44780 chip
- OLED 128x64 B/W with I2C on SSD1306 chip

## Source tree

Look inside `modules/tools` folder to begin.

## Compile and run

### STM32F103

Compile in x86-64 host
```
make stm
```

Flash with USB-to-Serial on `/dev/ttyUSB0`
```
# Connect BOOT0 jumper to 1
# Press reset button once

make stmflash

# Connect BOOT0 jumper to 0
# Press reset button once again
```

`stm32flash` will show like this when finished
```
Using Parser : Raw BINARY
Interface serial_posix: 9600 8E1
Version      : 0x22
Option 1     : 0x00
Option 2     : 0x00
Device ID    : 0x0410 (STM32F10xxx Medium-density)
- RAM        : 20KiB  (512b reserved by bootloader)
- Flash      : 128KiB (size first sector: 4x1024)
- Option RAM : 16b
- System RAM : 2KiB
Write to memory
Erasing memory
Wrote address 0x08002340 (100.00%) Done.
```

### stm32flash

Use this tool to flash binary image to STM32 board

Refer https://sourceforge.net/p/stm32flash/wiki/Home/

### Serial connection with STM32 board

```
Serial     STM32F103
====================
TXD  ----  A10
RXD  ----  A9
GND  ----  GND
+5V  ----  5V
```

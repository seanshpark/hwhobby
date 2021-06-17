# rpi_counter

Four 7 segment LED module on Raspberry Pi 1

## Connection

```
RPi         LED
=================
GPIO23 ---- Clock
GPIO24 ---- DIO
+3.3   ---- VCC
GND    ---- GND
```

## Compile and run

Compile in x86-64 host
```
make rpi
```

Run in RPi
```
cd path_to_led47clock
sudo ./led47clock
```

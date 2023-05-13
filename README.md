# HeidelbergInterface
[![.github/workflows/arduino-lint.yaml](https://github.com/Fbisinger/HeidelbergInterface/actions/workflows/arduino-lint.yaml/badge.svg)](https://github.com/Fbisinger/HeidelbergInterface/actions/workflows/arduino-lint.yaml) [![Compile Examples](https://github.com/Fbisinger/HeidelbergInterface/actions/workflows/compile-examples.yaml/badge.svg)](https://github.com/Fbisinger/HeidelbergInterface/actions/workflows/compile-examples.yaml) [![Github All Releases](https://img.shields.io/github/downloads/Fbisinger/HeidelbergInterface/total.svg)]()

HeidelbergInterface is a universal Arduino library for communication with the Heidelberg Wallbox Energy Control via RS485/Modbus-RTU. It is designed to work with eps8266/esp32. Additional hardware for the RS485 communication is required, e.g. MAX485 IC (cheap Dev-Boards availlable).

## Notice
All functions rely on Modbus register layout versions >= V1.0.8 if you are using older versions some function, e.g. remote lock will not work properly! You can retrieve your reg layout version with `getRegLayoutVersion()`.

:de: © F.Bisinger 2023
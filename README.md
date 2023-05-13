# HeidelbergInterface
[![.github/workflows/arduino-lint.yaml](https://github.com/Fbisinger/HeidelbergInterface/actions/workflows/arduino-lint.yaml/badge.svg)](https://github.com/Fbisinger/HeidelbergInterface/actions/workflows/arduino-lint.yaml) [![Compile Examples](https://github.com/Fbisinger/HeidelbergInterface/actions/workflows/compile-examples.yaml/badge.svg)](https://github.com/Fbisinger/HeidelbergInterface/actions/workflows/compile-examples.yaml) [![Downloads All Releases](https://img.shields.io/github/downloads/Fbisinger/HeidelbergInterface/total.svg)]()
[![Current Release Date](https://img.shields.io/github/release-date/Fbisinger/HeidelbergInterface)]() [![Current Release](https://img.shields.io/github/v/release/Fbisinger/HeidelbergInterface)]() [![License](https://img.shields.io/github/license/Fbisinger/HeidelbergInterface)]()

HeidelbergInterface is a universal Arduino library for communication with the Heidelberg Wallbox Energy Control via RS485/Modbus-RTU. It is designed to work with eps8266/esp32. Additional hardware for the RS485 communication is required, e.g. MAX485 IC (cheap Dev-Boards availlable).

## Minimal example
```c
// HeidelbergInterface
// MIT License Copyright (c) 2023 F. Bisinger

#include <HeidelbergInterface.h>
#include <SoftwareSerial.h>

#define MODBUS_RATE_HEIDELBERG 19200  // Baud Rate
#define MAX485_DE 33                  // GPIO33, DE pin on the TTL to RS485 converter
#define MAX485_RX 34                  // GPIO34, RO pin on the TTL to RS485 converter
#define MAX485_TX 17                  // GPIO17, DI pin on the TTL to RS485 converter
#define SLAVE_ID_HEIDELBERG 1            // Configured slave ID of Heidelberg Wallbox

// Serial communication to the RS485 converter via SoftwareSerial
// Harware Serial will work fine as well
SoftwareSerial *serial;
HeidelbergInterface interface;

void setup()
{
  Serial.begin(115200);
  serial = new SoftwareSerial(MAX485_RX, MAX485_TX, false); //RX, TX
  // Wallbox Heidelberg Energy Control uses 19.200 bit/sec, 
  // 8 data bit, 1 parity bit (even), 1 stop bit
  serial->begin(MODBUS_RATE_HEIDELBERG, SWSERIAL_8E1);
  interface.begin(*serial, SLAVE_ID_HEIDELBERG, MAX485_DE); 
}

void loop()
{
  // must be called to keep the modbus data buffer updated 
  interface.mbloop();
  
  Serial.print("Heidelberg Wallbox Modbus Register Layout-Version: ");
  Serial.println(interface.getRegLayoutVersion(), HEX);
  /*
  FOR ALL AVAILABLE FUNCTIONS, SEE src/HeidelbergInterface.h
  */

  delay(5000);
}
```

## Dependencies
- [EspSoftwareSerial](https://github.com/plerup/espsoftwareserial)
- [ModbusMaster](https://github.com/4-20ma/ModbusMaster)
## Notice
All functions rely on Modbus register layout versions >= V1.0.8 if you are using older versions some function, e.g. remote lock will not work properly! You can retrieve your reg layout version with `getRegLayoutVersion()`.

:de: © F.Bisinger 2023
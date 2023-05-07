// HeidelbergInterface
// MIT License Copyright (c) 2023 F. Bisinger

// Example sketch of HeidelbergInterface Library

#include <HeidelbergInterface.h>
#include <SoftwareSerial.h>

#define MODBUS_RATE_HEIDELBERG 19200  // Baud Rate
#define MAX485_DE 33                  // GPIO33, DE pin on the TTL to RS485 converter
#define MAX485_RX 34                  // GPIO34, RO pin on the TTL to RS485 converter
#define MAX485_TX 17                  // GPIO17, DI pin on the TTL to RS485 converter
#define SLAVE_ID_HEIDELBERG 1            // Configured slave ID of Growatt Inverter

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
  interface.mbloop();

  Serial.print("Modbus Register Layout Version: ");
  Serial.println(interface.getRegLayoutVersion(), HEX);

  Serial.print("Current Time | Last Modbus Update: ");
  Serial.print(millis());
  Serial.print(" | ");
  Serial.println(interface.getLastUpdateTime());

  delay(1000);
}

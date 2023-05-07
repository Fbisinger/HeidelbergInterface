// HeidelbergInterface
// MIT License Copyright (c) 2023 F. Bisinger

// Example sketch of HeidelbergInterface Library

#include <HeidelbergInterface.h>
#include <SoftwareSerial.h>

#define MODBUS_RATE_HEIDELBERG 9600  // Baud Rate
#define MAX485_DE 33              // GPIO33, DE pin on the TTL to RS485 converter
#define MAX485_RX 34              // GPIO34, RO pin on the TTL to RS485 converter
#define MAX485_TX 17              // GPIO17, DI pin on the TTL to RS485 converter
#define SLAVE_ID_GROWATT 3        // Configured slave ID of Growatt Inverter

// Serial communication to the RS485 converter via SoftwareSerial
// Harware Serial will work fine as well
SoftwareSerial *serial;

HeidelbergInterface interface;

void setup()
{
  serial = new SoftwareSerial(MAX485_RX, MAX485_TX, false); //RX, TX
  serial->begin(MODBUS_RATE_HEIDELBERG);
  interface.begin(*serial, SLAVE_ID_GROWATT, MAX485_DE);

  Serial.println(interface.getRegLayoutVersion(), HEX);
}

void loop()
{
  interface.updateBuffer();
}
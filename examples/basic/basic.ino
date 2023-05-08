// HeidelbergInterface
// MIT License Copyright (c) 2023 F. Bisinger

// Example sketch of HeidelbergInterface Library

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

int counter = 0;
bool toggle_1 = true;
bool toggle_2 = true;

void setup()
{
  Serial.begin(115200);

  serial = new SoftwareSerial(MAX485_RX, MAX485_TX, false); //RX, TX
  // Wallbox Heidelberg Energy Control uses 19.200 bit/sec, 
  // 8 data bit, 1 parity bit (even), 1 stop bit
  serial->begin(MODBUS_RATE_HEIDELBERG, SWSERIAL_8E1);
  interface.begin(*serial, SLAVE_ID_HEIDELBERG, MAX485_DE);

  delay(2000);
  bool error = interface.setMaxCurr(16);
  Serial.println(error);
  
}

void loop()
{
  interface.mbloop();

  Serial.print("Modbus Register Layout Version: ");
  Serial.println(interface.getRegLayoutVersion(), HEX);

  Serial.print("Current Time | Last Modbus Update | Diff: ");
  Serial.print(millis());
  Serial.print(" | ");
  Serial.print(interface.getLastUpdateTime());
  Serial.print(" | ");
  Serial.println(millis() - interface.getLastUpdateTime());

  Serial.print("Current Power: ");
  Serial.println(interface.getPower());

  Serial.print("Energy Since Installation: ");
  Serial.println(interface.getEnergyInst());
  Serial.print("Energy Since PowerOn: ");
  Serial.println(interface.getEnergyPwOn());

  Serial.print("L1 Current: ");
  Serial.println(interface.getCurrL1());
  Serial.print("L2 Current: ");
  Serial.println(interface.getCurrL2());
  Serial.print("L3 Current: ");
  Serial.println(interface.getCurrL3());

  Serial.println("Charging State (2=A1 3=A2 4=B1 5=B2 6=C1 7=C2 8=derating 9=E 10=F 11=ERR): " + String(interface.getChgState()));
  Serial.println("PCB-Temp: " + String(interface.getPcbTemp()));
  Serial.println("Voltage (L1,L2,L3): " + String(interface.getVoltageL1()) + " " + String(interface.getVoltageL2()) + " " + String(interface.getVoltageL3()));
  Serial.println("External Lock: " + String(interface.getExtLockState()));
  Serial.println("WD-Timeout (ms): " + String(interface.getWatchDogTimeout()));
  Serial.println("MAX-Current: " + String(interface.getMaxCurr()));
  Serial.println("FS-Current: " + String(interface.getFsCurr()));

  if((millis() >= 50000) && (toggle_1 == true)){
    Serial.println("Stop Charging in 10s");
    delay(10000);
    interface.setMaxCurr(0);
    toggle_1 = false;
    }

  if((millis() >= 100000) && (toggle_2 == true)){
    Serial.println("Setting to 6A in 20s");
    delay(20000);
    interface.setMaxCurr(6);
    toggle_2 = false;
    }

  
  


  delay(1000);
}

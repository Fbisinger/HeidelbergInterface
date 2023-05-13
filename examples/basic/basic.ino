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

// For demo wallbox charge control
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
  
  // Prints current wallbox paramters, see functon definition below. 
  // For detailed library capabilities have a look at src/HeidelbergInterface.h
  printStatus();

  // Disable energy flow after 60s
  if((millis() >= 60000) && (toggle_1 == true)){
    Serial.println("Stopping to charge in 10s ...");
    delay(10000);
    if(interface.setMaxCurr(0)){
      toggle_1 = false;
    }
    
  }

  // Set charging current to 6A after 120s
  if((millis() >= 120000) && (toggle_2 == true)){
    Serial.println("Setting to 6A charging current in 10s ...");
    delay(10000);
    if(interface.setMaxCurr(6)){
      toggle_2 = false;
    }
    
  }

  delay(1000);
}



void printStatus() {
  Serial.print("Modbus Register Layout Version: ");
  Serial.println(interface.getRegLayoutVersion(), HEX);

  Serial.print("Current Time | Last Modbus Update | Diff: ");
  Serial.print(millis());
  Serial.print(" | ");
  Serial.print(interface.getLastUpdateTime());
  Serial.print(" | ");
  Serial.println(millis() - interface.getLastUpdateTime());

  Serial.print("Charging State (2=A1 3=A2 4=B1 5=B2 6=C1 7=C2 8=derating 9=E 10=F 11=ERR): ");
  Serial.println(interface.getChgState());

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
  /*
  MORE FUNCTIONS AVAILABLE!, SEE src/HeidelbergInterface.h
  */
}
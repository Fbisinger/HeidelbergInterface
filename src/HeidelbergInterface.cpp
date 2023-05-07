// HeidelbergInterface
// MIT License Copyright (c) 2023 F. Bisinger

// For detailed Modbus register layout information referr to:
// https://www.amperfied.de/wp-content/uploads/2022/06/ModBus-Register-Tabelle.pdf

#include "Arduino.h"
#include "HeidelbergInterface.h"
#include "ModbusMaster.h"

ModbusMaster modbus;

HeidelbergInterface::HeidelbergInterface()
{
  //Default Values
  _commAllowed = true;
  _updateRate = 60000;
  _lastMsg = millis();

}

void HeidelbergInterface::begin(SoftwareSerial serial, int slaveid, int pinDE)
{
  _pinDE = pinDE;
  modbus.begin(slaveid, *serial);
  modbus.preTransmission(preTransmission);
  modbus.postTransmission(postTransmission);
}

// Enable Modbus communication, only necessary if communication was disabled beforehand
void HeidelbergInterface::enableCom()
{
  _commAllowed = true;
}
// Disable Modbus communication, 
// only necessary if several slaves with different comm speeds on the same bus
// Function will block all comunication of the HeidelbergInterface client
void HeidelbergInterface::disableCom()
{
  _commAllowed = false;
}

// Set Modbus data buffer refresh rate in milli seconds (updateRate must be >= 5000)
bool HeidelbergInterface::setUpdateRate(int timespan)
{
  if (timespan >= 5000){
    _updateRate = timespan;
    return true;
  }
  return false;  
}

// Reads Modbus registers and updates buffer with a predefined updateRate
void HeidelbergInterface::mbloop()
{
  if ((millis() > _lastMsg + _updateRate) && _commAllowed){
    //do mb stuff
    if(/*sucess*/){
      _lastMsg = millis();
    }

    //Otherwise retry in next loop iteration
  }
}

// Return system time of last succesfull modbus buffer update
uint32_t HeidelbergInterface::getLastUpdateTime()
{
  return _lastMsg;
}

// Modbus Register Layout Version hexadecimal (0x100 -> V1.0.0)
int HeidelbergInterface::getRegLayoutVersion()
{
  return _modbusdataheidelberg.register_layout;
}
// Wallbox charging state (2=A1, 3=A2, 4=B1, 5=B2, 6=C1,7=C2, 8=derating, 9=E, 10=F, 11=ERR)
int HeidelbergInterface::getChgState()
{
  return _modbusdataheidelberg.charging_state;
}

// L1 - Current RMS in Arms
float HeidelbergInterface::getCurrL1()
{
  return _modbusdataheidelberg.currentL1;
}

// L2 - Current RMS in Arms
float HeidelbergInterface::getCurrL2()
{
  return _modbusdataheidelberg.currentL2;
}

// L3 - Current RMS in Arms
float HeidelbergInterface::getCurrL3()
{
  return _modbusdataheidelberg.currentL3;
}

// PCB-Temperatur in °C
float HeidelbergInterface::getPcbTemp()
{
  return _modbusdataheidelberg.pcb_temp;
}

// Voltage L1 - N rms in Volt
int HeidelbergInterface::getVoltageL1()
{
  return _modbusdataheidelberg.voltageL1;
}

// Voltage L2 - N rms in Volt
int HeidelbergInterface::getVoltageL2()
{
  return _modbusdataheidelberg.voltageL2;
}

// Voltage L3 - N rms in Volt
int HeidelbergInterface::getVoltageL3()
{
  return _modbusdataheidelberg.voltageL3;
}

// extern lock state (false = locked / true = unlocked)
bool HeidelbergInterface::getExtLockState()
{
  return _modbusdataheidelberg.extern_lock;
}

// Power (L1+L2+L3) in VA
int HeidelbergInterface::getPower()
{
  return _modbusdataheidelberg.power;
}

// Energy since PowerOn in VAh
int HeidelbergInterface::getEnergyPwOn()
{
  return _modbusdataheidelberg.energy_power_on;
}

// Energy since Installation in VAh
int HeidelbergInterface::getEnergyInst()
{
  return _modbusdataheidelberg.energy_since_installation;
}

// Hardware configuration maximal current in A
int HeidelbergInterface::getHwMaxCurr()
{
  return _modbusdataheidelberg.hw_max_current;
}

// Hardware configuration minimal current in A
int HeidelbergInterface::getHwMinCurr()
{
  return _modbusdataheidelberg.hw_min_current;
}

// ModBus-CLient WatchDog Timeout in milli seconds (0=Off)
int HeidelbergInterface::getWatchDogTimeout()
{
  return _modbusdataheidelberg.watchdog;
}

// Set ModBus-Master WatchDog Timeout in ms
int HeidelbergInterface::setWatchdogTimeout(int timeout)
{
  return 0;
}

// Software configured maximal current in A [0; 6 to 16]
int HeidelbergInterface::getMaxCurr()
{
  return _modbusdataheidelberg.max_current;
}

// Set software configured maximal current in A [0; 6 to 16]
int HeidelbergInterface::setMaxCurr(int current)
{
  return 0;
}

// FailSafe Current configuration (in case loss of Modbus communication) in A [0; 6 to 16]
int HeidelbergInterface::getFsCurr()
{
  return _modbusdataheidelberg.fs_current;
}

// Set FailSafe Current configuration (in case loss of Modbus communication) in A [0; 6 to 16]
int HeidelbergInterface::setFsCurr(int current)
{
  return 0;
}






void HeidelbergInterface::preTransmission()
{
  digitalWrite(_pinDE, 1);
}

void HeidelbergInterface::postTransmission()
{
  digitalWrite(_pinDE, 0);
}

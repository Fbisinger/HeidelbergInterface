// HeidelbergInterface
// MIT License Copyright (c) 2023 F. Bisinger

// For detailed Modbus register layout information referr to:
// https://www.amperfied.de/wp-content/uploads/2022/06/ModBus-Register-Tabelle.pdf

#include "Arduino.h"
#include "HeidelbergInterface.h"
#include "ModbusMaster.h"

HeidelbergInterface::HeidelbergInterface()
{
  //Default Values
  _commAllowed = true;
  _updateRate = 30000;
  _lastMsg = 0;
  _serialDebug = &Serial;

}

void HeidelbergInterface::begin(Stream &serial, int slaveid, int pinDE)
{
  _pinDE = pinDE;
  pinMode(_pinDE, OUTPUT);
  _modbus.begin(slaveid, serial);

  static HeidelbergInterface* obj = this;
  _modbus.preTransmission([]() { obj->preTransmission();}); // external callback
  _modbus.postTransmission([]() { obj->postTransmission();});
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
    _modbusBuffer = _modbus.readInputRegisters(4, 15);
    
    delay(1000);

    if (_modbusBuffer == _modbus.ku8MBSuccess)   {
      _modbusdataheidelberg.register_layout = _modbus.getResponseBuffer(0);
      _modbusdataheidelberg.charging_state = _modbus.getResponseBuffer(1);
      _modbusdataheidelberg.currentL1 = _modbus.getResponseBuffer(2) * 0.1;
      _modbusdataheidelberg.currentL2 = _modbus.getResponseBuffer(3) * 0.1;
      _modbusdataheidelberg.currentL3 = _modbus.getResponseBuffer(4) * 0.1;
      _modbusdataheidelberg.pcb_temp = _modbus.getResponseBuffer(5) / 10;
      _modbusdataheidelberg.voltageL1 = _modbus.getResponseBuffer(6);
      _modbusdataheidelberg.voltageL2 = _modbus.getResponseBuffer(7);
      _modbusdataheidelberg.voltageL3 = _modbus.getResponseBuffer(8);
      _modbusdataheidelberg.extern_lock = _modbus.getResponseBuffer(9);
      _modbusdataheidelberg.power = _modbus.getResponseBuffer(10);
      _modbusdataheidelberg.energy_power_on = _modbus.getResponseBuffer(11) << 16 | _modbus.getResponseBuffer(12); // combined from two separate registers
      _modbusdataheidelberg.energy_since_installation = _modbus.getResponseBuffer(13) << 16 | _modbus.getResponseBuffer(14); // combined from two separate registers
      _serialDebug->print("[DEBUG] Energy pwron High byte, Low byte");
      _serialDebug->print(_modbus.getResponseBuffer(11));
      _serialDebug->print(",");
      _serialDebug->println(_modbus.getResponseBuffer(12));
      _serialDebug->print("[DEBUG] Energy inst High byte, Low byte");
      _serialDebug->print(_modbus.getResponseBuffer(13));
      _serialDebug->print(",");
      _serialDebug->println(_modbus.getResponseBuffer(14));
      _serialDebug->println("[DEBUG] Buffer update sucessful");
      // Update message time
      _lastMsg = millis();
    }

    else{
        _serialDebug->print("[ERROR] Failed reading Input registers, result code: ");
        _serialDebug->print(_modbusBuffer);
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
uint32_t HeidelbergInterface::getEnergyPwOn()
{
  return _modbusdataheidelberg.energy_power_on;
}

// Energy since Installation in VAh
uint32_t HeidelbergInterface::getEnergyInst()
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
bool HeidelbergInterface::setWatchdogTimeout(int timeout)
{
  return 0;
}

// Software configured maximal current in A [0; 6 to 16]
int HeidelbergInterface::getMaxCurr()
{
  return _modbusdataheidelberg.max_current;
}

// Set software configured maximal current in A [0; 6 to 16]
bool HeidelbergInterface::setMaxCurr(int current)
{
  if (current == 0 || (current >= 6 && current <= 16)){
    _modbus.setTransmitBuffer(0, current*10);
    uint8_t _write_response = _modbus.writeMultipleRegisters(261, 1);
    if (_write_response == _modbus.ku8MBSuccess) {
      return true;
    }
  }
  return false;
}

// FailSafe Current configuration (in case loss of Modbus communication) in A [0; 6 to 16]
int HeidelbergInterface::getFsCurr()
{
  return _modbusdataheidelberg.fs_current;
}

// Set FailSafe Current configuration (in case loss of Modbus communication) in A [0; 6 to 16]
bool HeidelbergInterface::setFsCurr(int current)
{
  if (current == 0 || (current >= 6 && current <= 16)){
    _modbus.setTransmitBuffer(0, current*10);
    uint8_t _write_response = _modbus.writeMultipleRegisters(262, 1);
    if (_write_response == _modbus.ku8MBSuccess) {
      return true;
    }
  }
  return false;
}

void HeidelbergInterface::preTransmission()
{
  digitalWrite(_pinDE, 1);
  _serialDebug->println("[DEBUG] pretransmission");
}

void HeidelbergInterface::postTransmission()
{
  digitalWrite(_pinDE, 0);
  _serialDebug->println("[DEBUG] posttransmission");
}

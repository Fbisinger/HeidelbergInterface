// HeidelbergInterface
// MIT License Copyright (c) 2023 F. Bisinger

#ifndef HeidelbergInterface_h
#define HeidelbergInterface_h

#include "Arduino.h"
#include "ModbusMaster.h"

class HeidelbergInterface
{
  public:
    HeidelbergInterface();
    void begin(Stream &serial, int id, int pinDE);
    void enableCom();
    void disableCom();
    bool setUpdateRate(int timespan);
    void mbloop();
    uint32_t getLastUpdateTime();


    int getRegLayoutVersion();
    int getChgState();
    
    float getCurrL1();
    float getCurrL2();
    float getCurrL3();
    
    float getPcbTemp();
    
    int getVoltageL1();
    int getVoltageL2();
    int getVoltageL3();
    
    bool getExtLockState();

    int getPower();
    int getEnergyPwOn();
    int getEnergyInst();
    int getHwMaxCurr();
    int getHwMinCurr();


    int getWatchDogTimeout();
    bool setWatchdogTimeout(int timeout);
    int getMaxCurr();
    bool setMaxCurr(int current);
    int getFsCurr();
    bool setFsCurr(int current);

  private:
    void preTransmission();
    void postTransmission();
    int _slaveID;
    int _mbSpeed;
    int _mbTimeout;
    int _pinDE;
    Stream *_serialDebug;
    bool _commAllowed;
    int _updateRate;
    uint32_t _lastMsg;
    uint8_t _modbusBuffer;
    ModbusMaster _modbus;

    struct modbus_registers_heidelberg
    {
      int register_layout;
      int charging_state;
      float pcb_temp; 
      int voltageL1, voltageL2, voltageL3;
      float currentL1, currentL2, currentL3;
      bool extern_lock;
      int power;
      int energy_power_on, energy_since_installation;
      
      int hw_max_current, hw_min_current;
      int watchdog;
      int max_current, fs_current;
    };
    struct modbus_registers_heidelberg _modbusdataheidelberg;   
};

#endif
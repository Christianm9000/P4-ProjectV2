#ifndef TASK_ORCHESTRATOR_H
#define TASK_ORCHESTRATOR_H

#include <sensor_manager.h>
#include <lorawan_manager.h>
#include <data_manager.h>

#include <Arduino.h>
#include <ArduinoLowPower.h>

class Orchestrator
{
private:
    SensorModule Sensors;
    LoRaWAN LoRa;
    DataManager NVStorage;

public:
    int get_SoC();
    int sleep();
    int make_measurements();
    int handle_uplink();
    void begin();
    void continue1();
};

#endif
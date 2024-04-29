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
    uint8_t tempPin;
    uint16_t minutes;

    // Creating an instance of the other classes (DataManager, SensorManager, LoraWanManager)
    SensorModule sensor;
    LoRaWAN loRa;
    DataManager NVstorage;

public:
    int get_SoC();
    int sleep(uint16_t minutes);
    void make_measurements(uint8_t tempPin, int moistPin, int dryValue, int wetValue);
    int handle_uplink();
    void begin();
    void continue1();
};

#endif
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
    uint16_t minutes;
    uint8_t command;

    // Creating an instance of the other classes (DataManager, SensorManager, LoraWanManager)
    SensorModule Sensor;
    LoRaWAN LoRa;
    DataManager dm;

public:
    Orchestrator();

    int get_SoC();
    int sleep(uint16_t minutes);

    void make_measurements();

    uint8_t handle_uplink(uint8_t* data, uint8_t size);
    uint8_t handle_downlink();

};

#endif
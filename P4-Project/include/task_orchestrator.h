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

    uint16_t measurement_power_req = 5; // Capacity in mv needed to complete command 0 - make measurement.
    uint16_t transmit_power_req = 10; // Capacity in mv needed to complete command 1 - make measurement and transmit.
    bool has_data_measurements = false;
    bool SWC_received = false;

    // Creating an instance of the other classes (DataManager, SensorManager, LoraWanManager)
    SensorModule Sensor;
    LoRaWAN LoRa;
    DataManager dm;

public:
    Orchestrator();

    int get_SoC();
    int sleep(uint16_t minutes);

    void make_measurements(bool dummy);

    uint8_t handle_uplink(uint8_t* data, uint8_t size);
    uint8_t handle_downlink();

};

#endif
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

public:
    int get_SoC();
    int sleep(uint16_t minutes);
    std::pair<int, float> make_measurements(uint8_t tempPin);
    int handle_uplink();
    void begin();
    void continue1();
};

#endif
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
    int tempPin;
    unsigned int seconds;

public:
    int get_SoC();
    int sleep(unsigned int seconds);
    int make_measurements(int tempPin);
    int handle_uplink();
    void begin();
    void continue1();
};

#endif
#include "task_orchestrator.h"

/*

*/

void Orchestrator::begin()
{
    //
    // int SoC_value = get_SoC();
}

std::pair<int, float> Orchestrator::make_measurements(uint8_t tempPin)
{
    SensorModule sensorModule(A1, 500, 200, tempPin); 

    float temperature = sensorModule.getTemperature();
    int moisturePercentage = sensorModule.getMoisture();

    return std::make_pair(moisturePercentage, temperature);
}

int Orchestrator::sleep(uint16_t minutes)
{
    unsigned long sleep_time = minutes;
    LowPower.deepSleep(sleep_time);
    return sleep_time;
}

int Orchestrator::get_SoC()
{
    pinMode(14, INPUT);
    return digitalRead(14);
}

// int Orchestrator::handle_uplink()
// {
// }

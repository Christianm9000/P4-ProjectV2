#include "task_orchestrator.h"

/*

*/

void Orchestrator::begin()
{
    //
    int SoC_value = get_SoC();
}

int Orchestrator::make_measurements(uint8_t tempPin)
{
    SensorModule sensorModule(A1, 500, 200, tempPin); // A1: Moisture sensor pin, 500: Dry reading, 200: Wet reading, 17: DS18B20 digital data pin

    float temperature = sensorModule.getTemperature();
    int moisturePercentage = sensorModule.getMoisture();

    return (moisturePercentage, temperature);
}

int Orchestrator::sleep(uint16_t minutes)
{
    unsigned long sleep_time = minutes;
    LowPower.deepSleep(sleep_time);
}
int Orchestrator::get_SoC()
{
    pinMode(14, INPUT);
    return digitalRead(14);
}

int Orchestrator::handle_uplink()
{
}

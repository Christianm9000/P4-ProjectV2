#include "task_orchestrator.h"

/*

*/

void Orchestrator::begin()
{
    //
    int SoC_value = get_SoC();
}

int make_measurement()
{
    SensorModule sensorModule(A1, 500, 200, 17); // A1: Moisture sensor pin, 500: Dry reading, 200: Wet reading, 17: DS18B20 digital data pin

    float temperature = sensorModule.getTemperature();
    int moisturePercentage = sensorModule.getMoisture();

    return (moisturePercentage, temperature);
}

int sleep(unsigned int seconds)
{
    unsigned long sleep_time = seconds * 1000;
    LowPower.deepSleep(sleep_time);
}
int get_SoC()
{
    pinMode(14, INPUT);
    return digitalRead(14);
}
int handle_uplink()
{
}
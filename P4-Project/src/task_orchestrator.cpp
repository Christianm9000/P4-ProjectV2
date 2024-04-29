#include "task_orchestrator.h"
#include "data_manager.h"

void Orchestrator::begin()
{
    //
    // int SoC_value = get_SoC();
}

void Orchestrator::make_measurements(uint8_t tempPin, int moistPin, int dryValue, int wetValue)
{
    SensorModule sensor(moistPin, dryValue, wetValue, tempPin);

    float temperature = sensor.getTemperature();
    int moisturePercentage = sensor.getMoisture();

    NVstorage.append_data(moisturePercentage, temperature); // Call append_data
}
int Orchestrator::sleep(uint16_t minutes)
{
    // Converting from minutes to millisecond since LowPower.deepsleep() takes millis as argument
    unsigned long sleep_time = minutes * 60000; // unsigned long --> Size (4 bytes) - Range 0 to 4,294,967,295
    LowPower.deepSleep(sleep_time);
    return sleep_time;
}

int Orchestrator::get_SoC()
{
    pinMode(14, INPUT);
    return digitalRead(14);
}

int Orchestrator::handle_uplink()
{
}

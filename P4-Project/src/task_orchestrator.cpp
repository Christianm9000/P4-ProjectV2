#include "task_orchestrator.h"
#include "data_manager.h"

/*

*/

void Orchestrator::begin()
{
    //
    // int SoC_value = get_SoC();
}

void Orchestrator::make_measurements(uint8_t tempPin, int moistPin, int dryValue, int wetValue)
{
    SensorModule sensorModule(moistPin, dryValue, wetValue, tempPin); 

    float temperature = sensorModule.getTemperature();
    int moisturePercentage = sensorModule.getMoisture();

    DataManager dataManager; // Create an instance of DataManager
    dataManager.append_data(moisturePercentage, temperature); // Call append_data
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

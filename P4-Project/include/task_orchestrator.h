#ifndef TASK_ORCHESTRATOR_H
#define TASK_ORCHESTRATOR_H

#include <sensor_manager.h>
#include <lorawan_manager.h>
#include <data_manager.h>

#include <Arduino.h>
#include <ArduinoLowPower.h>

#include <SPI.h>
#include <Mcp320x.h>

class Orchestrator
{
private:
    uint16_t minutes;
    uint8_t command;

    uint16_t measurement_power_req = 5; // Capacity in mv needed to complete command 0 - make measurement.
    uint16_t transmit_power_req = 10; // Capacity in mv needed to complete command 1 - make measurement and transmit.
    bool has_data_measurements = false;
    bool SWC_received = false;

    uint8_t SPI_CS = 4;     // SPI chip select
    uint8_t SOC_ENABLE_PIN = 3; // mosfet control pin
    uint16_t ADC_VREF = 5000;   // 5000mV on V_REF pin of MCP3201
    int ADC_CLK = 1600000;      // SPI clock 1.6MHz

    MCP3201 adc;

    // Creating an instance of the other classes (DataManager, SensorManager, LoraWanManager)
    SensorModule Sensor;
    LoRaWAN LoRa;
    DataManager dm;

public:
    Orchestrator();

    uint8_t setup_SoC();

    // turns on the MOSFET, which controls the flow of the supercap.
    // Remember to turn off when done in order to save power.
    uint8_t enable_SoC();

    // turns off the MOSFET, which controls the flow of the supercap.
    uint8_t disable_SoC();

    // uses the SPI interface to get a reading from the MCP3201.
    // Converts it to a mV value of 0mV to 5000mV
    uint16_t get_SoC();

    int sleep(uint16_t minutes);

    void make_measurements();

    uint8_t handle_uplink(uint8_t* data, uint8_t size);
    uint8_t handle_downlink();

};

#endif
#include "task_orchestrator.h"

Orchestrator::Orchestrator() 
    : Sensor(), // Initialize Sensor Manager
      LoRa(),   // Initialize LoRa Manager
      dm() // Initialize Data Manager
{  
    // This will run when the IoT device has lost power and resets.

    // Setup Variables used when powered on.
    this->minutes;
    this->command;

    uint8_t* dummy_Data_array = new uint8_t[1, 2, 3, 4];
    uint8_t dummy_Data_size = 4;
    bool SWC_Received = false;

    bool transmit_flag = false;
    
    // Join LoRaWAN Network
    LoRa.setup();


    while (!SWC_Received) // Initialization Loop. Keep Retrieving SWC until one is received.
    {
        // Send Dummy Message and Receive SWC
        uint8_t send_response = handle_uplink(dummy_Data_array, dummy_Data_size);

        if (send_response == 2)
        {
            SWC_Received = true;
        }

        // Sleep Some time before trying again.
        
    }


    // Enter Run Loop
    while(true) 
    {
        dm.get_SWC_state(this->command, this->minutes);

        switch (command)
        {
        case 0:
            Serial.println("Take Measurement.");
            break;

        case 1:
            Serial.println("Take Measurement. And Send Data.");
            break;
        
        default:
            break;
        }

        // Sleep For the Defined Time.
        if (this->minutes > 0)
        {
            sleep(this->minutes);
        }
    }
}

int Orchestrator::sleep(uint16_t minutes)
{
    // Converting from minutes to millisecond since LowPower.deepsleep() takes millis as argument
    unsigned long sleep_time = minutes * 60000; // unsigned long --> Size (4 bytes) - Range 0 to 4,294,967,295
    LowPower.deepSleep(sleep_time);
    return sleep_time;
}

void Orchestrator::make_measurements()
{
    float temperature = this->Sensor.getTemperature();
    int moisturePercentage = this->Sensor.getMoisture();

    this->dm.append_data(moisturePercentage, temperature); // Call append_data
}

int Orchestrator::get_SoC()
{
    pinMode(14, INPUT);
    return digitalRead(14);
}

uint8_t Orchestrator::handle_uplink(uint8_t* data, uint8_t packet_size)
{
    uint8_t send_response = this->LoRa.send_data(data, packet_size);

    switch (send_response)
    {
    case 0:
        Serial.println("Error Transmitting.");
        break;

    case 1:
        Serial.println("Message Sent.");
        break;

    case 2:
        Serial.println("Message Sent. SWC Received and Applied.");

        // Handle the Downlink
        handle_downlink();

    default:
        break;
    }

    return send_response;
}

uint8_t Orchestrator::handle_downlink()
{
    // Fetch the Downlink Data from the loRa Manager
    std::pair<char*, uint16_t> SWC_Pair = this->LoRa.retrieve_data();

    // Set the SWC in the Data Manager
    dm.set_SWC(SWC_Pair.first);

    // Sleep For the Prolonged Offset
    if (SWC_Pair.second != 0)
    {
        this->sleep(SWC_Pair.second);
    }

    return 1;
}
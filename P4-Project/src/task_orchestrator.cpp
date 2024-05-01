#include "task_orchestrator.h"

Orchestrator::Orchestrator() 
    : Sensor(), // Initialize Sensor Manager
      LoRa(),   // Initialize LoRa Manager
      dm(), // Initialize Data Manager
      adc(this->ADC_VREF, this->SPI_CS)
{  
    // This will run when the IoT device has lost power and resets.

    // Setup Variables used when powered on.
    this->minutes;                              // Time to sleep after fetching command
    this->command;                              // Command from sleep wake cycle 0 or 1
    this->measurement_power_req;                // Power requirement in % needed to perform measurement
    this->transmit_power_req;                   // Power requirement in % needed to perform transmission
    this->has_data_measurements;                // Check if the device have acquired measurements. Needed for decisionmaking.
    this->SWC_received;                         // Flag to check if SWC has been received upon downlink.

    uint8_t* dummy_Data_array = new uint8_t[1, 1, 1, 1];
    uint8_t dummy_Data_size = 4;
    
    // setup of the external MCP3201 12-bit ADC, which measures the Voltage across the supercap (SoC)
    this->setup_SoC();

    // Join LoRaWAN Network
    this->LoRa.setup();

    // Initialization loop. Ensures we dont start 'working' until a SWC has been received.
    while (!this->SWC_received)
    {
        // Check if there is enough power to transmit
        if (this->transmit_power_req < this->get_SoC())
        {
            // Send Dummy Message and Receive SWC
            uint8_t send_response = handle_uplink(dummy_Data_array, dummy_Data_size);

        }

        // Sleep Some time before trying again.
    }


    // Enter Run Loop
    while(true) 
    {   
        // Get Command from SWC
        dm.get_SWC_state(this->command, this->minutes);

        // Reset SWC flag if necessary
        if (this->SWC_received)
        {
            this->SWC_received = false;
        }


        switch (this->command)
        {
        case 0:
        /*
        Simple case where only data measurement is required. Checks if we have enough power and performs the measurements.
        */  
            bool dummy = true;
            if ( this->measurement_power_req < this->get_SoC())
            {
                dummy = false;
            }

            // Make Measurements
            this->make_measurements(dummy);

            break;
        case 1:
        /*
        If it is possible, we want to both make measurements and transmit. First we check if we have enough power for both.
        If the first If statement completes, then the second one will also be true.
        There is however a chance that we might have some previous unsent measurements and only enough power to transmit. 
        Hence it becomes optimal to deliver some data instead of none.
        */
            bool dummy = true;

            if ((this->measurement_power_req + this->transmit_power_req) < this->get_SoC()) // Check if both measurement and transmit is possible.
            {
                dummy = false;
            }

            // Make measurements
            this->make_measurements(dummy);

            if (this->has_data_measurements && this->transmit_power_req < this->get_SoC()) // If we have enough power to transmit, check if there is any saved data.
            {
                // Get Data from Data Manager
                std::pair<uint8_t*, uint8_t> returned_pair = dm.return_data();
                uint8_t* returned_data = returned_pair.first;
                int array_size = returned_pair.second;

                // Handle Uplink with data
                this->handle_uplink(returned_data, array_size);

                // Reset data flag as old has been transmitted.
                this->has_data_measurements = false;
            }
        
        default:
            break;
        }
        
        // Sleep if required and if SWC_received flag is false
        if (this->minutes > 0 && this->SWC_received != true)
        {
            this->sleep(this->minutes);
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

void Orchestrator::make_measurements(bool dummy)
{   
    // Variable Initialization
    float temperature = -20.0;
    uint8_t moisturePercentage = 100;

    if (!dummy)
    {
        // Make measurements
        temperature = this->Sensor.getTemperature();
        moisturePercentage = this->Sensor.getMoisture();
    }

    if (!dummy || this->has_data_measurements)
    {
        // Append Data to data manager
        this->dm.append_data(moisturePercentage, temperature);

        // Set measurements flag to true
        this->has_data_measurements = true;
    }
}

uint8_t Orchestrator::setup_SoC() {
    pinMode(this->SPI_CS, OUTPUT);
    pinMode(this->SOC_ENABLE_PIN, OUTPUT);
    // Set initial states
    digitalWrite(this->SPI_CS, HIGH);
    digitalWrite(this->SOC_ENABLE_PIN, LOW);
    // initialize SPI interface for MCP3201
    SPISettings settings(ADC_CLK, MSBFIRST, SPI_MODE0);
    SPI.begin();
    SPI.beginTransaction(settings);
    return 1;
}

uint8_t Orchestrator::enable_SoC() {
    digitalWrite(this->SOC_ENABLE_PIN, HIGH);
    return 1;
}

uint8_t Orchestrator::disable_SoC() {
    digitalWrite(this->SOC_ENABLE_PIN, LOW);
    return 1;
}

uint16_t Orchestrator::get_SoC()
{
    uint16_t raw = adc.read(MCP3201::Channel::SINGLE_0);
    uint16_t val = adc.toAnalog(raw);   // convert from digital value to mV-value in range [0mV, 5000mV]
    return val;
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

    // Set SWC received Flag
    this->SWC_received = true;

    // Sleep For the Prolonged Offset
    if (SWC_Pair.second != 0)
    {
        this->sleep(SWC_Pair.second);
    }

    return 1;
}
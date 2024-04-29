#ifndef DATA_MANAGER_H
#define DATA_MANAGER_H

#include <Arduino.h>
#include <ArduinoJson.h>        // docs: https://arduinojson.org/v7/tutorial/deserialization/
#include <vector>
#include <utility>


class DataManager {

private:
    //  datastruct containing the SWC.
    /*  will look something like (JSON format)
        {
        "0":[0, 300],       "0" = the SWC_step_num, [0, 300] = [0:sleep command, 300:minutes]
        "1":[1, 300]
        }       
    */
    JsonDocument SWC;

    // keep track of key of initial step of SWC
    uint8_t init_step;

    // int keeping track of which step to use of the SWC
    uint8_t SWC_step_num;
    
    // offset the temperature data conversion range negatively to [(-32.0 - offset), (31.5 - offset)]
    float offset;

    // array containing compressed measurement data
    std::vector<uint8_t> compressed_data;

    // keep track of number of samples/size of 'compressed_data'
    uint8_t num_samples = 0;

    // tell the class whether to print or not
    bool verbose;

    uint8_t compress_soil_data(int soil_mois_data);

    uint8_t compress_temp_data(float air_temp_data);


public:

    // Constructor
    DataManager(bool set_verbose = false, float temp_data_offset = -12.0);

    // Destructor
    ~DataManager(); 

    uint8_t reset(bool rst_SWC = false);

    uint8_t set_SWC(const char *json, uint8_t first_step = 0);

    uint8_t get_SWC_state(uint8_t& command, uint16_t& sleep_mins);

    uint8_t confirm_SWC_state();

    uint8_t append_data(int soil_moist_data, float air_temp_data);
    
    std::pair<uint8_t*, uint8_t> return_data();
};

#endif // DATA_MANAGER_H

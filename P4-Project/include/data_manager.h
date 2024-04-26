#ifndef DATA_MANAGER_H
#define DATA_MANAGER_H

#include <Arduino.h>
#include <ArduinoJson.h>        // docs: https://arduinojson.org/v7/tutorial/deserialization/


class DataManager {

private:
    //  datastruct containing the SWC.
    /*  will look something like (JSON format)
        {
        "1":[0, 300],       "1" = the SWC_step_num, [0, 300] = [0:sleep command, 300:minutes]
        "2":[1, 300],
        "n": 2              "n" = number of steps (2 in this case)
        }       
    */
    
    JsonDocument SWC;

    // int keeping track of which step to use of the SWC
    uint8_t SWC_step_num;
    
    // array containing compressed measurement data
    int compressed_data;

    // tell the class whether to print or not
    bool verbose;

public:

    // Constructor
    DataManager(uint8_t first_step = 1, bool set_verbose = false);

    // Destructor
    ~DataManager(); 

    uint8_t set_SWC(const char json[]);

    uint8_t get_SWC_state(uint8_t& command, uint16_t& sleep_mins);

    uint8_t confirm_SWC_state();

    uint8_t append_data(float soil_moist_data, float air_temp_data);

    uint8_t clear_data();
    
    uint8_t return_data();

};

#endif // DATA_MANAGER_H
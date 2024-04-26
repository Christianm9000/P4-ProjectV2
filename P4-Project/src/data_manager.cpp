#include "data_manager.h"

DataManager::DataManager(uint8_t first_step, bool set_verbose) {
    // Constructor
    SWC_step_num = first_step;  // defaults to 1, meaning start at beginning of SWC
    verbose = set_verbose;      // defaults to false
    
}

DataManager::~DataManager() {
    // Destructor
}

// takes a serialized json-object containing a SWC.
// returns 1 in case of success, 0 in case of failure
uint8_t DataManager::set_SWC(const char *json) {
    JsonDocument doc;
    
    deserializeJson(doc, json);
    SWC = doc;

    if (verbose) {
        Serial.println("SWC JSON: ");
        serializeJsonPretty(SWC, Serial);
        Serial.println(); // New line for clarity
    }
    
    return 1; // Success
};

// pass references to the 'command' and 'sleep_mins'. They will be altered by this method and can be used afterwards.
// returns 1 in case of success, 0 in case of failure
uint8_t DataManager::get_SWC_state(uint8_t& command, uint16_t& sleep_mins) {

    String step_num_str = String(SWC_step_num);         // Convert SWC_step_num to a String
    const char* step_num_ptr = step_num_str.c_str();    // Get the pointer to the null-terminated character array

    Serial.println(step_num_ptr);

    // Access the current step using the index obtained from the pointer
    JsonArray current_step = SWC[step_num_ptr];

    if (verbose) {    
        Serial.println("Current step JSON: ");
        serializeJsonPretty(current_step, Serial);
        Serial.println(); // New line for clarity
    }
    
    if (current_step.isNull()) {
        return 0; // Failed to get SWC state
    }

    // Retrieve the command and sleep minutes values. ArduinoJSON cast them to the appropriate data types automatically
    command = current_step[0];      // Cast to uint8_t
    sleep_mins = current_step[1];   // Cast to uint16_t

    return 1;   // Success
}



// increments 'SWC_step_num' by one. If the 'SWC_step_num' is larger than the last step, reset 'SWC_step_num' to 1
// and start the SWC over.
uint8_t DataManager::confirm_SWC_state() {
    SWC_step_num ++;
    int max_step = SWC["n"];

    // check for out-of-bounds problems - wrap around the SWC_step_num when nescessary
    if (SWC_step_num > max_step) {
        SWC_step_num = 1;
    }

    return 1;
};

uint8_t DataManager::append_data(float soil_moist_data, float air_temp_data) {

};

uint8_t DataManager::clear_data() {

};

uint8_t DataManager::return_data() {

};

#include "data_manager.h"

DataManager::DataManager(bool set_verbose, float temp_data_offset) {
    // Constructor
    offset = temp_data_offset;  // defaults to -12.0, meaning temperature-readings should be in range [-20.0, 43.5] degrees Celsius
    verbose = set_verbose;      // defaults to false
    
}

// Destructor - clear 'compressed_data' vector. Simple attributes are cleared automatically (int, bool, etc).
// This method is automatically called if the DataManager-object goes out of scope or is explicitly deleted.
DataManager::~DataManager() {
    // Deallocate memory for the compressed data vector
    compressed_data.clear();
}

// reset all attributes and make the object ready for a new SWC.
// if rst_SWC flag is set to true, clear the SWC as well. Only set if intention is full reset 
uint8_t DataManager::reset(bool rst_SWC) {
    if (rst_SWC) {
        SWC.clear();
    }
    compressed_data.clear();    // clear stored data
    num_samples = 0;
    SWC_step_num = init_step;   // point to first step of SWC
    
    return 1;   // success
}

// takes a serialized json-object containing a SWC.
// returns 1 in case of success, 0 in case of failure
uint8_t DataManager::set_SWC(const char *json, uint8_t first_step) {

    init_step = first_step;     // defaults to 0, meaning start at beginning of SWC - used in "confirm_SWC_state()"
    SWC_step_num = first_step;  // defaults to 0, meaning start at beginning of SWC
    JsonDocument doc;
    deserializeJson(doc, json);
    SWC = doc;

    if (verbose) {
        Serial.println("SWC JSON: ");
        serializeJsonPretty(SWC, Serial);
        Serial.println();
        Serial.println();
    }
    
    return 1; // Success
};

// pass references to the 'command' and 'sleep_mins'. They will be altered by this method and can be used afterwards.
// returns 1 in case of success, 0 in case of failure
uint8_t DataManager::get_SWC_state(uint8_t& command, uint16_t& sleep_mins) {

    String step_num_str = String(SWC_step_num);         // Convert SWC_step_num to a String
    const char* step_num_ptr = step_num_str.c_str();    // Get the pointer to the null-terminated character array

    // Access the current step using the index obtained from the pointer
    JsonArray current_step = SWC[step_num_ptr];

    if (verbose) {
        Serial.println("step_num_ptr: " + String(step_num_ptr));
        Serial.println("Current step JSON: ");
        serializeJsonPretty(current_step, Serial);
        Serial.println();
        Serial.println();
    }
    
    if (current_step.isNull()) {
        return 0; // Failed to get SWC state - null pointer
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

    // unused params - needed to call 'get_SWC_state()'
    uint8_t dummy1;
    uint16_t dummy2;
    // if get_SWC_state returns 0, it means that the 'current_step' is a null-pointer. If so, set 'SWC_step_num' equal to 'init_step'
    if (get_SWC_state(dummy1, dummy2) == 0) {
        SWC_step_num = init_step;
    }

    return 1;
};

// compress the int into 7 bits using fixed-point arithmetic
uint8_t DataManager::compress_soil_data(int soil_mois_data) {
// Convert the input to a 7-bit representation - discard the MSB later, since uint8_t is still 8 bits ofc.
return static_cast<uint8_t>(map(soil_mois_data, 0, 100, 0, 127));   // map(value, fromLow, fromHigh, toLow, toHigh)
}

// compress the float into 7 bits using fixed-point arithmetic (6 bit int, 1 bit float)
// 'offset' offsets the conversion range negatively to [(-32.0 - offset), (31.5 - offset)]
// with an offset of -12.0, the actual measurements must be in [-20.0, 43.5]
uint8_t DataManager::compress_temp_data(float air_temp_data) {
    air_temp_data = constrain(air_temp_data, -32.0 - offset, 31.5 - offset);
    float air_temp = air_temp_data + offset;
    // make the range possitive [0.0, 63.5] with the "middlepoint" (0.0) between positive and negative being 32 + offset
    air_temp = air_temp + 32;
    uint8_t int_part = uint8_t(air_temp);
    uint8_t float_part = uint8_t((air_temp - int_part) * 2);
    // bitshift the integer-part 1 time to the left, making the value NXXX_XXXY, N = discard, X = integer-part, Y = float-part
    return (int_part << 1) | float_part;    // discard MSB later
}

// appends the datapoints to the SRAM storage. The data is compressed within this function before it is stored
uint8_t DataManager::append_data(int soil_moist_data, float air_temp_data) {
    // store compressed values in array - loop later, removes need for repeating code 2 times
    uint8_t compressed_values[2];
    compressed_values[0] = compress_soil_data(soil_moist_data);
    compressed_values[1] = compress_temp_data(air_temp_data);
    
    for (uint8_t i = 0; i < 2; i++) {
        // Reference to the current value
        uint8_t& current_value = compressed_values[i];

        // calculate how many useful bits 'compressed_data' stores.
        uint16_t stored_bits = num_samples * 7;  // 7 = size of one sample

        // if there are any "empty" bits in the back of the vector, place the MSB's of the new data there
        uint8_t bits_filled_in_back = stored_bits % 8;

        // Align comp_mois, currently MSB is empty, shift 1 to the left
        current_value = current_value << 1;
        
        if (bits_filled_in_back != 0) {
            // write MSB's of new data to LSB's of existing back
            uint8_t vect_back = compressed_data.back();
            vect_back = vect_back | (current_value >> bits_filled_in_back);   // bit-shift right to align with empty space, then OR with 'vect_back'

            // remove previous back, replace with new
            compressed_data.pop_back();
            compressed_data.push_back(vect_back);

            // create new back with .push_back() containing remaining bits
            compressed_data.push_back(current_value << (8 - bits_filled_in_back));
        }
        else {
            // create new back with .push_back()
            compressed_data.push_back(current_value);
        }
        num_samples++;
    }

    if (verbose) {
        // Iterate over the vector and print each element
        Serial.println("Elements of the 'compressed_data' vector:");
        for (const uint8_t& element : compressed_data) {
            Serial.print(element, BIN);
            Serial.print(" ");
        }
        Serial.println();
    }
    return 1;
}


// Returns a pointer to an array containing the compressed data in a uint8_t array.
// remember to dealloc via 'delete[] returned_pointer' when done using it.
// Throws a 'const std::bad_alloc' error if dynamic memory allocation via 'new' fails.
uint8_t* DataManager::return_data() {
    uint16_t used_bits = num_samples * 7;
    uint8_t used_bytes = uint8_t(used_bits / 8);
    if ((used_bits % 8) > 0) { used_bytes++; }

    uint8_t* data_array_ptr = new uint8_t[used_bytes];  // allocate on heap - persists after scope is exited

    if (verbose) {
        Serial.println("used_bits: " + String(used_bits) + " (" + String(used_bytes) + " bytes)");
        Serial.println("data_array_ptr: ");
    }

    for (int i = 0; i < used_bytes; i++) {          // copy the data from start to finish
        data_array_ptr[i] = compressed_data.at(i);  // instead of the LIFO .back(), .pop_back() - combo
        if (verbose) {
            Serial.print(data_array_ptr[i], BIN);
            Serial.print(" ");
        }
    }

    if (verbose) {
        Serial.println("");
    }

    return data_array_ptr;
}

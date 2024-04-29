
#include "data_manager.h"
#include "arduino_secrets.h"


void setup() {
  Serial.begin(9600);

  delay(1000);

  DataManager dm(true, -12.0);   // set verbose = true, temp_data_offset = -12.0

  const char* newSWC = "{\"0\":[0, 300],\"1\":[1, 300]}";
  
  // test .set_SWC - WORKING
  dm.set_SWC(newSWC, 0);

  // test .get_SWC_state - WORKING
  uint8_t command;
  uint16_t sleep_mins;

  for (int i = 0; i< 5; i++) {
    dm.get_SWC_state(command, sleep_mins);

    Serial.println("current instruction step from the SWC: ");
    Serial.print("command: ");
    Serial.println(command);

    Serial.print("sleep_mins: ");
    Serial.println(sleep_mins);
    Serial.println();
    // imagine that the SWC step is completed ...

    // test .confirm_SWC_state - WORKING
    dm.confirm_SWC_state();
  }


  // test .append_data() - WORKING
  int mois_data = 100;
  float temp_data = 31.5 + 12;
  dm.append_data(mois_data, temp_data);

  mois_data = 50;
  temp_data = 0 + 12;
  dm.append_data(mois_data, temp_data);

  mois_data = 0;
  temp_data = -32.0 + 12;
  dm.append_data(mois_data, temp_data);

  mois_data = 100;
  temp_data = 31.5 + 12;
  dm.append_data(mois_data, temp_data);
  // dm.append_data(mois_data, temp_data);
  

  std::pair<uint8_t*, uint8_t> returned_pair = dm.return_data();
  uint8_t* returned_data = returned_pair.first;
  uint8_t array_size = returned_pair.second;

  for (int i = 0; i < array_size; i++) {
          Serial.print(returned_data[i], BIN);
          Serial.print(" ");
  }

  // IMPORTANT
  delete[] returned_data;
  returned_data = nullptr;


  // test .reset() - 
  dm.reset(false);  // weak reset

  std::pair<uint8_t*, uint8_t> returned_pair2 = dm.return_data();
  uint8_t* returned_data2 = returned_pair2.first;
  uint8_t array_size2 = returned_pair2.second;

  for (int i = 0; i < array_size2; i++) {
          Serial.print(returned_data2[i], BIN);
          Serial.print(" ");
  }

  for (int i = 0; i< 5; i++) {
    dm.get_SWC_state(command, sleep_mins);

    Serial.println("current instruction step from the SWC: ");
    Serial.print("command: ");
    Serial.println(command);

    Serial.print("sleep_mins: ");
    Serial.println(sleep_mins);
    Serial.println();
    // imagine that the SWC step is completed ...

    // test .confirm_SWC_state - WORKING
    dm.confirm_SWC_state();
  }

  dm.reset(true);   // full reset - including SWC

  for (int i = 0; i< 5; i++) {
    dm.get_SWC_state(command, sleep_mins);

    Serial.println("current instruction step from the SWC: ");
    Serial.print("command: ");
    Serial.println(command);

    Serial.print("sleep_mins: ");
    Serial.println(sleep_mins);
    Serial.println();
    // imagine that the SWC step is completed ...

    // test .confirm_SWC_state - WORKING
    dm.confirm_SWC_state();
  }
}

void loop() {

}

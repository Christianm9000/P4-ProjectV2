#include <ArduinoLowPower.h>
#include "lorawan_manager.h"
#include "arduino_secrets.h"
#include "data_manager.h"

void setup() {
  Serial.begin(9600);

  delay(1000);

  int mois_data = 100;
  float temp_data = 31.5 + 12;

  LoRaWAN lm(SECRET_APP_EUI, SECRET_APP_KEY);
  DataManager dm(true, -12.0);   // set verbose = true, temp_data_offset = -12.0
  lm.setup();

  // test .append_data()
  
  dm.append_data(mois_data, temp_data);

  //mois_data = 50;
  //temp_data = 0 + 12;
  dm.append_data(mois_data, temp_data);

  //mois_data = 0;
  //temp_data = -32.0 + 12;
  dm.append_data(mois_data, temp_data);

  //mois_data = 100;
  //temp_data = 31.5 + 12;
  dm.append_data(mois_data, temp_data);

  dm.append_data(mois_data, temp_data);

  std::pair<uint8_t*, uint8_t> returned_pair = dm.return_data();
  uint8_t* returned_data = returned_pair.first;
  int array_size = returned_pair.second;

  while (true)
  {
    Serial.println("Block Code Until User Input");
    while (!Serial.available());
    String msg = Serial.readStringUntil('\n');

    Serial.println("Transmitting and Checking Downlink Now.");
    int sendStatus = lm.send_data(returned_data, array_size);
    Serial.println("Done with Transmit and Downlink Check.");

    if (sendStatus == 2) {
      std::pair<char*, int> SWC_Pair = lm.retrieve_data();
      
      char* downlinkData = SWC_Pair.first;
      uint16_t offset = SWC_Pair.second;

      String data = String(downlinkData);
      Serial.println("Downlink Data: " + data);

      Serial.println(offset);
      
    } else if (sendStatus == 1) {
      Serial.println("Message sent, no downlink.");
    } else {
      Serial.println("Failed to send message.");
    }
  }
}

void loop() {

}
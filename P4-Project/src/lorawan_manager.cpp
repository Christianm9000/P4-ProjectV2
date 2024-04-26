#include "lorawan_manager.h"
#include "arduino_secrets.h"

LoRaWAN::LoRaWAN(const String& eui, const String& key) : appEui(eui), appKey(key) {
  if (!modem.begin(EU868)) {
    Serial.println("Failed to start module");
    while (1);
  }
  Serial.println("Modem initialized successfully");
}


void LoRaWAN::setup() {
  Serial.begin(115200);
  while (!Serial);                              // Wait for Serial To Initialize
  
  if (!modem.joinOTAA(appEui, appKey)) {        // Join Given Network Via OTAA
    Serial.println("Joining network failed.");
    while (1);
  }

  Serial.println("Network Joined!");
  modem.minPollInterval(60);                    // The Amount of Seconds between checking for downlink. Works as a cooldown. If checked device must wait 60 sec to check again.
}


int LoRaWAN::set_config(bool adr, int spreadingFactor, int power) {
  // Set Variables
  this->TXPower = power;
  this->SpreadingFactor = spreadingFactor;
  this->ADR = adr;

  // Enable or Disable ADR
  modem.setADR(this->ADR);

  //modem.setSpreadingFactor(spreadingFactor);
  //modem.setTxPower(power);
  return 0; // Success
}


int LoRaWAN::send_data(const String& data) {
  // Declare Data to send and error variable
  this->dataToSend = data;
  int err;

  // Start Package Construction and Add Data
  modem.beginPacket();
  modem.print(dataToSend);

  // Send Data and receive Return Code
  err = modem.endPacket(true);
  if (err > 0) {
    Serial.println("Message sent correctly!");
    if (modem.available()) {
      return 2; // Sent correctly, downlink received
    }
    return 1; // Sent correctly, no downlink
  } else {
    Serial.println("Error sending message");
    return 0; // Send failed
  }
}


String LoRaWAN::retrieve_data() {
  // Check if Any Data is Available
  if (!modem.available()) {
    return ""; // No data received
  }

  String receivedData;
  while (modem.available()) {
    receivedData += (char)modem.read();          // Reads Byte for Byte of the buffer until no more data is available.
  }
  Serial.println(receivedData);

  return receivedData;
}

#include "lorawan_manager.h"
#include "arduino_secrets.h"

LoRaWAN::LoRaWAN(const String& eui, const String& key) : appEui(eui), appKey(key) {
  if (!modem.begin(EU868)) {
    Serial.println("Failed to start module");
    while (1);
  }
  Serial.println("Modem initialized successfully");

  // Set the appEUi and appKey
  this->appEui = eui;
  this->appKey = key;
}


void LoRaWAN::setup() {
  if (!modem.joinOTAA(this->appEui, this->appKey)) {            // Join Given Network Via OTAA
    Serial.println("Joining network failed.");
    while (1);
  }

  Serial.println("Network Joined!");
  modem.minPollInterval(60);              // The Amount of Seconds between checking for downlink. Works as a cooldown. If checked device must wait 60 sec to check again.
}


int LoRaWAN::set_config(bool adr, int spreadingFactor, int power) {
  // Set Variables
  this->TXPower = power;
  this->SpreadingFactor = spreadingFactor;
  this->ADR = adr;

  // Enable or Disable ADR
  //modem.setADR(this->ADR);

  //modem.setSpreadingFactor(spreadingFactor);
  //modem.setTxPower(power);
  return 0; // Success
}


int LoRaWAN::send_data(uint8_t* data) {
  // Declare Data to send and error variable
  int err;

  // Start Package Construction and Add Data
  modem.beginPacket();
  modem.write(data, sizeof(data));

  // Send Data and receive Return Code
  err = modem.endPacket(true);

  // Check Send Status Code
  if (err > 0) {
    Serial.println("Message sent correctly!");
  } else {
    Serial.println("Error sending message");
    return 0; // Send failed
  }

  //Delay to Allow Data Downlink
  for (unsigned int j = 0; j<2; j++){
    delay(5000);
    if (!modem.available()) {
      Serial.println("No downlink message received at this time.");
      if (j == 1) {
        return 1;
      }
    }
    else {
      Serial.println("Downlink Received.");
      return 2;
    }
  }

}


String LoRaWAN::retrieve_data() {
  // Check if Any Data is Available
  if (!modem.available()) {
    return ""; // No data received
  }

  char rcv[64];
  int i = 0;
  while (modem.available()) {
    rcv[i++] = (char)modem.read();
  }
  Serial.print("Received: ");
  for (unsigned int j = 0; j < i; j++) {
    Serial.print(rcv[j] >> 4, HEX);
    Serial.print(rcv[j] & 0xF, HEX);
    Serial.print(" ");
  }

  return rcv;
}

#include "lorawan_manager.h"
#include "arduino_secrets.h"

LoRaWAN::LoRaWAN(const String& eui, const String& key) {
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

  // Declare Response Variable
  uint8_t err;

  // Enable or Disable ADR and Set Response Variable
  if(modem.setADR(this->ADR)) {
    err = 1;
  }
  else {
    err = 0;
  }

  //modem.setSpreadingFactor(spreadingFactor);
  //modem.setTxPower(power);

  // Return Success 1 or Failed 0
  return err;
}


int LoRaWAN::send_data(uint8_t* data, uint8_t size) {
  // Declare Error Variable
  uint8_t err;

  // Start Package Construction and Add Data
  modem.beginPacket();
  modem.write(data, size);

  // Send Data and receive Return Code
  err = modem.endPacket(true);

  if (err != 0) {

    //Delay to Allow Data Downlink
    for (int j = 0; j<2; j++) { // Check twice with 5 second delay
      delay(5000);
      if (!modem.available()) {
        if (j == 1) {
          return 1; // If no data is received after second check, then return 1 = message sent, no downlink.
        }
      }
      else {
        return 2; // Else return 2 = Message sent, downlink received.
      }
    }
  }
  return 0; // Send failed
}


std::pair<char*, uint16_t> LoRaWAN::retrieve_data() {
  // Check if any data is available
  if (!modem.available()) {
    // No data received. Return nullptr and 0
    return {nullptr, 0};
  }

  // Check Packet Size and return nullptr if no data is available
  int packet_size = modem.parsePacket();

  // No packet or read error
  if (packet_size <= 0) {
    return {nullptr, 0};
  }

  // Allocate Memory for SWC plus null terminator
  char* rcv = new char[packet_size + 1];

  // Sleep offset before SWC start. Measured in minutes
  uint16_t offset = 0; 

  int i = 0;
  bool readingSWC = true; // True if we are still reading the SWC. False if we are reading the offset
  while (modem.available() && i < packet_size) { // Ensure we do not overflow the buffer

    // Read from buffer
    char c = (char)modem.read();

    if (readingSWC) {

      // End of SWC
      if (c == '}') {
        readingSWC = false;
        rcv[i] = '\0'; // Null-terminate the SWC string
      }
      rcv[i++] = c; // add character to char array

    } else {
      // Convert char to int and accumulate
      offset = offset * 10 + (c - '0');
    }
  }
  rcv[i] = '\0'; // Add string terminator if the char array is converted.
  
  return std::pair<char*, uint16_t>(rcv, offset);
}
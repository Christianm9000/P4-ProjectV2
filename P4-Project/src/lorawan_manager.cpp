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
      Serial.println("REC DATA: NO DOWNLINK.");
      return {nullptr} // No data received. Return nullptr and 0
  }

  // Check Packet Size and return nullptr if no data is available
  int packet_size = modem.parsePacket();

  // No packet or read error
  if (packet_size <= 0) {
    Serial.println("REC DATA: NO PACKET SIZE.");
    return std::pair<char*, uint16_t>(nullptr, 0);
  }

  // Allocate Memory and Declare sleep offset Variable
  char* rcv = new char[packet_size + 1];  // Allocate Memory for SWC plus null terminator
  uint16_t offset = 0; // Sleep offset before SWC start. Measured in minutes

  int i = 0;
  uint8_t packet_switch = 0; // 0 if we are still reading the SWC. 1 if we are reading the offset
  while (modem.available() && i < packet_size) { // Ensure we do not overflow the buffer

    // Add character to SWC cycle char array
    if (packet_switch == 0) {

      // Switch to offset calculation when the SWC cycle has been constructed.
      // if ((char)modem.peek() == '}') {
      //   packet_switch = 1;
      // }

      rcv[i++] = (char)modem.read();  // Read each byte as a character
    }

    // Add to the offset in case we are done with the SWC
    // else {
    //   // Create offset in minutes by left shifting number after each read to continue adding. Necessary as we can only read the next byte from the buffer.
    //   num = (uint8_t)modem.read();

    //   Serial.println("num: " + num);

    //   offset = offset*10 + num;/*(int)modem.read();*/
    // }
  }

  rcv[i] = '\0'; // Add string terminator if the char array is converted.
  
  return std::pair<char*, uint16_t>(rcv, offset);
}

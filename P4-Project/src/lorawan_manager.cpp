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
  while (!Serial);
  
  Serial.print("Module version: ");
  Serial.println(modem.version());
  Serial.print("Device EUI: ");
  Serial.println(modem.deviceEUI());

  if (!modem.joinOTAA(appEui, appKey)) {
    Serial.println("Joining network failed. Move near a window and retry.");
    while (1);
  }

  modem.minPollInterval(60);
}

int LoRaWAN::insert_data(const String& data) {
  dataToSend = data;
  return 0; // Success
}

int LoRaWAN::set_config(bool adr, int spreadingFactor, int power) {
  modem.setADR(adr);
  //modem.setSpreadingFactor(spreadingFactor);
  //modem.setTxPower(power);
  return 0; // Success
}

int LoRaWAN::send_data() {
  int err;
  modem.beginPacket();
  modem.print(dataToSend);
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
  if (!modem.available()) {
    return ""; // No data received
  }

  String receivedData;
  while (modem.available()) {
    receivedData += (char)modem.read();
  }
  return receivedData;
}

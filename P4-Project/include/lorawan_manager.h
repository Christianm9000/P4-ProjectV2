#ifndef LORAWAN_MANAGER_H
#define LORAWAN_MANAGER_H

#include <MKRWAN.h>

class LoRaWAN {
private:
  LoRaModem modem;
  String appEui;
  String appKey;
  String dataToSend;

public:
  // Constructor to initialize the modem with the regional settings
  LoRaWAN(const String& eui, const String& key);

  // Initialize serial communication and join the LoRaWAN network
  void setup();

  // Store data to be sent to the LoRa network
  int insert_data(const String& data);

  // Configure ADR, spreading factor, and TX power
  int set_config(bool adr, int spreadingFactor, int power);

  // Send data over LoRaWAN and check for downlink messages
  int send_data();

  // Retrieve data received from downlink
  String retrieve_data();
};

#endif // LORAWAN_MANAGER_H
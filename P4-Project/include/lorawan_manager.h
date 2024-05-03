#ifndef LORAWAN_MANAGER_H
#define LORAWAN_MANAGER_H

#include <MKRWAN.h>
#include <utility>
#include "arduino_secrets.h"

class LoRaWAN {
private:
  LoRaModem modem;
  String appEui;
  String appKey;
  int TXPower;
  bool ADR;

public:
  // Constructor to initialize the modem with the regional settings
  LoRaWAN(const String& eui = SECRET_APP_EUI, const String& key = SECRET_APP_KEY);

  // Join LoRaWAN network with the defined AppEUI and AppKEY
  uint8_t setup();

  // Configure ADR, spreading factor, and TX power
  uint8_t set_config(bool adr, uint8_t power);

  // Send data over LoRaWAN and check for downlink messages
  uint8_t send_data(uint8_t* data, uint8_t size);

  // Retrieve data received from downlink
  std::pair<char*, uint16_t> retrieve_data();
};

#endif // LORAWAN_MANAGER_H

#ifndef LORAWAN_MANAGER_H
#define LORAWAN_MANAGER_H

#include <MKRWAN.h>
#include <utility>

class LoRaWAN {
private:
  LoRaModem modem;
  String appEui;
  String appKey;
  int TXPower;
  int SpreadingFactor;
  bool ADR;

public:
  // Constructor to initialize the modem with the regional settings
  LoRaWAN(const String& eui, const String& key);

  // Join LoRaWAN network with the defined AppEUI and AppKEY
  void setup();

  // Configure ADR, spreading factor, and TX power
  int set_config(bool adr, int spreadingFactor, int power);

  // Send data over LoRaWAN and check for downlink messages
  int send_data(uint8_t* data, uint8_t size);

  // Retrieve data received from downlink
  std::pair<char*, int> retrieve_data();
};

#endif // LORAWAN_MANAGER_H

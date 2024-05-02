#include <ArduinoLowPower.h>
#include <MKRWAN.h>
#include <utility>
#include "arduino_secrets.h"
void setup()
{
  //Serial.begin(9600);
  //delay(1000);

  LoRaModem modem;
  
  if (!modem.begin(EU868)) {
      while (1);
  }

  if (!modem.joinOTAA(SECRET_APP_EUI, SECRET_APP_KEY)) {            // Join Given Network Via OTAA
    while (1);
  }

  modem.minPollInterval(60);              // The Amount of Seconds between checking for downlink. Works as a cooldown. If checked device must wait 60 sec to check again.


  while(true)
  {
    int err;
    modem.beginPacket();
    modem.print("poop");
    err = modem.endPacket(true);

    LowPower.deepSleep(30000);
  }
}

void loop()
{

}
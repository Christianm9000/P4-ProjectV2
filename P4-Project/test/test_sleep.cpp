#include <ArduinoLowPower.h>
#include <MKRWAN.h>
#include <utility>
#include "arduino_secrets.h"
void setup()
{
    Serial.begin(9600);
    delay(1000);

    LoRaModem modem;

    if (!modem.begin(EU868)) {
        Serial.println("Failed to start module");
        while (1);
    }

    Serial.println("Modem initialized successfully");

    Serial.println("Block Code Until User Input");
    while (!Serial.available());
    String msg = Serial.readStringUntil('\n');

    LowPower.deepSleep(30000);


}

void loop()
{

}
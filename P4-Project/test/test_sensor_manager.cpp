#include "sensor_manager.h"

void setup()
{
  Serial.begin(9600);

  delay(1000);

  SensorModule sm(5, 16, 1023, 300, 7);

  int moist;
  float temp;

  Serial.println("Block Code Until User Input");
  while (!Serial.available());
  String msg = Serial.readStringUntil('\n');

  sm.toggleSensorPower(); // Power on
  while(true)
  {
    moist = sm.getMoisture();
    temp = sm.getTemperature();

    delay(2000);
    Serial.println("moisture: " + String(moist) + "Temperture: " + String(temp));
    delay(2000);
  }
}

void loop()
{

}
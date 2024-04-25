#include "sensor_manager.h"
/*
SensorModule sensorModule(A1, 500, 200, 17); // A1: Moisture sensor pin, 500: Dry reading, 200: Wet reading, 17: DS18B20 digital data pin

float temperature = sensorModule.getTemperature();
int moisturePercentage = sensorModule.getMoisture();

*/


SensorModule::SensorModule(int moisturePin, int dry, int wet, int tempPin)
    : moisturePin(moisturePin), dryReading(dry), wetReading(wet), oneWire(tempPin), tempSensor(&oneWire) {
  // Start the temperature sensor
  tempSensor.begin();
}

int SensorModule::getMoisture() {
  int moistureValue = analogRead(moisturePin);
  int moisturePercentage = map(moistureValue, dryReading, wetReading, 0, 100);
  moisturePercentage = constrain(moisturePercentage, 0, 100);
  return moisturePercentage;
}

float SensorModule::getTemperature() {
  tempSensor.requestTemperatures();
  return tempSensor.getTempCByIndex(0);
}

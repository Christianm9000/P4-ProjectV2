#include "sensor_manager.h"
/*
SensorModule sensorModule(A1, 500, 200, 17); // A1: Moisture sensor pin, 500: Dry reading, 200: Wet reading, 17: DS18B20 digital data pin

float temperature = sensorModule.getTemperature();
int moisturePercentage = sensorModule.getMoisture();

*/


SensorModule::SensorModule(uint8_t sensorPowerPin, uint8_t moisturePin, uint16_t dry, uint16_t wet, uint8_t tempPin) : oneWire(tempPin) {
  // Initialize Attributes
  this->sensorsPowerPin = sensorPowerPin;
  pinMode(this->sensorsPowerPin, OUTPUT);
  digitalWrite(this->sensorsPowerPin, LOW); // start with sensors turned off
  this->moisturePin = moisturePin;
  this->dryReading = dry;
  this->wetReading = wet;
  this->tempSensor = &oneWire;

  tempSensor.begin();
}

int SensorModule::getMoisture() {
  int moistureValue = analogRead(this->moisturePin);
  int moisturePercentage = map(moistureValue, this->dryReading, this->wetReading, 0, 100);
  moisturePercentage = constrain(moisturePercentage, 0, 100);
  return moisturePercentage;
}

float SensorModule::getTemperature() {
  tempSensor.requestTemperatures();
  return tempSensor.getTempCByIndex(0);
}

bool SensorModule::toggleSensorPower() {
  digitalWrite(this->sensorsPowerPin, !digitalRead(this->sensorsPowerPin)); // Toggle based on current state
  return digitalRead(this->sensorsPowerPin);
}

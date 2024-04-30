#ifndef SENSOR_MANAGER_H
#define SENSOR_MANAGER_H

#include <Arduino.h>
#include <OneWire.h>
#include <DallasTemperature.h>

class SensorModule {
private:
  uint8_t moisturePin;
  uint8_t dryReading;
  uint8_t wetReading;
  OneWire oneWire;
  DallasTemperature tempSensor;

public:
  SensorModule(uint8_t moisturePin = PIN_A1, uint16_t dry = 500, uint8_t wet = 200, uint8_t tempPin = PIN_PA21);
  int getMoisture();
  float getTemperature();
};

#endif

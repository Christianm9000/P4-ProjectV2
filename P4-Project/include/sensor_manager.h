#ifndef SENSOR_MANAGER_H
#define SENSOR_MANAGER_H

#include <Arduino.h>
#include <OneWire.h>
#include <DallasTemperature.h>


class SensorModule {
private:
  uint8_t sensorsPowerPin;
  uint8_t moisturePin;
  uint8_t dryReading;
  uint8_t wetReading;
  OneWire oneWire;
  DallasTemperature tempSensor;

public:
  SensorModule(uint8_t sensorPowerPin = 5, uint8_t moisturePin = 16, uint16_t dry = 1023, uint8_t wet = 300, uint8_t tempPin = 7);
  
  int getMoisture();
  
  float getTemperature();
  
  bool toggleSensorPower();
};

#endif

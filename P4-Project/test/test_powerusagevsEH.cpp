#include "sensor_manager.h"
#include "lorawan_manager.h"
#include "arduino_secrets.h"
#include "data_manager.h"
#include <SPI.h>
#include <Mcp320x.h>
#include <Arduino.h>
#include <ArduinoLowPower.h>

void setup()
{
  Serial.begin(9600);

  delay(1000);

  Serial.println("Starting");
  delay(1000);

  SensorModule sm(5, 16, 1023, 300, 7);
  LoRaWAN lm(SECRET_APP_EUI, SECRET_APP_KEY);
  DataManager dm;
  MCP3201 adc(5000, 4);

  // Setup SoC
  pinMode(4, OUTPUT);
  pinMode(3, OUTPUT);
  // Set initial states
  digitalWrite(4, HIGH);
  digitalWrite(3, LOW);
  // initialize SPI interface for MCP3201
  SPISettings settings(1600000, MSBFIRST, SPI_MODE0);
  SPI.begin();
  SPI.beginTransaction(settings);

  lm.setup();

  int moist;
  float temp;

  // Serial.println("Block Code Until User Input");
  // while (!Serial.available());
  // String msg = Serial.readStringUntil('\n');

  //Get SoC
  digitalWrite(3, HIGH);
  delay(80);
  uint16_t raw = adc.read(MCP3201::Channel::SINGLE_0);
  uint16_t val = adc.toAnalog(raw);   // convert from digital value to mV-value in range [0mV, 5000mV]
  digitalWrite(3, LOW);

  Serial.println("SoC: " + String(val));

  sm.toggleSensorPower(); // Power on

  for(int j=0; j<4; j++)
  {
    moist = sm.getMoisture();
    temp = sm.getTemperature();

    delay(2000);
    Serial.println("moisture: " + String(moist) + "Temperture: " + String(temp));
    dm.append_data(moist, temp);
    delay(2000);
  }

  std::pair<uint8_t*, uint8_t> returned_pair = dm.return_data();
  uint8_t* returned_data = returned_pair.first;
  int array_size = returned_pair.second;

  Serial.println("Transmitting and Checking Downlink Now.");
  int sendStatus = lm.send_data(returned_data, array_size);
  Serial.println("Done with Transmit");
}

void loop()
{

}
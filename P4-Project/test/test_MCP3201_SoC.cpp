/**
 * Basic ADC reading example.
 * - connects to ADC
 * - reads value from channel
 * - converts value to analog voltage
 */

#include <SPI.h>
#include <Mcp320x.h>

#define SPI_CS    	    4 		    // SPI slave select
#define SOC_ENABLE_PIN  3           // Gate of MOSFET
#define ADC_VREF        5000        // 5V Vref
#define ADC_CLK         1600000     // SPI clock 1.6MHz


MCP3201 adc(ADC_VREF, SPI_CS);

void setup() {

  // configure PIN mode
  pinMode(SPI_CS, OUTPUT);
  pinMode(SOC_ENABLE_PIN, OUTPUT);

  // set initial PIN state
  digitalWrite(SPI_CS, HIGH);
  digitalWrite(SOC_ENABLE_PIN, LOW);

  // initialize serial
  Serial.begin(115200);

  // initialize SPI interface for MCP3201
  SPISettings settings(ADC_CLK, MSBFIRST, SPI_MODE0);
  SPI.begin();
  SPI.beginTransaction(settings);
}

void loop() {

  uint32_t t1;
  uint32_t t2;

  // start sampling
  Serial.println("Reading...");

  digitalWrite(SOC_ENABLE_PIN, HIGH);
  delay(100);
  t1 = micros();
  uint16_t raw = adc.read(MCP3201::Channel::SINGLE_0);
  t2 = micros();
  digitalWrite(SOC_ENABLE_PIN, LOW);

  // get analog value
  uint16_t val = adc.toAnalog(raw);

  // readed value
  Serial.print("value: ");
  Serial.print(raw);
  Serial.print(" (");
  Serial.print(val);
  Serial.println(" mV)");

  // sampling time
  Serial.print("Sampling time: ");
  Serial.print(static_cast<double>(t2 - t1) / 1000, 4);
  Serial.println("ms");

  delay(2000);
}

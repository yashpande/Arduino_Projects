#include "DHT.h"
const int pingPin = 7;
DHT dht;
#define DHT22_PIN 5

void setup() {
  // initialize serial communication:
  Serial.begin(115200);
  Serial.print("status,temperature,humidity,speed");
  dht.setup(2);
}

struct
{
  uint32_t total;
  uint32_t ok;
  uint32_t crc_error;
  uint32_t time_out;
  uint32_t unknown;
} stat = { 0,0,0,0,0 };


void loop()
{
  delay(dht.getMinimumSamplingPeriod());
  float humidity = dht.getHumidity();
  float temperature = dht.getTemperature();
  Serial.print(dht.getStatusString());
  Serial.print(",");
  Serial.print(humidity, 1);
  Serial.print(",");
  Serial.print(temperature, 1);
  Serial.print(",");
  long duration;
  pinMode(pingPin, OUTPUT);
  digitalWrite(pingPin, LOW);
  delayMicroseconds(2);
  digitalWrite(pingPin, HIGH);
  delayMicroseconds(5);
  digitalWrite(pingPin, LOW);
  pinMode(pingPin, INPUT);
  duration = pulseIn(pingPin, HIGH);
  Serial.print(duration);
  Serial.print(",");
  Serial.print((float) 4540000/duration);
  Serial.println();
}

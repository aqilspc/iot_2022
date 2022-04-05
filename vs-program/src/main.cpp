#include <Arduino.h>
#include "DHT.h"
#define DHTPIN D1
#define DHTTYPE DHT11
#define ijo D2   // led warna merah
#define kuning D3 // led warna hijau
#define abang D4  // led warnah biru
DHT dht(DHTPIN, DHTTYPE);
void setup()
{
  Serial.begin(115200);
  pinMode(ijo, OUTPUT); // atur pin-pin digital sebagai output
  pinMode(kuning, OUTPUT);
  pinMode(abang, OUTPUT);
  dht.begin();
}
void loop()
{
  float h = dht.readHumidity();
  float t = dht.readTemperature();
  float f = dht.readTemperature(true);

  if (isnan(h) || isnan(t) || isnan(f))
  {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }

  Serial.print(F("Temperature: "));
  Serial.print(t);
  Serial.print(F("°C "));
  if (t >= 28)
  {
    Serial.println(F("panas beh!"));
    digitalWrite(ijo, LOW);
    digitalWrite(kuning, LOW);
    digitalWrite(abang, HIGH);
  }
  else if (t < 28 && t >= 27)
  {
    Serial.println(F("normal beh!"));
    digitalWrite(ijo, HIGH);
    digitalWrite(kuning, LOW);
    digitalWrite(abang, LOW);
  }
  else
  {
    Serial.println(F("dingin beh!"));
    digitalWrite(ijo, LOW);
    digitalWrite(kuning, HIGH);
    digitalWrite(abang, LOW);
  }
  delay(2000);
}
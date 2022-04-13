#include <Arduino.h>
#include "DHT.h"
#define sensorLDR A0
#define DHTPIN D1
#define DHTTYPE DHT11
int nilaiSensor;
#define biru D3
#define abang D2
DHT dht(DHTPIN, DHTTYPE);
void setup() {
 Serial.begin(115200);
 pinMode(biru, OUTPUT);
 pinMode(abang, OUTPUT);
 Serial.println("Contoh Penggunaan Sensor LDR");
 dht.begin();
}
void loop() {
 nilaiSensor = analogRead(sensorLDR);
  float h = dht.readHumidity();
  float t = dht.readTemperature();
  float f = dht.readTemperature(true);

  if (isnan(h) || isnan(t) || isnan(f))
  {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }
  Serial.print("Nilai Sensor Cahaya: ");
  Serial.println(nilaiSensor);

  Serial.print(F("Temperature: "));
  Serial.print(t);
  Serial.print(F("°C "));

 if(nilaiSensor > 400 && t < 28){
   Serial.println(F("Cahaya redup dan suhu dingin"));
   digitalWrite(biru, HIGH);
   delay(50);
   digitalWrite(biru,LOW);
 }else if(nilaiSensor > 400 && t > 28){
   Serial.println(F("Cahaya redup dan suhu tinggi"));
   digitalWrite(biru, HIGH);
   delay(50);
   digitalWrite(biru,LOW);
 }else if(nilaiSensor < 400 && t < 28){
   Serial.println(F("Cahaya terang dan suhu dingin"));
   digitalWrite(abang, HIGH);
   delay(50);
   digitalWrite(abang,LOW);
 }else if(nilaiSensor < 400 && t > 28){
   Serial.println(F("Cahaya terang dan suhu tinggi"));
   digitalWrite(biru, HIGH);
   digitalWrite(abang,LOW);
 }
    delay(2000);
}
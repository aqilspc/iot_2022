#include <Arduino.h>
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
#include "DHT.h"
#define DHTPIN D5
#define DHTTYPE DHT11
#define ijo D6   // led warna merah
#define mera D3 // led warna hijau
#define biru D4  // led warnah biru
DHT dht(DHTPIN, DHTTYPE);
LiquidCrystal_I2C lcd(0x3F, 16, 2);
void setup() {
  // put your setup code here, to run once:
  lcd.init(); // initialize the lcd
  lcd.backlight();
  lcd.clear();
  lcd.home();
  Serial.begin(115200);
  pinMode(ijo, OUTPUT); // atur pin-pin digital sebagai output
  pinMode(mera, OUTPUT);
  pinMode(biru, OUTPUT);
  dht.begin();
}
void loop() {
  // put your main code here, to run repeatedly:
  float h = dht.readHumidity();
  float t = dht.readTemperature();
  float f = dht.readTemperature(true);
  if (isnan(h) || isnan(t) || isnan(f))
  {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }
  Serial.print(t);
  Serial.print(F("°C "));
  int far = (9/5 * t) + 32;
  Serial.print(F("Temperature: "));
  Serial.print(far);
  Serial.print(F("°F "));
  lcd.home();
  lcd.print(String(t)+ String("°C") + String(far) + String("°F"));
  lcd.setCursor(0, 1);
  //lcd.print(far);
  //lcd.setCursor(0, 1);
  lcd.print("2022-04-20 08:32");
  if (t >= 28)
  {
    Serial.println(F("panas beh!"));
    digitalWrite(ijo, LOW);
    digitalWrite(biru, LOW);

    digitalWrite(mera, HIGH);
    delay(50);
    digitalWrite(mera,LOW);
  }
  else if (t < 28 && t >= 27)
  {
    Serial.println(F("normal beh!"));
    digitalWrite(ijo, LOW);
    digitalWrite(mera, LOW);
    digitalWrite(biru, HIGH);
    delay(50);
    digitalWrite(biru,LOW);
  }
  else
  {
    Serial.println(F("dingin beh!"));
    digitalWrite(mera, LOW);
    digitalWrite(biru, LOW);
    digitalWrite(ijo,HIGH);
    delay(50);
    digitalWrite(ijo,LOW);
  }
  delay(2000);
}
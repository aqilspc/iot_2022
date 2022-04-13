#include <Arduino.h>
#define sensorLDR A0
int nilaiSensor;
#define kuning D3
void setup() {
 Serial.begin(115200);
 pinMode(kuning, OUTPUT);
 Serial.println("Contoh Penggunaan Sensor LDR");
 delay(3000);
}

void loop() {
 nilaiSensor = analogRead(sensorLDR);
 Serial.print("Nilai Sensor : ");
 Serial.println(nilaiSensor);
 if(nilaiSensor > 400){
   Serial.println(F("Sore / Malam beh!"));
   digitalWrite(kuning, HIGH);
 }else{
   Serial.println(F("Pagi beh!"));
   digitalWrite(kuning, LOW);
 }
    delay(2000);
}
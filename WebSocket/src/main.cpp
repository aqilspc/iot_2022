#include <Arduino.h>
#include <ESP8266WiFi.h>
#include "DHT.h"
#define DHTPIN D1
#define DHTTYPE DHT11
#define LED D4
DHT dht(DHTPIN, DHTTYPE);

const char *ssid = "INTERNET"; // nama SSID untuk koneksi Anda
const char *password = "12345678"; // password akses point WIFI Anda
const uint16_t port = 1884; // diganti dengan port serve Anda
const char *host = "103.102.1.134"; //diganti dengan host server Anda, bisa ip ataupun domain

void connect_wifi()
{
  Serial.printf("Connecting to %s ", ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println(" connected");
}

void connect_server()
{
  WiFiClient client;

  Serial.printf("\n[Connecting to %s ... ", host);
  if (client.connect(host, port))
  {
    Serial.println("connected]");

    if (isnan(h) || isnan(t) || isnan(f))
    Serial.println("[Sending a request]");
    //suhu 
     float h = dht.readHumidity();
     float t = dht.readTemperature();
     float f = dht.readTemperature(true);

    {
       Serial.println("Failed to read from DHT sensor!");
       return;
    }

    client.print(String ("Suhu Rumah Anda :") + String(t) + String("°C"));

    Serial.println("[Response:]");
    String line = client.readStringUntil('\n');
    Serial.println(line);
    client.stop();
    Serial.println("\n[Disconnected]");
  }
  else
  {
    Serial.println("connection failed!]");
    client.stop();
  }
  delay(3000);
}

void setup()
{
  Serial.begin(115200);
  connect_wifi();
  dht.begin();
}

void loop()
{
  connect_server();
}
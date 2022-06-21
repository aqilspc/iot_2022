
#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <SimpleDHT.h>
#define sensorLDR A0
// hp
// const char *ssid = "od3ng";
// const char *password = "0d3n9bro";

// kampus
const char *ssid = "itcbackend";            // sesuaikan dengan username wifi
const char *password = "12345678";        // sesuaikan dengan password wifi
const char *mqtt_server = "broker.hivemq.com"; // isikan server broker
int nilaiSensorCahaya;
WiFiClient espClient;
PubSubClient client(espClient);

SimpleDHT11 dht11(D7);

long now = millis();
long lastMeasure = 0;
String macAddr = "";

void setup_wifi()
{
  delay(10);
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("WiFi connected - ESP IP address: ");
  Serial.println(WiFi.localIP());
  macAddr = WiFi.macAddress();
  Serial.println(macAddr);
}

void reconnect()
{
  while (!client.connected())
  {
    Serial.print("Attempting MQTT connection...");
    if (client.connect(macAddr.c_str()))
    {
      Serial.println("connected");
    }
    else
    {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      delay(5000);
    }
  }
}

void setup()
{
  Serial.begin(115200);
  Serial.println("Mqtt Node-RED");
  setup_wifi();
  client.setServer(mqtt_server, 1883);
  Serial.begin(115200);
 Serial.println("Contoh Penggunaan Sensor LDR");
 delay(3000);
 nilaiSensorCahaya = analogRead(sensorLDR);
}

void loop()
{
  if (!client.connected())
  {
    reconnect();
  }
  if (!client.loop())
  {
    client.connect(macAddr.c_str());
  }
  now = millis();
  if (now - lastMeasure > 5000)
  {
    lastMeasure = now;
    int err = SimpleDHTErrSuccess;

    byte temperature = 0;
    byte humidity = 0;
    if ((err = dht11.read(&temperature, &humidity, NULL)) != SimpleDHTErrSuccess)
    {
      Serial.print("Pembacaan DHT11 gagal, err=");
      Serial.println(err);
      delay(1000);
      return;
    }
    static char temperatureTemp[7];
    dtostrf(temperature, 4, 2, temperatureTemp);
    Serial.println(temperatureTemp);

    static char nilaiSensorCahayaTemp[7];
    dtostrf(nilaiSensorCahaya, 4, 2, nilaiSensorCahayaTemp);
    Serial.println(nilaiSensorCahayaTemp);

    client.publish("room/suhu", temperatureTemp); // agar lebih unix silakan tambahkan NIM ex: 0001/room/suhu
    nilaiSensorCahaya = analogRead(sensorLDR);
    client.publish("room/cahaya", nilaiSensorCahayaTemp); // agar lebih unix silakan tambahkan NIM ex: 0001/room/suhu
     if(nilaiSensorCahaya > 400){
       Serial.println("terang");
      client.publish("room/cahayakondisi","terang");
    }else if(nilaiSensorCahaya < 400 ){
      Serial.println("redup");
      client.publish("room/cahayakondisi","redup"); 
    }

  }
}
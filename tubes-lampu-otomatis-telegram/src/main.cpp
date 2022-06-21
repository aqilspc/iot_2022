
#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <PubSubClient.h>
//#include <SimpleDHT.h>
#define sensorLDR A0
#include "CTBot.h"
CTBot myBot;
const int dop = D3;
// hp
// kampus
const char *ssid = "itcbackend";            // sesuaikan dengan username wifi
const char *password = "12345678";        // sesuaikan dengan password wifi
const char *mqtt_server = "129.146.255.226"; // isikan server broker
String token = "5425621140:AAFeRpdh4OibtGpjQDYYZ4_jdVw053Jpqxs";
int keputusan = 0;
int otomatis = 1;
int nilaiSensorCahaya;
//int nilaiLDR = 0;
WiFiClient espClient;
PubSubClient client(espClient);

//SimpleDHT11 dht11(D7);

//long now = millis();
//long lastMeasure = 0;
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
  //pinMode(dop,OUTPUT);
  Serial.println("Mqtt Node-RED");
  setup_wifi();
  client.setServer(mqtt_server, 1883);
  delay(3000);
  //nilaiSensorCahaya = analogRead(sensorLDR);
  Serial.println("Memulai Koneksi Telegram...");
  // connect the ESP8266 to the desired access point
  myBot.wifiConnect(ssid, password);
  // set the telegram bot token
  myBot.setTelegramToken(token);

    // check if all things are ok
  if (myBot.testConnection())
    Serial.println("\ntestConnection OK");
  else
    Serial.println("\ntestConnection Not OK");

  // set the pin connected to the LED to act as output pin
  pinMode(dop, OUTPUT);
  digitalWrite(dop, LOW); // turn off the led (inverted logic!)
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
  //now = millis();
  //if (now - lastMeasure > 5000)
  //{
    //lastMeasure = now;
    //int err = SimpleDHTErrSuccess;

    //byte temperature = 0;
    //byte humidity = 0;
    //if ((err = dht11.read(&temperature, &humidity, NULL)) != SimpleDHTErrSuccess)
    //{
      //Serial.print("Pembacaan DHT11 gagal, err=");
      //Serial.println(err);
      //delay(1000);
      //return;
    //}
    //static char temperatureTemp[7];
    //dtostrf(temperature, 4, 2, temperatureTemp);
    //Serial.println(temperatureTemp);

    static char nilaiSensorCahayaTemp[7];
    dtostrf(nilaiSensorCahaya, 4, 2, nilaiSensorCahayaTemp);
    Serial.println(nilaiSensorCahayaTemp);
    
    String kondisilampu = "Padam";
    String KondisiOtomatis = "Aktif";
    //client.publish("room/suhu", temperatureTemp); // agar lebih unix silakan tambahkan NIM ex: 0001/room/suhu
    nilaiSensorCahaya = analogRead(sensorLDR);
    //nilaiLDR = analogRead(sensorLDR);
    client.publish("room/cahaya", nilaiSensorCahayaTemp); // agar lebih unix silakan tambahkan NIM ex: 0001/room/suhu
     if(nilaiSensorCahaya < 500){
      if(otomatis == 1){
        digitalWrite(dop, LOW);
        kondisilampu = "Padam";
      }
       Serial.println("terang");
       Serial.println(keputusan);
       client.publish("room/cahayakondisi","terang");

    }else if(nilaiSensorCahaya > 500 ){
      if(otomatis == 1){
        digitalWrite(dop, HIGH);
        kondisilampu = "Nyala";
      }
      Serial.println(keputusan);
      Serial.println("redup");
      client.publish("room/cahayakondisi","redup"); 
    }

    TBMessage msg;
    if (myBot.getNewMessage(msg)) {
      if (msg.text.equalsIgnoreCase("matikan")) {
         keputusan = 1;                  
         digitalWrite(dop, LOW);                              
         myBot.sendMessage(msg.sender.id, "Lampu rumah bagian depan sudah mati"); 
      }
      else if (msg.text.equalsIgnoreCase("hidupkan")) {  
         keputusan = 1;
         digitalWrite(dop, HIGH);                    
         myBot.sendMessage(msg.sender.id, "Lampu rumah bagian depan sudah di hidupkan"); 
      }else if(msg.text.equalsIgnoreCase("keadaan")){
         myBot.sendMessage(msg.sender.id, (String)"Keadaan lampu saat ini " + kondisilampu + (String)". \n Tingkat intensitas cahaya : "+nilaiSensorCahaya+ (String)"\n Saklar otomatis : "+KondisiOtomatis+(String)". ");
      }else if(msg.text.equalsIgnoreCase("otomatis")){
         otomatis = 1;
         KondisiOtomatis = "Aktif";
         myBot.sendMessage(msg.sender.id, "Saklar lampu otomatis nyala!");
      }else if(msg.text.equalsIgnoreCase("nonotomatis")){
         otomatis = 0;
         KondisiOtomatis = "Non Aktif";
         myBot.sendMessage(msg.sender.id, "Saklar lampu otomatis mati!");
      }else {                                                    
         String reply;
         reply = "Selamat datang"+msg.sender.username + (String)".\n Anda dapat menggunakan perintah berikut ini \n hidupkan : untuk menyalakan lampu \n matikan : untuk mematikan lampu \n otomatis : untuk mengaktifkan penyalaan lampu secara otomatis \n nonotomatis : untuk mengnonaktifkan penyalaan lampu secara otomatis.";
         myBot.sendMessage(msg.sender.id, reply);             // and send it
      }
    }
  //}
  delay(500);
}

#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <PubSubClient.h>
//#include <SimpleDHT.h>
#include <ArduinoHttpClient.h>
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

char serverAddress[] = "129.146.241.54";  // server address
int port = 80;
int keputusan = 0;
int otomatis = 1;
int nilaiSensorCahaya;
String kondisilampu;
String KondisiOtomatis = "Aktif";
WiFiClient espClient;
HttpClient clientweb = HttpClient(espClient, serverAddress, port);

PubSubClient client(espClient);
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
  myBot.wifiConnect(ssid, password);
  myBot.setTelegramToken(token);

    // check if all things are ok
  if (myBot.testConnection())
    Serial.println("\ntestConnection OK");
   else
    Serial.println("\ntestConnection Not OK");

  pinMode(dop, OUTPUT);
  digitalWrite(dop, LOW);
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

    static char nilaiSensorCahayaTemp[7];
    dtostrf(nilaiSensorCahaya, 4, 2, nilaiSensorCahayaTemp);
    Serial.println(nilaiSensorCahayaTemp);
    
    
    nilaiSensorCahaya = analogRead(sensorLDR);
    client.publish("room/cahaya", nilaiSensorCahayaTemp); // agar lebih unix silakan tambahkan NIM ex: 0001/room/suhu
     if(nilaiSensorCahaya < 500){
      if(otomatis == 1){
          digitalWrite(dop, LOW);
          kondisilampu = "Padam";

          String postDataNaik;
          postDataNaik = String("/sensor.php?")+String("kondisi=") + String(kondisilampu) +String("&")+ String("otomatis=") + String(KondisiOtomatis) +String("&")+ String("ldr=") + String(nilaiSensorCahaya);
          clientweb.beginRequest();
          clientweb.post(postDataNaik);
          clientweb.sendHeader(HTTP_HEADER_CONTENT_TYPE, "application/x-www-form-urlencoded");
          clientweb.sendHeader(HTTP_HEADER_CONTENT_LENGTH, postDataNaik.length());
          clientweb.sendHeader("X-CUSTOM-HEADER", "custom_value");
          clientweb.endRequest();
          clientweb.write((const byte*)postDataNaik.c_str(), postDataNaik.length());
      }
       Serial.println("terang");
       Serial.println(keputusan);
       client.publish("room/cahayakondisi","terang");
       client.publish("room/kondisilampu","padam");

    }else if(nilaiSensorCahaya > 500 ){
      if(otomatis == 1){
          digitalWrite(dop, HIGH);
          kondisilampu = "Nyala";

          String postDataTurun;
          postDataTurun = String("/sensor.php?")+String("kondisi=") + String(kondisilampu) +String("&")+ String("otomatis=") + String(KondisiOtomatis) +String("&")+ String("ldr=") + String(nilaiSensorCahaya);
          clientweb.beginRequest();
          clientweb.post(postDataTurun);
          clientweb.sendHeader(HTTP_HEADER_CONTENT_TYPE, "application/x-www-form-urlencoded");
          clientweb.sendHeader(HTTP_HEADER_CONTENT_LENGTH, postDataTurun.length());
          clientweb.sendHeader("X-CUSTOM-HEADER", "custom_value");
          clientweb.endRequest();
          clientweb.write((const byte*)postDataTurun.c_str(), postDataTurun.length());
        
      }
      Serial.println(keputusan);
      Serial.println("redup");
      client.publish("room/cahayakondisi","redup"); 
      client.publish("room/kondisilampu","nyala");
    }

    TBMessage msg;
    if (myBot.getNewMessage(msg)) {

        //post data sender id dalam api jika ada di skip jika belum dimasukkan
        String postData;
        postData = String("sender_id=") + String(msg.sender.id);
        clientweb.beginRequest();
        clientweb.post("/sensortele.php");
        clientweb.sendHeader(HTTP_HEADER_CONTENT_TYPE, "application/x-www-form-urlencoded");
        clientweb.sendHeader(HTTP_HEADER_CONTENT_LENGTH, postData.length());
        clientweb.sendHeader("X-CUSTOM-HEADER", "custom_value");
        clientweb.endRequest();
        clientweb.write((const byte*)postData.c_str(), postData.length());
        
      if (msg.text.equalsIgnoreCase("matikan")) {
          keputusan = 1;                  
          digitalWrite(dop, LOW);                              
          myBot.sendMessage(msg.sender.id, "Lampu rumah bagian depan sudah di matikan \n oleh "+msg.sender.username+(String)".");

          //brodcast yg lain
          String postDataUrip;
          String cast = "mati";
          postDataUrip = String("/broadcast.php?")+String("sender_id=") + String(msg.sender.id) +String("&")+ String("cast=") + String(cast) +String("&")+ String("username=") + String(msg.sender.username);
          clientweb.beginRequest();
          clientweb.post(postDataUrip);
          clientweb.sendHeader(HTTP_HEADER_CONTENT_TYPE, "application/x-www-form-urlencoded");
          clientweb.sendHeader(HTTP_HEADER_CONTENT_LENGTH, postData.length());
          clientweb.sendHeader("X-CUSTOM-HEADER", "custom_value");
          clientweb.endRequest();
          clientweb.write((const byte*)postDataUrip.c_str(), postDataUrip.length()); 

      }else if (msg.text.equalsIgnoreCase("hidupkan")) {  
          keputusan = 1;
          digitalWrite(dop, HIGH);                    
          myBot.sendMessage(msg.sender.id, "Lampu rumah bagian depan sudah di hidupkan \n oleh "+msg.sender.username+(String)".");

          //brodcast yg lain
          String postDataUrip;
          String cast = "urip";
          postDataUrip = String("/broadcast.php?")+String("sender_id=") + String(msg.sender.id) +String("&")+ String("cast=") + String(cast) +String("&")+ String("username=") + String(msg.sender.username);
          clientweb.beginRequest();
          clientweb.post(postDataUrip);
          clientweb.sendHeader(HTTP_HEADER_CONTENT_TYPE, "application/x-www-form-urlencoded");
          clientweb.sendHeader(HTTP_HEADER_CONTENT_LENGTH, postData.length());
          clientweb.sendHeader("X-CUSTOM-HEADER", "custom_value");
          clientweb.endRequest();
          clientweb.write((const byte*)postDataUrip.c_str(), postDataUrip.length()); 

      }else if(msg.text.equalsIgnoreCase("keadaan")){
          myBot.sendMessage(msg.sender.id, (String)"Keadaan lampu saat ini " + kondisilampu + (String)". \n Tingkat intensitas cahaya : "+nilaiSensorCahaya+ (String)" \n Saklar otomatis : "+KondisiOtomatis+(String)".");
      }else if(msg.text.equalsIgnoreCase("otomatis")){
          otomatis = 1;
          KondisiOtomatis = "Aktif";
          myBot.sendMessage(msg.sender.id, "Saklar lampu otomatis di aktifkan! \n oleh "+msg.sender.username+(String)".");

          //brodcast yg lain
          String postDataUrip;
          String cast = "otomatis";
          postDataUrip = String("/broadcast.php?")+String("sender_id=") + String(msg.sender.id) +String("&")+ String("cast=") + String(cast) +String("&")+ String("username=") + String(msg.sender.username);
          clientweb.beginRequest();
          clientweb.post(postDataUrip);
          clientweb.sendHeader(HTTP_HEADER_CONTENT_TYPE, "application/x-www-form-urlencoded");
          clientweb.sendHeader(HTTP_HEADER_CONTENT_LENGTH, postData.length());
          clientweb.sendHeader("X-CUSTOM-HEADER", "custom_value");
          clientweb.endRequest();
          clientweb.write((const byte*)postDataUrip.c_str(), postDataUrip.length());

      }else if(msg.text.equalsIgnoreCase("nonotomatis")){
          otomatis = 0;
          KondisiOtomatis = "NonAktif";
          myBot.sendMessage(msg.sender.id, "Saklar lampu otomatis di non aktifkan! \n oleh "+msg.sender.username+(String)".");

           //brodcast yg lain
          String postDataUrip;
          String cast = "nonotomatis";
          postDataUrip = String("/broadcast.php?")+String("sender_id=") + String(msg.sender.id) +String("&")+ String("cast=") + String(cast) +String("&")+ String("username=") + String(msg.sender.username);
          clientweb.beginRequest();
          clientweb.post(postDataUrip);
          clientweb.sendHeader(HTTP_HEADER_CONTENT_TYPE, "application/x-www-form-urlencoded");
          clientweb.sendHeader(HTTP_HEADER_CONTENT_LENGTH, postData.length());
          clientweb.sendHeader("X-CUSTOM-HEADER", "custom_value");
          clientweb.endRequest();
          clientweb.write((const byte*)postDataUrip.c_str(), postDataUrip.length());
      }else if(msg.text.equalsIgnoreCase("info")){                                                  
          String reply;
          reply = "Selamat datang "+msg.sender.username + (String)".\n Anda dapat menggunakan perintah berikut ini \n keadaan : untuk mengecek kondisi \n hidupkan : untuk menyalakan lampu \n matikan : untuk mematikan lampu \n otomatis : untuk mengaktifkan penyalaan lampu secara otomatis \n nonotomatis : untuk mengnonaktifkan penyalaan lampu secara otomatis.";
          myBot.sendMessage(msg.sender.id, reply);             // and send it
      }else if(msg.text.equalsIgnoreCase("/start")){                                                  
          String reply;
          reply = "Selamat datang "+msg.sender.username + (String)".\n Anda dapat menggunakan perintah berikut ini \n keadaan : untuk mengecek kondisi \n hidupkan : untuk menyalakan lampu \n matikan : untuk mematikan lampu \n otomatis : untuk mengaktifkan penyalaan lampu secara otomatis \n nonotomatis : untuk mengnonaktifkan penyalaan lampu secara otomatis.";
          myBot.sendMessage(msg.sender.id, reply);             // and send it
      }else{
          String reply;
          reply = "Selamat datang "+msg.sender.username + (String)".\n perintah yang anda kirimkan tidak terdaftar silahkan ketik info untuk melihat perintah yang dapat dikrimkan.";
          myBot.sendMessage(msg.sender.id, reply);
      }
    }
  delay(4000);
}
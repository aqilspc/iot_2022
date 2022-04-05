#include <Arduino.h>
#define RED_LED D2   //led warna merah
#define GREEN_LED D5 //led warna hijau
#define BLUE_LED D4  //led warnah biru

void setup()
{
  Serial.begin(115200);
  pinMode(GREEN_LED, OUTPUT); //atur pin-pin digital sebagai output
  pinMode(RED_LED, OUTPUT);
  pinMode(BLUE_LED, OUTPUT);
  Serial.println("Contoh Program LED SOS");
}
void loop()
{
  digitalWrite(GREEN_LED, HIGH);
  digitalWrite(RED_LED, HIGH);
  digitalWrite(BLUE_LED, HIGH);
  delay(2150); 
  // 3 dits (3 titik atau huruf S)
  for (int x = 0; x < 3; x++)
  {
    digitalWrite(GREEN_LED, HIGH); // LED nyala
    delay(150);                  // delay selama 150ms
    digitalWrite(GREEN_LED, LOW); // LED mati
    delay(100);                  // delay selama 100ms
  }
    // 3 dahs (3 garis atau huruf O)
  for (int x = 0; x < 3; x++)
  {
    digitalWrite(GREEN_LED, HIGH); // LED nyala
    delay(400);                  // delay selama 400ms
    digitalWrite(GREEN_LED, LOW); // LED mati
    delay(100);                  // delay selama 100ms
  }
   for (int x = 0; x < 3; x++)
  {
    digitalWrite(BLUE_LED, HIGH); // LED nyala
    delay(150);                  // delay selama 150ms
    digitalWrite(BLUE_LED, LOW); // LED mati
    delay(100);                  // delay selama 100ms
  }
  delay(100);
  for (int x = 0; x < 3; x++)
  {
    digitalWrite(BLUE_LED, HIGH); // LED nyala
    delay(400);                  // delay selama 400ms
    digitalWrite(BLUE_LED, LOW); // LED mati
    delay(100);  
  }
  
   for (int x = 0; x < 3; x++)
  {
    digitalWrite(RED_LED, HIGH); // LED nyala
    delay(150);                  // delay selama 150ms
    digitalWrite(RED_LED, LOW); // LED mati
    delay(100);                  // delay selama 100ms
  }
  
    for (int x = 0; x < 3; x++)
  {
    digitalWrite(RED_LED, HIGH); // LED nyala
    delay(400);                  // delay selama 400ms
    digitalWrite(RED_LED, LOW); // LED mati
    delay(100);  
  }

  // wait 2 seconds before repeating the SOS signal
  delay(2000);
}

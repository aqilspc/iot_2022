#include <dummy.h>

// the setup function runs once when you press reset or power the board
void setup() {
  // initialize digital pin LED_BUILTIN as an output.
  pinMode(D1, OUTPUT);
  pinMode(D2, OUTPUT);
  pinMode(D3, OUTPUT);
  //pinMode(D4, OUTPUT);

}

// the loop function runs over and over again forever
void loop() {
  digitalWrite(D1, LOW);          
  digitalWrite(D2, LOW);
  digitalWrite(D3, LOW);
  //digitalWrite(D4, LOW); 
  delay(50); 
  //digitalWrite(D2, HIGH);
  digitalWrite(D1, HIGH);
  digitalWrite(D2, HIGH);
  digitalWrite(D3, HIGH);
  //digitalWrite(D4, HIGH);
  delay(50); // wait for a second
}

#include <Arduino.h>
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x3F, 16, 2);
void setup() {
  // put your setup code here, to run once:
  lcd.init(); // initialize the lcd
  lcd.backlight();
  lcd.clear();
  lcd.home();
}
void scrollText(int row, String message, int delayTime, int lcdColumns)
{
  for (int i = 0; i < lcdColumns; i++)
  {
    message = " " + message;
  }
  message = message + " ";
  int k = message.length();
  for (int pos = 0; pos < k; pos++)
  {
    lcd.setCursor(0, row);
    lcd.print(message.substring(pos, pos + lcdColumns));
    delay(delayTime);
  }
}
void loop() {
  // put your main code here, to run repeatedly:
  lcd.home();
  lcd.print("Polinema");
  scrollText(1, "KELOMPOK 3.", 250, 16);
}
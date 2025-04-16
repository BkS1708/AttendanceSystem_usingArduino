#include <LiquidCrystal.h>

// Define LCD Pins: RS, EN, D4, D5, D6, D7
LiquidCrystal lcd(8, 9, 10, 11, 12, 13);

void setup() {
  lcd.begin(16, 2); // Initialize 16x2 LCD
}

void loop() {
  lcd.clear();
  lcd.setCursor(0, 0); // Move cursor to the first line
  lcd.print("NMIMS");
  delay(1000);

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Project");
  delay(1000);

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("2");
  delay(1000);

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("ICT");
  delay(1000);
}

#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x3F,16,2);  // set the LCD address to 0x3F for a 16 chars (coloumn) and 2 line (row) display

void setup() {
  lcd.init();
  lcd.clear();         
  lcd.backlight();      // Make sure backlight is on
}

void loop() {
    // Print a message on both lines of the LCD.
    lcd.setCursor(0,0);   //Set cursor to character 2 on line 0
    lcd.print("0123456789101112");
    
    lcd.setCursor(0,1);   //Move cursor to character 2 on line 1
    lcd.print("abcedfghijklmnop");
    
    //lcd.print((char)223); //Print degree symbol
}
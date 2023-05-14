#include "LCD.h"

#define VERSION "0.0.1"

//byte settings_pic[] = {
//  B000000,
//  B000000,
//  B000000,
//  B000000,
//  B000000,
//  B000000,
//  B000000,
//  B000000,
//};

long timer = 0;

LCD lcd;

void setup() {
  Serial.begin(9600);
  lcd.init();
  
}

void loop() {
  if (Serial.available() > 0) {
    int s;
    while (Serial.available() > 0) {
      s = Serial.read();
    }
    Serial.print("Sim: ");
    Serial.println(s);
    if (s == 117) {
      Serial.println("u");
      lcd.menu_action(-1);
    }
    else if (s == 100) {
      Serial.println("d");
      lcd.menu_action(1);
    }
    else if (s == 62) {
      Serial.println(">");
      lcd.menu_action(0, 1);
    }
    else if (s == 60) {
      Serial.println("<");
      lcd.menu_action(0, -1);
    }
    else if (s == 112) {
      Serial.println("p");
      lcd.menu_action(0, 0, true);
    }
    lcd.battery++;
    lcd.ping++;
  }
  if (millis() - ::timer > 500) {
    //lcd.menu_action(-1);
    lcd.show_frame();
    ::timer = millis();
  }
}

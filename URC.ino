#include "OLED.h"

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

myOLED myoled;

void setup() {
  Serial.begin(9600);
  myoled.init();
  
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
      myoled.menu_action(-1);
    }
    else if (s == 100) {
      Serial.println("d");
      myoled.menu_action(1);
    }
    else if (s == 62) {
      Serial.println(">");
      myoled.menu_action(0, 1);
    }
    else if (s == 60) {
      Serial.println("<");
      myoled.menu_action(0, -1);
    }
    else if (s == 112) {
      Serial.println("p");
      myoled.menu_action(0, 0, true);
    }
    else if (s == 91) {
      Serial.println("[");
      myoled.menu_action(-1, 0, true);
    }
    else if (s == 93) {
      Serial.println("]");
      myoled.menu_action(1, 0, true);
    }
  }
  if (millis() - ::timer > 500) {
    //lcd.menu_action(-1);
    myoled.show_frame();
    ::timer = millis();
  }
}

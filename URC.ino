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

LCD mlcd;

void setup() {
  Serial.begin(9600);
  Serial.println("start");
  mlcd.init();
  
}

void loop() {
  Serial.println("next");
  if (millis() - ::timer > 2000) {
    Serial.println("iter");
    mlcd.act();
    ::timer = millis();
  }
}

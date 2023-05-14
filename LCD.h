#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <Arduino.h>

#define SCREEN_WIDTH 16
#define SCREEN_HEIGHT 2
#define SCREEN_ADDRESS 0x27

byte settings_pic[] = {
  B11110,
  B11111,
  B00110,
  B00110,
  B00110,
  B00110,
  B00110,
  B00000,
};

byte mods_pic[] = {
  B10000,
  B11000,
  B10010,
  B00111,
  B10000,
  B11000,
  B10000,
  B00000,
};

byte guide_pic[] = {
  B11111,
  B00000,
  B11111,
  B00000,
  B11111,
  B00000,
  B11111,
  B00000,
};

class LCD {
  private:
    uint8_t cnt = 0;
    uint8_t battery = 0;
    char menu[3][20] = {"Settings", "Modes", "Guide"};
    LiquidCrystal_I2C lcd = LiquidCrystal_I2C(SCREEN_ADDRESS, SCREEN_WIDTH, SCREEN_HEIGHT);
    
  public:
    LCD(){}

  void init() {
    lcd.init();
    lcd.createChar(0, settings_pic);
    lcd.createChar(1, mods_pic);
    lcd.createChar(2, guide_pic);
    lcd.backlight();
  }

  void LCD::show_battery() {
    if (battery >= 100) {
      lcd.setCursor(12, 0);
    }
    else if (battery > 9){
      lcd.setCursor(13, 0);
    }
    else {
      lcd.setCursor(14, 0);
    }
    lcd.print(battery);
    lcd.print("%");
  }

  void act() {
    lcd.clear();
    lcd.setCursor(0, 0);        
    //Serial.println(menu[cnt]);
    lcd.write(cnt);  
    lcd.print(menu[cnt]);
    cnt++;
    if (cnt > 2) {
      cnt = 0;
    }
    show_battery();
  }

};

//extern LCD lcd;

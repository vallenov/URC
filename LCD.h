#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <Arduino.h>

#define SCREEN_WIDTH 16
#define SCREEN_HEIGHT 2
#define SCREEN_ADDRESS 0x27

byte settings_raw_pic[] = {
  B11110,
  B11111,
  B00100,
  B00100,
  B00100,
  B00100,
  B00100,
  B00000,
};

byte mods_raw_pic[] = {
  B10000,
  B11000,
  B10010,
  B00111,
  B10000,
  B11000,
  B10000,
  B00000,
};

byte guide_raw_pic[] = {
  B11111,
  B00000,
  B11111,
  B00000,
  B11111,
  B00000,
  B11111,
  B00000,
};

byte ping_raw_pic[] = {
  B01110,
  B10001,
  B00100,
  B01010,
  B00000,
  B00100,
  B00000,
  B00000,
};

byte ethernity_raw_pic[] = {
  B00000,
  B00000,
  B01010,
  B10101,
  B01010,
  B00000,
  B00000,
  B00000,
};

class LCD {
  private:
    uint8_t cnt = 0; //test
    int8_t current_menu_position[2] = {0, 0};
    int8_t current_menu_level = 0; 
    uint8_t menu_len = 3;
    char menu[3][20] = {"Settings", "Modes", "Guide"}; // list of items
    char menu2[3][3][20] = {
      {"Sett1", "Sett2", "Sett3"},
      {"Mod1", "Mod2", "Mod3"},
      {"G1", "G2", "G3"}
    };
    LiquidCrystal_I2C lcd = LiquidCrystal_I2C(SCREEN_ADDRESS, SCREEN_WIDTH, SCREEN_HEIGHT);

    uint8_t ping_pic = 3;
    uint8_t ethernity_pic = 4;
    
  public:
    uint8_t battery = 0; // battery level
    uint8_t ping = 0; //ping
    LCD(){}

  void init() {
    lcd.init();
    lcd.createChar(0, settings_raw_pic);
    lcd.createChar(1, mods_raw_pic);
    lcd.createChar(2, guide_raw_pic);
    lcd.createChar(3, ping_raw_pic);
    lcd.createChar(4, ethernity_raw_pic);
    lcd.backlight();
  }

  void show_battery() {
    if (battery >= 100) {
      lcd.setCursor(12, 0);
      battery = 100;
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

  void show_ping() {
    lcd.setCursor(0, 0);
    lcd.write(ping_pic);
    if (ping > 99) {
      lcd.print("!");
    }
    else {
      lcd.print(ping);
    }
  }

  void show_menu() {
    lcd.setCursor(current_menu_level, 1);        
    lcd.write(current_menu_position[0]);
    if (current_menu_level == 0) {
      lcd.print(menu[current_menu_position[current_menu_level]]);
    }
    if (current_menu_level == 1) {
      lcd.print(menu2[current_menu_position[current_menu_level-1]][current_menu_position[current_menu_level]]);
    }
  }

  void show_frame() {
    lcd.clear();
    show_battery();
    show_ping();
    show_menu(); 
  }

  void menu_action(int8_t act = 0, int8_t change_level = 0) {
    if (change_level != 0) {
      current_menu_level = current_menu_level + change_level;
      if (current_menu_level < 0) {
        current_menu_level = 0;
      }
      if (current_menu_level > 1) {
        current_menu_level = 1;
      }
      current_menu_position[1] = 0;
    }
    else if (act != 0) {
      current_menu_position[current_menu_level] = current_menu_position[current_menu_level] + act;
      if (current_menu_position[current_menu_level] > 2) {
        current_menu_position[current_menu_level] = 0;
      }
      if (current_menu_position[current_menu_level] < 0) {
        current_menu_position[current_menu_level] = menu_len-1;
      }
    }
  }
};

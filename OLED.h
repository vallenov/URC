#include <OLED_I2C.h>
#include <Arduino.h>
#include <string.h>

#define SCREEN_WIDTH 128 
#define SCREEN_HEIGHT 64 

extern uint8_t SmallFont[];
extern uint8_t TinyFont[];

OLED  disp(SDA, SCL, 8);

class myOLED {
  private:
    uint8_t left_blank_wide = 10;
    int8_t current_menu_position[2] = {0, 0};
    int8_t current_menu_level = 0; 
    uint8_t menu_len = 4;
    boolean backlight = true;
    uint8_t brightness = 50; // in percent
    uint8_t timeout = 5;
    char menu[4][10] = {"Screen", "NRF24", "Modes", "Guide"}; // list of items
    char menu2[4][4][10] = {
      {"Backlight", "Timeout", "Lightness", "Sett4"},
      {"Frequency", "Powermode", "Sett3", "Sett4"},
      {"Mod1", "Mod2", "Mod3", "Mod4"},
      {"G1", "G2", "G3", "G4"}
    };
    uint8_t ping_pic = 3;
    uint8_t ethernity_pic = 4;
    
  public:
    uint8_t battery = 56; // battery level
    uint16_t ping = 999; //ping
    myOLED(){}

  void init() {
    disp.begin();
    disp.setBrightness(brightness);
    disp.setFont(SmallFont);
  }

  void show_battery() {
    if (battery >= 100) {
      battery = 100;
    }
    char buf[4];
    itoa(battery, buf, 10);
    strcat(buf, "%");
    disp.print(buf, RIGHT, 0);
  }

  void show_ping() {
    if (ping > 999) {
      ping = 999;
    }
    char buf[15];
    itoa(ping, buf, 10);
    strcat(buf, "ms");
    disp.print(buf, 75, 0);
  }

  void show_menu() {
    if (current_menu_level == 0) {
      for (uint8_t i = 0; i < menu_len; i++) {
        uint8_t shift = i+2;
        if (i == current_menu_position[current_menu_level]) {
          disp.print(">", LEFT, shift*8);
        }
        disp.print(menu[i], left_blank_wide, shift*8);
      } 
    }
    if (current_menu_level == 1) {
      for (uint8_t i = 0; i < menu_len; i++) {
        uint8_t shift = i+2;
        uint8_t col = shift*8;
        if (i == current_menu_position[current_menu_level]) {
          disp.print(">", LEFT, col);
        }
        Serial.println(menu2[current_menu_position[current_menu_level-1]][i]);
        disp.print(menu2[current_menu_position[current_menu_level-1]][i], left_blank_wide, col);
        if (current_menu_position[0] == 0) {
          if (i == 0) {
            disp.print(backlight ? "ON" : "OFF", RIGHT, col);
          }
          else if (i == 1) {
            char buf[10];
            itoa(timeout, buf, 10);
            strcat(buf, "sec");
            disp.print(buf, RIGHT, col);
          }
          else if (i == 2) {
            char buf[10];
            itoa(brightness, buf, 10);
            strcat(buf, "%");
            disp.print(buf, RIGHT, col);
          }
          if (i == 3) {
            disp.print(backlight ? "ON" : "OFF", RIGHT, col);
          }
        }
        else if (current_menu_position[0] == 1){
          if (i == 0) {
            disp.print(backlight ? "ON" : "OFF", RIGHT, col);
          }
          else if (i == 1) {
            disp.print(backlight ? "ON" : "OFF", RIGHT, col);
          }
          else if (i == 2) {
            disp.print(backlight ? "ON" : "OFF", RIGHT, col);
          }
          else if (i == 3) {
            disp.print(backlight ? "ON" : "OFF", RIGHT, col);
          }
        }
        else if (current_menu_position[0] == 2){
          if (i == 0) {
            disp.print(backlight ? "ON" : "OFF", RIGHT, col);
          }
          else if (i == 1) {
            disp.print(backlight ? "ON" : "OFF", RIGHT, col);
          }
          else if (i == 2) {
            disp.print(backlight ? "ON" : "OFF", RIGHT, col);
          }
          else if (i == 3) {
            disp.print(backlight ? "ON" : "OFF", RIGHT, col);
          }
        }
        else if (current_menu_position[0] == 3){
          if (i == 0) {
            disp.print(backlight ? "ON" : "OFF", RIGHT, col);
          }
          else if (i == 1) {
            disp.print(backlight ? "ON" : "OFF", RIGHT, col);
          }
          else if (i == 2) {
            disp.print(backlight ? "ON" : "OFF", RIGHT, col);
          }
          else if (i == 3) {
            disp.print(backlight ? "ON" : "OFF", RIGHT, col);
          }
        }
      }
    }
  }

  void show_frame() {
    disp.clrScr();
    show_battery();
    show_ping();
    show_menu(); 
    disp.update();
  }

  void menu_action(int8_t act = 0, int8_t change_level = 0, bool pressX = false) {
    if (!pressX and change_level != 0) {
      current_menu_level = current_menu_level + change_level;
      if (current_menu_level < 0) {
        current_menu_level = 0;
      }
      if (current_menu_level > 1) {
        current_menu_level = 1;
      }
    }
    else if (!pressX and act != 0) {
      current_menu_position[current_menu_level] = current_menu_position[current_menu_level] + act;
      if (current_menu_position[current_menu_level] > menu_len - 1) {
        current_menu_position[current_menu_level] = 0;
      }
      if (current_menu_position[current_menu_level] < 0) {
        current_menu_position[current_menu_level] = menu_len - 1;
      }
    }
    else if (pressX and (current_menu_level >= 1)) {
      if (current_menu_position[0] == 0) {
        if (current_menu_position[1] == 0) {
          switch_backlight();
        }
        else if (current_menu_position[1] == 1) {
          change_timeout(act);
        }
        else if (current_menu_position[1] == 2) {
          brightness = brightness + act*10;
          if (brightness < 10) {
            brightness = 10;
          }
          else if (brightness > 255) {
            brightness = 255;
          }
          disp.setBrightness(int((255/100) * brightness));
        }
      }
      else if (current_menu_position[0] == 1){
        if (current_menu_position[1] == 0) {
          switch_backlight();
        }
        else if (current_menu_position[1] == 1) {
          switch_backlight();
        }
        else if (current_menu_position[1] == 2) {
          switch_backlight();
        }
      }
      else if (current_menu_position[0] == 2){
        if (current_menu_position[1] == 0) {
          switch_backlight();
        }
        else if (current_menu_position[1] == 1) {
          switch_backlight();
        }
        else if (current_menu_position[1] == 2) {
          switch_backlight();
        }
      }
      else if (current_menu_position[0] == 3){
        if (current_menu_position[1] == 0) {
          switch_backlight();
        }
        else if (current_menu_position[1] == 1) {
          switch_backlight();
        }
        else if (current_menu_position[1] == 2) {
          switch_backlight();
        }
      }
    }
  }

  void change_timeout(int8_t delta) {
    timeout = timeout + delta;
    if (timeout < 1) {
      timeout = 1;
    }
    else if (timeout > 10) {
      timeout = 10;
    }
  }

  void switch_backlight() {
    if (backlight) {
      disp.setBrightness(0);
      backlight = false;
    }
    else {
      disp.setBrightness(brightness);
      backlight = true;
    }
  }
};

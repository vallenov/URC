#include <SPI.h>
#include "nRF24L01.h"
#include "RF24.h"
#include <SoftwareSerial.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include "OLED.h"

// Настройки для NRF24
#define SIG_POWER RF24_PA_LOW
#define SIG_SPEED RF24_1MBPS

#define VERSION "0.0.1"

//using namespace std;

/*
 Соответствие номеров кнопкам
 
 0 - UP
 1 - RIGHT
 2 - DOWN
 3 - LEFT
 4 - SELECT
 5 - ANALOG
 6 - START
 7 - 1
 8 - 2
 9 - 3
 10 - 4
 11 - RIGHT L2
 12 - RIGHT L1
 13 - LEFT L2
 14 - LEFT L1
 */

long timer = 0;
byte address[][6] = {"1Node", "2Node", "3Node", "4Node", "5Node", "6Node"}; // возможные номера труб

uint8_t old_rc_chanel; // Текущий канал передачи данных

myOLED myoled;

RF24 radio(9, 10);

SoftwareSerial mySerial(6, 7);

void radioSetup() {
  radio.begin();              // активировать модуль
  radio.setAutoAck(1);        // режим подтверждения приёма, 1 вкл 0 выкл
  radio.setRetries(0, 15);    // (время между попыткой достучаться, число попыток)
  radio.enableAckPayload();   // разрешить отсылку данных в ответ на входящий сигнал
  radio.setPayloadSize(32);   // размер пакета, в байтах
  radio.openWritingPipe(address[0]);   // мы - труба 0, открываем канал для передачи данных
  radio.setPALevel(SIG_POWER);         // уровень мощности передатчика
  radio.setDataRate(SIG_SPEED);        // скорость обмена
  // должна быть одинакова на приёмнике и передатчике!
  // при самой низкой скорости имеем самую высокую чувствительность и дальность!!

  radio.powerUp();         // начать работу
  radio.stopListening();   // не слушаем радиоэфир, мы передатчик
}

void check_actual_rc_channel() {
  if (old_rc_chanel != myoled.rc_chanel) {
    radio.setChannel(myoled.rc_chanel);
    old_rc_chanel = myoled.rc_chanel;
  }
}

bool get_data_from_gamepad(byte *content) {
  mySerial.write(" ");
  uint8_t sz = 0;
  if (mySerial.available()) {
    uint8_t zero_cnt = 0;
    uint8_t buff_cnt = 0;
    uint8_t int_cnt = 0;
    bool receiving = false;
    char buff[5] = {};
    while (mySerial.available()) {
      if (mySerial.peek() == 36) {
        int_cnt = 0;
        receiving = true;
        mySerial.read();
      }
      if (receiving) {
        if (mySerial.peek() == 94) {
          receiving = false;
          break;
        }
        buff[buff_cnt] = mySerial.read();
        buff_cnt++;
        if (mySerial.peek() == 59) {
          mySerial.read();
          uint8_t val = atoi(buff);
          if (val == 0) {
            zero_cnt++;
            Serial.println(zero_cnt);
          }
          if (zero_cnt >= 5) {
            Serial.println("ret");
            return false;
          }
          content[int_cnt] = val;
          buff_cnt = 0;
          int_cnt++;
          buff[5] = {};
          continue;
        }
      }
      else {
        mySerial.read();
      }
    }
    if (receiving) {
      return false;
    }
    return true;    
  }
  return false;
}

void transmit_pac() {
  bool success = false;
  byte button_data[5] = {};
  success = get_data_from_gamepad(button_data);
  if (success == false) {
    return;
  }
  while (mySerial.available()) {
    mySerial.read();
  }
  Serial.println();
  Serial.print("str res: ");
  for (uint8_t current = 0; current < 5; current++) {
    Serial.print(button_data[current]);
    Serial.print(" ");
  }
  Serial.println();
  delay(10);
  // отправка пакета в эфир
  if (radio.write(&button_data, sizeof(button_data))) {
    Serial.print("Transmit: ");
    for (uint16_t i : button_data) {
      Serial.print(i);
      Serial.print(" ");
    }
    Serial.println();
  }
}

void setup() {
  Serial.begin(9600);
  mySerial.begin(9600);
  myoled.init();
  radioSetup();
}

void loop() {
//  check_actual_rc_channel();
  transmit_pac();
//  if (Serial.available() > 0) {
//    int s;
//    while (Serial.available() > 0) {
//      s = Serial.read();
//    }
//    Serial.print("Sim: ");
//    Serial.println(s);
//    if (s == 117) {
//      Serial.println("u");
//      myoled.menu_action(-1);
//    }
//    else if (s == 100) {
//      Serial.println("d");
//      myoled.menu_action(1);
//    }
//    else if (s == 62) {
//      Serial.println(">");
//      myoled.menu_action(0, 1);
//    }
//    else if (s == 60) {
//      Serial.println("<");
//      myoled.menu_action(0, -1);
//    }
//    else if (s == 112) {
//      Serial.println("p");
//      myoled.menu_action(0, 0, true);
//    }
//    else if (s == 91) {
//      Serial.println("[");
//      myoled.menu_action(-1, 0, true);
//    }
//    else if (s == 93) {
//      Serial.println("]");
//      myoled.menu_action(1, 0, true);
//    }
//  }
  
//  if (millis() - ::timer > 500) {
//    //lcd.menu_action(-1);
//    myoled.show_frame();
//    ::timer = millis();
//  }
}

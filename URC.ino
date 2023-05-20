#include <SPI.h>
#include "nRF24L01.h"
#include "RF24.h"
#include <SoftwareSerial.h>

#include "OLED.h"

// Настройки для NRF24
#define SIG_POWER RF24_PA_LOW
#define SIG_SPEED RF24_1MBPS

#define VERSION "0.0.1"

long timer = 0;
byte address[][6] = {"1Node", "2Node", "3Node", "4Node", "5Node", "6Node"}; // возможные номера труб

uint8_t old_rc_chanel; // Текущий канал передачи данных
byte transmit_data[3]; // массив пересылаемых данных

int16_t buttons_state[2] = {0, 0};

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

void transmit_pac() {
  Serial.print("Size if t data: ");
  Serial.println(sizeof(transmit_data));
  // отправка пакета 
  if (radio.write(&transmit_data, sizeof(transmit_data))) {
    Serial.print("Transmit: ");
    for (uint8_t i : transmit_data) {
      Serial.print(i);
      Serial.print(" ");
    }
    Serial.println();
  }
}

void setup() {
  Serial.begin(9600);
  myoled.init();
  radioSetup();
}

void loop() {
  check_actual_rc_channel();
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

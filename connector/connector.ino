int button_data[15];
bool press[15] = {false, false, false, false, false, false, false, false, false, false, false, false, false, false, false};
// Что бы не загуружать процессор при каждой итерации
uint16_t sq_list[15] = {
      1, 2, 4, 8, 16, 32, 64, 128, 256, 512, 1024, 2048, 4096, 8192, 16384
    };
int right_ox, right_oy, left_ox, left_oy;

/*
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

void setup() {
  for (uint8_t pin = 2; pin <= 12; pin++) {
    pinMode(pin, INPUT_PULLUP);
  }
  pinMode(A0, INPUT);
  pinMode(A1, INPUT);
  pinMode(A2, INPUT);
  pinMode(A3, INPUT);
  pinMode(A4, INPUT);
  pinMode(A5, INPUT);
  pinMode(A6, INPUT);
  pinMode(A7, INPUT);
  Serial.begin(9600);
  //Serial.println("Start");
}

void loop() {
    // Читаем инфу с цифровфх пинов
    for (uint8_t current_pin = 0; current_pin <= 10; current_pin++) {
      button_data[current_pin] = !digitalRead(current_pin+2);
      //Serial.print(button_data[current_pin]);
      if (button_data[current_pin] == HIGH) {
        press[current_pin] = true;
//        Serial.print("Button ");
//        Serial.print(current_pin);
//        Serial.println(" pressed!");
      }
      if (press[current_pin] and button_data[current_pin] == LOW) {
        press[current_pin] = false;
//        Serial.print("Button ");
//        Serial.print(current_pin);
//        Serial.println(" released!");
      }
    }

    // Читаем аналоговые, которые выполняют роль цифровых
    uint8_t abutton_data[4] = {A0, A1, A2, A3};
    for (uint8_t current_pin = 0; current_pin <= 3; current_pin++) {
      button_data[current_pin+11] = !analogRead(abutton_data[current_pin]);
      //Serial.print(button_data[current_pin+11]);
      if (button_data[current_pin+11] == HIGH) {
        press[current_pin + 11] = true;
//        Serial.print("Button ");
//        Serial.print(current_pin+11);
//        Serial.println(" pressed!");
      }
      if (press[current_pin+11] and button_data[current_pin+11] == LOW) {
        press[current_pin+11] = false;
//        Serial.print("Button ");
//        Serial.print(current_pin+11);
//        Serial.println(" released!");
      }
    }

    // Чиаем аналоговые сигналы с джойстиков
    right_ox = analogRead(A5);
    right_oy = analogRead(A4);
    left_ox = analogRead(A7);
    left_oy = analogRead(A6);

    uint16_t buttons_state_result = 0;
    for (int8_t i = 0; i <= 14; i++) {
      if (press[i]) {
        buttons_state_result = buttons_state_result + sq_list[i];
      }
    }
//    Serial.print("    ");
//    Serial.print(buttons_state_result);

    // Полученную инфу с кнопок складываем в массив
//    for (uint8_t i = 0; i < 15; i++) {
//      result[i] = press[i];
//    }
//    char result_char[21];
    String result_char = "";
//    itoa(buttons_state_result, result_char, 10);
//    strcat(result_char, ";");
    result_char += "$" + String(buttons_state_result) + ";";
    uint8_t lst[4] = {right_ox, right_oy, left_ox, left_oy};
    for (uint8_t i: lst) {
//      char buf[4];
//      itoa(i, buf, 10);
//      strcat(buf, ";");
//      strcat(result_char, buf);
        result_char += String(i) + ";";
    }
    result_char += "^";
//    result[1] = right_ox;
//    result[2] = right_oy;
//    result[3] = left_ox;
//    result[4] = left_oy;

    // Передаем данные через UART
    if (Serial.available()) {
      char buf[28];
      result_char.toCharArray(buf, result_char.length()+1);
      //Serial.println(result_char);
      Serial.write(buf);
//      Serial.println();
      while (Serial.available()) {
        Serial.read();
      }
    }

    // Раскодировка
//    bool res[15] = {false, false, false, false, false, false, false, false, false, false, false, false, false, false};;
//    for (int8_t i = 14; i >= 0; i--) {
//      if (buttons_state_result > sq_list[i]) {
//        res[i] = true;
//        buttons_state_result = buttons_state_result - sq_list[i];
//      }
//      else if (buttons_state_result == sq_list[i]) {
//        res[i] = true;
//        break;
//      }
//    }
//    Serial.print("    ");
//    for (int8_t i = 0; i <= 14; i++) {
//      Serial.print(res[i]?"1":"0");
//    }
//    Serial.println();
}

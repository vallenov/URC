int button_data[15];
bool press[15] = {false, false, false, false, false, false, false, false, false, false, false, false, false, false, false};
int right_ox, right_oy, left_ox, left_oy;
int left_l1, left_l2, right_l1, right_l2;

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
}

void loop() {
    // Читаем инфу с цифровфх пинов
    for (uint8_t current_pin = 0; current_pin <= 10; current_pin++) {
      button_data[current_pin] = !digitalRead(current_pin+2);
      if (button_data[current_pin] == HIGH) {
        press[current_pin] = true;
        Serial.print("Button ");
        Serial.print(current_pin);
        Serial.println(" pressed!");
      }
      if (press[current_pin] and button_data[current_pin] == LOW) {
        press[current_pin] = false;
        Serial.print("Button ");
        Serial.print(current_pin);
        Serial.println(" released!");
      }
    }

    // Читаем аналоговые, которые выполняют роль цифровых
    uint8_t abutton_data[4] = {A0, A1, A2, A3};
    for (uint8_t current_pin = 0; current_pin <= 3; current_pin++) {
      button_data[current_pin+11] = !analogRead(abutton_data[current_pin]);
      if (button_data[current_pin+11] == HIGH) {
        press[current_pin + 11] = true;
        Serial.print("Button ");
        Serial.print(current_pin+11);
        Serial.println(" pressed!");
      }
      if (press[current_pin+11] and button_data[current_pin+11] == LOW) {
        press[current_pin+11] = false;
        Serial.print("Button ");
        Serial.print(current_pin+11);
        Serial.println(" released!");
      }
    }

    // Чиаем аналоговые сигналы с джойстиков
    right_ox = analogRead(A5);
    right_oy = analogRead(A4);
    left_ox = analogRead(A7);
    left_oy = analogRead(A6);
}

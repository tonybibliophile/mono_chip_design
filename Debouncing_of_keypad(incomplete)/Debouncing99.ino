const byte colPins[4] = {13, 12, 11, 10};
const byte rowPins[4] = {A0, A1, A2, A3};
const char keymap[4][4] = {
  {'1', '2', '3', 'A'},
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'*', '0', '#', 'D'}
};

unsigned long debounceTimes[4][4];
bool lastButtonState[4][4];

void setup() {
  Serial.begin(115200);

  // 設定列腳位
  for (int c = 0; c < 4; c++) {
    pinMode(colPins[c], OUTPUT);
    digitalWrite(colPins[c], HIGH);
  }

  // 設定行腳位
  for (int r = 0; r < 4; r++) {
    pinMode(rowPins[r], INPUT_PULLUP);
  }

  // 初始化狀態
  for (int r = 0; r < 4; r++) {
    for (int c = 0; c < 4; c++) {
      lastButtonState[r][c] = false;
      debounceTimes[r][c] = 0;
    }
  }
}

void loop() {
  for (int c = 0; c < 4; c++) {
    digitalWrite(colPins[c], LOW); // 啟用當前列

    for (int r = 0; r < 4; r++) {
      bool currentState = !digitalRead(rowPins[r]); // 按下 = true

      if (currentState != lastButtonState[r][c]) {
        debounceTimes[r][c] = millis();
      }

      if (millis() - debounceTimes[r][c] > 20) {
        if (currentState != lastButtonState[r][c]) {
          lastButtonState[r][c] = currentState;
          if (currentState) {
            Serial.println(keymap[r][c]);
          }
        }
      }
    }

    digitalWrite(colPins[c], HIGH); // 釋放當前列
  }
}

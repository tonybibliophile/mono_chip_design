#include <Keypad.h>

#define KEY_ROWS 4
#define KEY_COLS 4

// 鍵盤設定
const byte colPins[4] = {13, 12, 11, 10}; 
const byte rowPins[4] = {A0, A1, A2, A3}; 
const char keymap[KEY_ROWS][KEY_COLS] = { 
  {'1', '2', '3', 'A'},
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'*', '0', '#', 'D'}
};
Keypad myKeypad = Keypad(makeKeymap(keymap), rowPins, colPins, KEY_ROWS, KEY_COLS);

// 七段顯示器設定
const byte segmentPins[7] = {2, 3, 4, 5, 6, 7, 8};
const byte digitPins[3] = {9, A4, A5}; 
const byte segmentPatterns[10][7] = {
  {1, 1, 1, 1, 1, 1, 0}, // 0
  {0, 1, 1, 0, 0, 0, 0}, // 1
  {1, 1, 0, 1, 1, 0, 1}, // 2
  {1, 1, 1, 1, 0, 0, 1}, // 3
  {0, 1, 1, 0, 0, 1, 1}, // 4
  {1, 0, 1, 1, 0, 1, 1}, // 5
  {1, 0, 1, 1, 1, 1, 1}, // 6
  {1, 1, 1, 0, 0, 0, 0}, // 7
  {1, 1, 1, 1, 1, 1, 1}, // 8
  {1, 1, 1, 1, 0, 1, 1}  // 9
};

// 狀態列舉
enum State {
  PASSWORD_INPUT,     
  OLD_PASSWORD,
  NEW_PASSWORD,   
  VERIFICATION,        
  UNLOCKED               
};

// 全域變數
char currentPassword[9] = "20051209";    // 當前密碼
char buffer[9];                          // 暫存密碼輸入（8 位 + '\0'）
char verifyBuffer[4];                    // 暫存驗證碼輸入（3 位 + '\0'）
int buffer_index = 0;                    // 密碼輸入索引
int verify_index = 0;                    // 驗證碼輸入索引
int count_wrong = 0;                     // 錯誤計數
int verificationCode = 0;                // 當前驗證碼
char tempBuffer[9];                      // 暫存密碼（等待驗證碼確認）
State currentState = PASSWORD_INPUT;      // 當前狀態
State verifyForState = PASSWORD_INPUT;    // 驗證碼對應的密碼狀態
//邏輯：用verifyForState確認驗證對象
void setup() {
  Serial.begin(9600);
  randomSeed(analogRead(0)); // 使用 A6 初始化隨機數
  delay(100); // 僅在 setup 中使用短暫延遲
  Serial.println("Please enter password");

  // 初始化七段顯示器腳位
  for (int i = 0; i < 7; i++) {
    pinMode(segmentPins[i], OUTPUT);
    digitalWrite(segmentPins[i], LOW);
  }
  for (int i = 0; i < 3; i++) {
    pinMode(digitPins[i], OUTPUT);
    digitalWrite(digitPins[i], HIGH); // 共陰極，預設關閉
  }
}

void loop() {
  if (currentState == UNLOCKED) {
    displayNumber(666);
    return;
  }

  char key = myKeypad.getKey();
  if (key) {
    switch (currentState) {
      case PASSWORD_INPUT:
        handlePasswordInput(key);
        break;
      case OLD_PASSWORD:
        handleOldPassword(key);
        break;
      case NEW_PASSWORD:
        handleNewPassword(key);
        break;
      case VERIFICATION:
        handleVerification(key);
        break;
    }
  }
  displayNumber(verificationCode); 
}

// 處理普通密碼輸入
void handlePasswordInput(char key) {
  if (key >= '0' && key <= '9') {
    if (buffer_index < 8) {
      buffer[buffer_index++] = key;
      Serial.print("輸入: ");
      Serial.println(key);
    } else {
      Serial.println("Max length reached!");
    }
  } else if (key == '*') {
    if (buffer_index == 8) {
      buffer[buffer_index] = '\0';
      Serial.print("提交密碼: ");
      Serial.println(buffer);
      // 簡單迴圈複製，確保正確
      for (int i = 0; i < 8; i++) {
        tempBuffer[i] = buffer[i];
      }
      tempBuffer[8] = '\0';
      buffer_index = 0;
      clearBuffer();
      verificationCode = random(100, 1000);
      Serial.println("Enter verification code:");
      currentState = VERIFICATION;
      verifyForState = PASSWORD_INPUT;
    } else {
      Serial.println("Password must be 8 digits!");
      buffer_index = 0;
      clearBuffer();
      Serial.println("Please enter password");
    }
  } else if (key == '#') {
    buffer_index = 0;
    clearBuffer();
    Serial.println("re-enter password:");
  } else if (key == 'A' || key == 'B' || key == 'C') {
    Serial.println("illegal input");
  } else if (key == 'D') {
    currentState = OLD_PASSWORD;
    Serial.println("Enter old password:");
    buffer_index = 0;
    clearBuffer();
  }
}


void handleOldPassword(char key) {
  if (key >= '0' && key <= '9') {
    if (buffer_index < 8) {
      buffer[buffer_index++] = key;
      Serial.print("輸入: ");
      Serial.println(key);
    } else {
      Serial.println("Max length reached!");
    }
  } else if (key == '*') {
    if (buffer_index == 8) {
      buffer[buffer_index] = '\0';
      Serial.print("提交舊密碼: ");
      Serial.println(buffer);
      for (int i = 0; i < 8; i++) {
        tempBuffer[i] = buffer[i];
      }
      tempBuffer[8] = '\0';
      buffer_index = 0;
      clearBuffer();
      verificationCode = random(100, 1000);
      Serial.println("Enter verification code:");
      currentState = VERIFICATION;
      verifyForState = OLD_PASSWORD;
    } else {
      Serial.println("Password must be 8 digits!");
      buffer_index = 0;
      clearBuffer();
      Serial.println("Enter old password:");
    }
  } else if (key == '#') {
    currentState = PASSWORD_INPUT;
    buffer_index = 0;
    clearBuffer();
    Serial.println("re-enter password:");
  } else if (key == 'A' || key == 'B' || key == 'C') {
    Serial.println("illegal input");
  }
}

// 處理新密碼輸入
void handleNewPassword(char key) {
  if (key >= '0' && key <= '9') {
    if (buffer_index < 8) {
      buffer[buffer_index++] = key;
      Serial.print("輸入: ");
      Serial.println(key);
    } else {
      Serial.println("Max length reached!");
    }
  } else if (key == '*') {
    if (buffer_index == 8) {
      buffer[buffer_index] = '\0';
      Serial.print("提交新密碼: ");
      Serial.println(buffer);
      for (int i = 0; i < 8; i++) {
        tempBuffer[i] = buffer[i];
      }
      tempBuffer[8] = '\0';
      buffer_index = 0;
      clearBuffer();
      verificationCode = random(100, 1000);
      Serial.println("Enter verification code:");
      currentState = VERIFICATION;
      verifyForState = NEW_PASSWORD;
    } else {
      Serial.println("New password must be 8 digits!");
      buffer_index = 0;
      clearBuffer();
      Serial.println("Enter new password:");
    }
  } else if (key == '#') {
    currentState = PASSWORD_INPUT;
    buffer_index = 0;
    clearBuffer();
    Serial.println("re-enter password:");
  } else if (key == 'A' || key == 'B' || key == 'C') {
    Serial.println("illegal input");
  }
}

// 處理驗證碼輸入
void handleVerification(char key) {
  if (key >= '0' && key <= '9') {
    if (verify_index < 3) {
      verifyBuffer[verify_index++] = key;
      Serial.print("plz type in verification code: ");
      Serial.println(key);
    } else {
      Serial.println("Max length reached!");
    }
  } else if (key == '*') {
    if (verify_index == 3) {
      verifyBuffer[verify_index] = '\0';
      Serial.print("submit verfication: ");
      Serial.println(verifyBuffer);
      int inputCode = atoi(verifyBuffer);
      if (inputCode == verificationCode) {
        Serial.println("Verification correct!");
        Serial.print("check password: ");
        Serial.println(tempBuffer);
        if (verifyForState == OLD_PASSWORD) {
          if (strcmp(tempBuffer, currentPassword) == 0) {
            Serial.println("Old password correct. Enter new password:");
            currentState = NEW_PASSWORD;
            buffer_index = 0;
            clearBuffer();
          } else {
            Serial.println("Old password incorrect!");
            currentState = PASSWORD_INPUT;
            buffer_index = 0;
            clearBuffer();
            Serial.println("Please enter password");
          }
        } else if (verifyForState == NEW_PASSWORD) {
          for (int i = 0; i < 8; i++) {
            currentPassword[i] = tempBuffer[i];
          }
          currentPassword[8] = '\0';
          Serial.println("Password updated!");
          currentState = PASSWORD_INPUT;
          buffer_index = 0;
          clearBuffer();
          Serial.println("Please enter password");
        } else {
          if (strcmp(tempBuffer, currentPassword) == 0) {
            Serial.println("Unlock!");
            count_wrong = 0;
            currentState = UNLOCKED;
          } else {
            Serial.println("Wrong passwords!");
            count_wrong++;
            if (count_wrong == 3) {
              Serial.println("forget the password?");
              count_wrong = 0;
            }
            currentState = PASSWORD_INPUT;
            buffer_index = 0;
            clearBuffer();
            Serial.println("Please enter password");
          }
        }
        verify_index = 0;
        clearVerifyBuffer();
      } else {
        Serial.println("Verification incorrect!");
        verificationCode = random(100, 1000);
        Serial.println("Enter verification code:");
        verify_index = 0;
        clearVerifyBuffer();
      }
    } else {
      Serial.println("Verification code must be 3 digits!");
      verify_index = 0;
      clearVerifyBuffer();
      Serial.println("Enter verification code:");
    }
  } else if (key == '#') {
    verify_index = 0;
    clearVerifyBuffer();
    Serial.println("re-enter verification code:");
  } else if (key == 'A' || key == 'B' || key == 'C') {
    Serial.println("illegal input");
  }
}

// 顯示數字到七段顯示器
void displayNumber(int number) {
  if (number < 100 || number > 999) return;
  int digits[3];
  digits[0] = number / 100;
  digits[1] = (number / 10) % 10;
  digits[2] = number % 10;

  for (int i = 0; i < 3; i++) {
    for (int j = 0; j < 3; j++) {
      digitalWrite(digitPins[j], HIGH);
    }
    for (int j = 0; j < 7; j++) {
      digitalWrite(segmentPins[j], segmentPatterns[digits[i]][j]);
    }
    digitalWrite(digitPins[i], LOW);
    delay(5);
  }
}

// 清空密碼緩衝區
void clearBuffer() {
  for (int i = 0; i < 9; i++) {
    buffer[i] = '\0';
  }
}

// 清空驗證碼緩衝區
void clearVerifyBuffer() {
  for (int i = 0; i < 4; i++) {
    verifyBuffer[i] = '\0';
  }
}

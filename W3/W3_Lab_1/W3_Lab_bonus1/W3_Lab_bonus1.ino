#include <Keypad.h>
#include <Password.h>

#define KEY_ROWS 4
#define KEY_COLS 4

// Keypad設定
const byte colPins[4] = {A3,A1,A2,A0};
const byte rowPins[4] = {13,9,A5,A4};

const char keymap[KEY_ROWS][KEY_COLS] = { 
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'}
};

Keypad myKeypad = Keypad(makeKeymap(keymap), rowPins, colPins, KEY_ROWS, KEY_COLS);

// 全域變數
Password password = Password("20051209");  // 預設密碼
char buffer[8];                             // 暫存輸入
const char show[] = "Please enter password";
int buffer_index1 = 0;
int count_wrong = 0;
bool unlocked = false;                      // 是否解鎖
bool verify = false;
int verificationCode;

int buffer_ver=0;
int buffer_index_ver = 0;


void setup() {
  randomSeed(analogRead(0));
  Serial.begin(9600);
  delay(100);
  Serial.println(show);
}

void loop() {
  if (unlocked) return;  // 解鎖後不再讀輸入

  char key = myKeypad.getKey();
  if (verify){
    if(key){
      if (buffer_index_ver==3){
        //if()
        True_OR_False(buffer_index1);
        verify=false;
        return;
      }
      if (key >= '0' && key <= '9') {
        buffer_ver += atoi(key)*pow(10,buffer_index_ver);
        buffer_index_ver++;
        Serial.println(key); // 顯示星號
      } 
    }
  }
  else{
    if (key){
    // 數字鍵輸入
    if (key >= '0' && key <= '9') {
      if (buffer_index1 < 8) {
        buffer[buffer_index1] = key;
        buffer_index1++;
        Serial.println(key); // 顯示星號
      } else {
        Serial.println("Max length reached!");
      }
    } 
    // 驗證密碼
    else if (key == '*') {  
      //buffer[buffer_index1] = '\0';   // 補上字串結尾
      verify = true; //進入驗證模式
      verificationCode = random(100, 1000);
    } 
    // 重置輸入
    else if (key == '#') {
      buffer_index1 = 0;
      clearBuffer();
      Serial.println("\nre-enter password:");
    } 
    // 非法輸入
    else if (key == 'A' || key == 'B' || key == 'C' || key == 'D') {
      Serial.println("illegal input");
    }
  }
  }
  
}

// 驗證密碼函式
void True_OR_False(int buffer_index) {
  password.reset();
  for(int i = 0; i < buffer_index; i++) 
    password.append(buffer[i]);

  if (password.evaluate()) {
    Serial.println("Unlock!");
    count_wrong = 0;
    unlocked = true; // 解鎖後不再輸入
    buffer_index1 = 0;
    clearBuffer();
  } else {
    Serial.println("Wrong passwords!");
    count_wrong++;
    buffer_index1 = 0;
    clearBuffer();
    if (count_wrong == 3) {
      Serial.println("forget the password?");
      count_wrong = 0;
    }
  }
  password.reset();
}

// 清空 buffer
void clearBuffer() {
  for (int i = 0; i < 8; i++)
  buffer[i] = '\0';
}

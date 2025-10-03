#include <Keypad.h> //引用Keypad函式庫
#define KEY_ROWS 4 //按鍵模組的列數
#define KEY_COLS 4 //按鍵模組的行數
String buf ="";
const int SEG_COM = 9;
const int SEG_data[10][8] = {
  {1, 1, 1, 1, 1, 1, 0, 0},  // "0"  
  {0, 1, 1, 0, 0, 0, 0, 0},  // "1"  
  {1, 1, 0, 1, 1, 0, 1, 0},  // "2"  
  {1, 1, 1, 1, 0, 0, 1, 0},  // "3"  
  {0, 1, 1, 0, 0, 1, 1, 0},  // "4"  
  {1, 0, 1, 1, 0, 1, 1, 0},  // "5"  
  {1, 0, 1, 1, 1, 1, 1, 0},  // "6"  
  {1, 1, 1, 0, 0, 0, 0, 0},  // "7"  
  {1, 1, 1, 1, 1, 1, 1, 0},  // "8"  
  {1, 1, 1, 1, 0, 1, 1, 0}   // "9"  
};

const byte colPins[4] = {13, 12, 11, 10};
const byte rowPins[4] = {A0, A1, A2, A3};
const char keymap[KEY_ROWS][KEY_COLS] = {
    {'1','2','3','A'}, 
    {'4','5','6','B'},
    {'7','8','9','C'},
    {'*','0','#','D'}
};
Keypad myKeypad = Keypad(makeKeymap(keymap),rowPins,colPins,KEY_ROWS,KEY_COLS); // 設置keypad 

void setup(){
  Serial.begin(115200);

  for (int i = 2; i <= 9; i++) { //設置顯示器
    pinMode(i, OUTPUT);  // 設置腳位為輸出
    digitalWrite(i, HIGH);  // 設置為高電位
  }
}
int lastNum = -1;  // 預設沒有數字

void loop(){
  char key = myKeypad.getKey();

  if (key) {
    if (key >= '0' && key <= '9') {
      lastNum = key - '0';  // 更新要顯示的數字
      Serial.println(lastNum);
    }
  }

  if (lastNum >= 0) {
    digitalWrite(SEG_COM, LOW);  
    SEG_Drive(lastNum);  
    delay(5);
    digitalWrite(SEG_COM, HIGH);
  }
}

void SEG_Drive(char number){  
  for(int i=2;i<10;i++)
  {
    digitalWrite(i,SEG_data[number][i-2]);
  }

}
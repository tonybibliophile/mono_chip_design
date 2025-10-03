#include <Keypad.h> //引用Keypad函式庫
#define KEY_ROWS 4 //按鍵模組的列數
#define KEY_COLS 4 //按鍵模組的行數

const byte colPins[4] = {13, 12, 11, 10};
const byte rowPins[4] = {A0, A1, A2, A3};
const char keymap[KEY_ROWS][KEY_COLS] = {
    {'1','2','3','A'}, 
    {'4','5','6','B'}, 
    {'7','8','9','C'},
    {'*','0','#','D'}
};
Keypad myKeypad = Keypad(makeKeymap(keymap),rowPins,colPins,KEY_ROWS,KEY_COLS);
//declare buffer
String buffer="";

void setup(){
    Serial.begin(115200);
}
void loop(){
  char key = myKeypad.getKey();
  if (key != NO_KEY){
    if (key=='#'){
      Serial.println(buffer);
      buffer= "";
    }

    else {
      Serial.println(key);
      buffer+= key;
    }
    
  }
  
}
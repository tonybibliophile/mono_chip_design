#include <Keypad.h>

#define ROWS 4
#define COLS 4

const byte rowPins[ROWS] = {2, 3, 4, 5};
const byte colPins[COLS] = {6, 7, 8, 9};

const char keymap[ROWS][COLS] = {
  {'1', '2', '3', '+'},
  {'4', '5', '6', '-'},
  {'7', '8', '9', 'x'},
  {'$', '0', '=', '/'}
};

Keypad myKeypad = Keypad(makeKeymap(keymap), rowPins, colPins, ROWS, COLS);

String usrInput[2] = {"",""};
char myOperator = '?';
int idx = 0;

void setup(){
  Serial.begin(9600);
}

void loop(){
  char key = myKeypad.getKey();

  // ---- 輸入數字（整數） ----
  if(key != NO_KEY && key >= '0' && key <= '9'){
    usrInput[idx] += key;
    Serial.print(key);
  }

  // ---- 輸入運算符號 ----
  if(key != NO_KEY && (key=='+' || key=='-' || key=='x' || key=='/')){
    if(idx == 0 && usrInput[0] != ""){
      myOperator = key;
      idx = 1;
      Serial.print(key);
    }
  }

  // ---- 等號，計算結果 ----
  if(key == '='){
    Serial.print(key);

    if(usrInput[0] != "" && usrInput[1] != "" && myOperator != '?'){
      float num1 = usrInput[0].toFloat();
      float num2 = usrInput[1].toFloat();
      float result = 0;

      switch(myOperator){
        case '+': result = num1 + num2; break;
        case '-': result = num1 - num2; break;
        case 'x': result = num1 * num2; break;
        case '/': 
          if(num2 != 0) result = num1 / num2;
          else {
            Serial.println("Error: Divide by 0");
            result = NAN;
          }
          break;
      }

      if(!isnan(result)){
        Serial.print(" => ");
        Serial.println(result, 4); // 顯示最多4位小數
      }
    }

    // 重置
    usrInput[0] = "";
    usrInput[1] = "";
    myOperator = '?';
    idx = 0;
  }

  // ---- 重置符號 ----
  if(key == '$'){
    usrInput[0] = "";
    usrInput[1] = "";
    myOperator = '?';
    idx = 0;
    Serial.println("\nReset Done");
  }
}

const byte colPins[4] = {2, 3, 4, 5};  //行腳位
const byte rowPins[4] = {A0, A1, A2, A3};  //列腳位
const char keymap[4][4] = {{'1', '2', '3', 'A'},  //Keypad對應符號
                           {'4', '5', '6' ,'B'},
                           {'7', '8', '9', 'C'},
                           {'*', '0', '#', 'D'}};

void setup() {
  Serial.begin(115200);
  for(int i=0; i<=3; i++){
    pinMode(rowPins[i], INPUT_PULLUP);
    pinMode(colPins[i], OUTPUT);
    digitalWrite(colPins[i], HIGH);
  }
}

void loop() {
  for(int i=0; i<=3; i++){
    for(int j=0; j<=3; j++){
      digitalWrite(colPins[i], LOW);
      if(analogRead(rowPins[j])<=512){
        Serial.println(keymap[j][i]);
        delay(300);
      }
    }
    digitalWrite(colPins[i], HIGH);
  }
}

const int Led[5] = {8, 9, 10, 11, 12}; // LED陣列，儲存五個LED燈的連接腳位
const int sw_up = 2;    // 按鈕1，連接到數位腳位2
int pos = 0;  // 初始位置，指示哪個LED燈亮起，從1開始

void setup() {
// 初始化所有LED腳位為輸出模式，並設置為低電位（關閉）
for (int i = 0; i < 5; i++) {
  pinMode(Led[i], OUTPUT);   // 設置LED腳位為輸出
  digitalWrite(Led[i], LOW); // 初始化為關閉狀態
}
Serial.begin(9600);
// 設置初始位置的LED為開啟狀態
digitalWrite(Led[pos], HIGH); // 打開初始位置的LED
digitalWrite(Led[(pos+1)%5], HIGH); // 打開緊接在初始位置的下一個LED
pinMode(sw_up, INPUT_PULLUP);  // 設置按鈕1為輸入並啟用內建上拉電阻
}

void loop() {
bool swstate_up = digitalRead(sw_up);    
// 讀取按鈕1的狀態（HIGH表示未按下，LOW表示按下）

// 按下按鈕1（數位腳位2，內建上拉電阻），LED向左移動
if (swstate_up == LOW) {
  pos++;
  Serial.println((pos)%5);
  Serial.println((pos-2)%5);
  Serial.println("----------");
  digitalWrite(Led[(pos)%5], HIGH); // 打開初始位置的LED
  digitalWrite(Led[(pos-2)%5], LOW); 
  delay(500);
} 
}


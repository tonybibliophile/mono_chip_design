const int button = 2;       // 定義按鈕連接的數位腳位為2
const int led = 13;         // 定義內建LED連接的數位腳位為13
bool led_state = HIGH;      // LED的初始狀態設為HIGH（開啟LED）
unsigned long lastDebounceTime = 0; // 儲存上次按鈕狀態變化的時間
unsigned long debounceDelay = 50;   // de-bouncing延遲時間，單位為毫秒（防止按鈕抖動影響）
bool lastButtonState = HIGH;        // 儲存上次按鈕狀態的值，初始為HIGH（未按下）
bool buttonState;                   // 儲存當前按鈕狀態的值

void setup() {
pinMode(button, INPUT_PULLUP);  // 設置按鈕腳位為上拉輸入模式（默認為HIGH，按鈕按下時為LOW）
pinMode(led, OUTPUT);           // 設置LED腳位為輸出模式
digitalWrite(led, led_state);   // 設置LED的初始狀態（根據led_state變數）
}

void loop() {
// 讀取按鈕當前的電位（HIGH或LOW）
int reading = digitalRead(button);

// 如果按鈕狀態發生變化（按鈕被按下或釋放），重置去de-bouncing計時器
if (reading != lastButtonState) {
  lastDebounceTime = millis(); // 更新去抖動計時器為當前時間
}

// 如果當前時間與上次按鈕狀態變化時間的差超過去抖動延遲時間
if ((millis() - lastDebounceTime) > debounceDelay) {
  // 按鈕狀態已經穩定
  if (reading != buttonState) {
    buttonState=reading;
    
    if (buttonState==LOW)
    {
    led_state=!led_state;
    digitalWrite(led, led_state);
    }
  }
}

// 記錄當前按鈕狀態，以備下次迴圈使用
lastButtonState = reading; // 更新上次按鈕狀態為當前狀態
}

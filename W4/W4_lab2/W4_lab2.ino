#include <BluetoothSerial.h> // 引入Bluetooth函式庫

BluetoothSerial BT; // 宣告一個藍牙通訊物件，名字為BT
String msg = "";    // 儲存透過藍牙接收到的訊息
#define LED_PIN 2   // ESP32上面小顆藍色LED的腳位

#define GROUP_ID 2  // 請將問號改成你們的組別數字 

void setup(){
  // 定義Baud rate
  Serial.begin(9600);
  pinMode(LED_PIN, OUTPUT);
  // 設定藍牙名稱為ESP32_?(根據自己的組別id)，並啟用藍牙
  BT.begin("ESP32_"+String(GROUP_ID));
}

void loop(){
  if(BT.available()){
    msg = BT.readString();
    msg.trim();
    Serial.println(msg);
    // 判斷msg是否為hi，若是則在手機的bluetooth terminal印出句子"Group id is ???"
    if(msg == "hi"){
      // Add your code
      BT.println("Our group id is 2");
    }
    // 判斷msg是否為on，若是則點亮NodeMCU32s上面的藍色LED燈
    // 並在手機的bluetooth terminal印出句子"LED is turned on"
    else if(msg == "on"){
      // Add your code
      digitalWrite(LED_PIN,HIGH);
    }
    // 判斷msg是否為off，若是則熄滅NodeMCU32s上面的藍色LED燈
    // 並在手機的bluetooth terminal印出句子"LED is turned off"
    else if(msg == "off"){
      // Add your code
      digitalWrite(LED_PIN,LOW);
    }
    // 判斷msg是否為blink，若是則讓NodeMCU32s上面的藍色LED燈閃爍5次(亮暗間隔1s)
    // 並在手機的bluetooth terminal印出句子"LED is blinking"
    else if(msg == "blink"){
      // Add your code
      for (int i;i<5;i++)
      {
        digitalWrite(LED_PIN,HIGH);
        delay(1000);
        digitalWrite(LED_PIN,LOW);
        delay(1000);
      }
    }
    // 若msg都不是上面的字串，在手機的bluetooth terminal印出"Sorry :("
    else {
      // Add your code
      BT.println("Sorry");
    }
  }
    // ===== 電腦 Serial Monitor -> ESP32 -> 手機 (聊天) =====
    if(Serial.available()){
      String input = Serial.readString(); 
      BT.println(input);
      // 移除多餘換行
      // Add your code
      // 傳給手機
      // 自己電腦端確認bl
    }
}
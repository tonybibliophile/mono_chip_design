#include <Wire.h>        // 引入 Wire 函式庫，用於 I2C 通信
#include <I2Cdev.h>      // 引入 I2Cdev 函式庫，用於簡化 I2C 設備的操作
#include <MPU6050.h>     // 引入 MPU6050 函式庫，用於操作 MPU6050 傾斜感測器
#include <math.h>        // 引入 math 函式庫，用於數學計算，如角度轉換

//友善提醒:接線要注意，如果確定程式碼沒問題，極度建議把杜邦線換掉試試看，晃動過程中很容易訊號不穩
const int SEG_COM[4] = {10, 11, 12, 13};  //控制線
const int SEG_data[10][8] = {{1, 1, 1, 1, 1, 1, 0, 0},  //"0"  //資料線
                      {0, 1, 1, 0, 0, 0, 0, 0},  //"1"
                      {1, 1, 0, 1, 1, 0, 1, 0},  //"2"
                      {1, 1, 1, 1, 0, 0, 1, 0},  //"3"
                      {0, 1, 1, 0, 0, 1, 1, 0},  //"4"
                      {1, 0, 1, 1, 0, 1, 1, 0},  //"5"
                      {1, 0, 1, 1, 1, 1, 1, 0},  //"6"
                      {1, 1, 1, 0, 0, 0, 0, 0},  //"7"
                      {1, 1, 1, 1, 1, 1, 1, 0},  //"8"
                      {1, 1, 1, 0, 0, 1, 1, 0}};  //"9"
                      

int disp[4] = {0, 0, 0, 0};  //欲顯示數字

// MPU-6050設定
MPU6050 accelgyro; // 創建一個 MPU6050 類別的實例，預設 I2C 位址為 0x68
int16_t ax, ay, az, gx, gy, gz; // 定義變數來存儲加速度計和陀螺儀的數據

// 全域變數設定
 // 儲存上一次計算的 X 軸角度
 // 儲存上一次計算的 Y 軸角度
 // 儲存上一次計算的 Z 軸角度
 // 設定 xy 平面上的角度變化閾值 (度)
 // 設定 z 軸上的角度變化閾值 (度)
int stepCount = 0;          // 記錄步數的計數器
bool stepDetected = false;  // 標記是否已檢測到步數
float previousAngleX;
float previousAngleY;
float previousAngleZ;
unsigned long time;
unsigned long previous_time;
void setup() {
  for(int i=2; i<=13; i++){
    pinMode(i, OUTPUT);
    digitalWrite(i, HIGH);  //共陽極
  }
  Serial.begin(115200); // 設置串口通信的波特率為 115200
  accelgyro.initialize(); // 初始化 MPU6050 感測器

  // 設定加速度計的範圍為 8g。此時範圍為 -16g 到 16g，每 g 對應 4096 單位
  accelgyro.setFullScaleAccelRange(MPU6050_ACCEL_FS_8);
}

void loop() {
  // 獲取 MPU6050 感測器的六個數據：三個加速度和三個角速度
  accelgyro.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);
  time=millis();
  // 計算加速度大小並轉換為 g 值
  // 設定為 8g 時，每 g 對應 4096 單位，因此需要除以 4096
  float accelX = ax / 4096.0; // X 軸加速度 (g)
  float accelY = ay / 4096.0; // Y 軸加速度 (g)
  float accelZ = az / 4096.0; // Z 軸加速度 (g)

  // 使用 arctangent 函數計算角度值
  // angleX: X 軸的角度，通過 Y 和 Z 軸的加速度計算
  // angleY: Y 軸的角度，通過 X 和 Z 軸的加速度計算
  // angleZ: Z 軸的角度，通過 X 和 Y 軸的加速度計算
  float angleX = atan2(accelX, sqrt(accelY * accelY + accelZ * accelZ)) * 180 / PI;
  float angleY = atan2(accelY, sqrt(accelX * accelX + accelZ * accelZ)) * 180 / PI;
  float angleZ = atan2(sqrt(accelX * accelX + accelY * accelY), accelZ) * 180 / PI;
  
  // 打印角度值以供調試
  /*
  Serial.print("Angle X: ");
  Serial.print(angleX); // 打印 X 軸角度
  Serial.print(" Angle Y: ");
  Serial.print(angleY); // 打印 Y 軸角度
  Serial.print(" Angle Z: ");
  Serial.println(angleZ); // 打印 Z 軸角度
  */
  for(int i=0; i<=3; i++){  //個、十、百、千位數
    digitalWrite(SEG_COM[i], LOW);  //觸發第i位數顯示數字
    SEG_Drive(disp[i]);  //資料線寫入數值
    delay(5);
    digitalWrite(SEG_COM[i], HIGH);  //結束觸發第i位數
  }
  // 判斷是否超過閾值來檢測步數
  // 如果步數尚未檢測到，並且角度變化超過設定的閾值，則認為檢測到一步
  if ((time-previous_time)>200){
    if ((angleX-previousAngleX+angleY-previousAngleY+angleZ-previousAngleZ)>50 && stepDetected == false) {
    stepCount++; // 增加步數計數器
    Serial.print("Step Count: ");
    Serial.println(stepCount); // 打印步數
    disp[0] =stepCount/1000;
    disp[1] =(stepCount-disp[0]*1000)/100;
    disp[2] =(stepCount-disp[0]*1000-disp[1]*100)/10;
    disp[3] =(stepCount-disp[0]*1000-disp[1]*100-disp[2]*10);
  }

  // 如果角度變化回到小於閾值，重置 stepDetected
  // 這樣可以檢測下一次步數
    if ((angleX-previousAngleX+angleY-previousAngleY+angleZ-previousAngleZ)<40 && stepDetected == true) {
      stepDetected = false; // 重置步數檢測標記
  }
  }
  

  // 更新前一次的角度值，以便進行下一次的角度變化檢測
  previousAngleX = angleX;
  previousAngleY = angleY;
  previousAngleZ = angleZ;
 // 等待 100 毫秒，這個延遲可以調整以適應實際需要
}

void SEG_Drive(char number){  //將字元變數從SEG_data[][]找到相對應的位置，並寫入a~g中
  for(int i=0;i<8;i++)
  {
    digitalWrite(i+2,SEG_data[number][i]);
  }


}



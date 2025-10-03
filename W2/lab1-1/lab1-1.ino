int Led[5] = {8, 9, 10, 11, 12}; // 使用迴圈依序設定腳位8～12都是輸出功能

void setup()  
{
  for (int i = 0; i < 5; i++)  // 整數變數i由0變化到4，每次加1
  {
  pinMode(Led[i], OUTPUT);  // 設定i變數所指定的腳位為輸出功能
  }
}
 
void loop()
{
  for (int i=0;i<5;i++)
  {
    digitalWrite(Led[i], HIGH);  //第8腳位輸出高電位，點亮LED
    delay(1000);  //等待1000ms  
  }
  for (int j=0;j<5;j++)
  {
    digitalWrite(Led[j],LOW);  //第8腳位輸出高電位，點亮LED
  } 
  delay(1000);
  for (int i=4;i>=0;i--)
    {
      digitalWrite(Led[i], HIGH);  //第8腳位輸出高電位，點亮LED
      delay(1000);  //等待1000ms  
    }
    for (int j=0;j<5;j++)
  {
    digitalWrite(Led[j],LOW);  //第8腳位輸出高電位，點亮LED
  } 
  delay(1000);
  
}
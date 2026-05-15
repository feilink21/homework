// 定义LED引脚
const int ledPin = 13;

void setup() {
  // 设置引脚为输出模式
  pinMode(ledPin, OUTPUT);
}

void loop() {
  // ====================
  // SOS 摩斯电码闪烁逻辑
  // ====================

  // S: 短闪 3 次（点 ···）
  for(int i=0; i<3; i++) {
    digitalWrite(ledPin, HIGH);
    delay(200);
    digitalWrite(ledPin, LOW);
    delay(200);
  }
  delay(500);  // 字母间隔

  // O: 长闪 3 次（划 ---）
  for(int i=0; i<3; i++) {
    digitalWrite(ledPin, HIGH);
    delay(600);
    digitalWrite(ledPin, LOW);
    delay(200);
  }
  delay(500);  // 字母间隔

  // S: 短闪 3 次（点 ···）
  for(int i=0; i<3; i++) {
    digitalWrite(ledPin, HIGH);
    delay(200);
    digitalWrite(ledPin, LOW);
    delay(200);
  }
  
  delay(2000); // 完整 SOS 结束，间隔2秒再重复
}
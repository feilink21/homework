#define TOUCH_PIN 4
#define LED_PIN 2
#define THRESHOLD 200 // 阈值，触摸时数值低于这个触发

bool ledState = false;

// 中断服务函数
void gotTouch() {
  ledState = !ledState;
  digitalWrite(LED_PIN, ledState);
}

void setup() {
  Serial.begin(115200);
  delay(1000);
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW); // 初始熄灭

  // 绑定触摸中断
  touchAttachInterrupt(TOUCH_PIN, gotTouch, THRESHOLD);
}

void loop() {
  // 空循环也可以，保持程序运行
  delay(10);
}

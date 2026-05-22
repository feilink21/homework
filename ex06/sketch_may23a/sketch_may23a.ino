// 定义两个LED引脚（必须支持PWM）
#define LED_A  2    // 第一个LED
#define LED_B  4    // 第二个LED

// 亮度变量
int brightness = 0;
// 每次增减的步长（数值越大变化越快）
int fadeStep = 3;

void setup() {
  // 设置两个引脚为输出模式
  pinMode(LED_A, OUTPUT);
  pinMode(LED_B, OUTPUT);
}

void loop() {
  // ========== 核心：反相PWM ==========
  // LED_A：从 0 → 255 变亮
  analogWrite(LED_A, brightness);
  
  // LED_B：从 255 → 0 变暗（和A完全相反）
  analogWrite(LED_B, 255 - brightness);

  // 更新亮度
  brightness += fadeStep;

  // 到达极值时反向
  if (brightness <= 0 || brightness >= 255) {
    fadeStep = -fadeStep;
  }

  // 控制渐变速度（越小越快）
  delay(10);
}

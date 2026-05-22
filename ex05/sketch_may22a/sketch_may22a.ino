#define TOUCH_PIN 4    // 触摸引脚 T0
#define LED_PIN   2    // PWM LED引脚（必须支持PWM）

// 触摸阈值（根据你的硬件微调）
#define THRESHOLD 500

// 呼吸灯速度档位：数值越小，呼吸越快
int speedGear = 1;     // 当前档位：1慢 / 2中 / 3快
int maxGear = 3;       // 总共3档

// 呼吸灯变量
int brightness = 0;
int fadeAmount = 5;

// 触摸自锁 + 防抖变量
bool lastTouched = false;
unsigned long lastDebounce = 0;
unsigned long debounceDelay = 80;

void setup() {
  Serial.begin(115200);
  pinMode(LED_PIN, OUTPUT);
}

void loop() {
  // ===================== 1. 读取触摸 + 档位切换 =====================
  int touchVal = touchRead(TOUCH_PIN);
  bool isTouched = (touchVal < THRESHOLD);
  unsigned long now = millis();

  // 防抖
  if (isTouched != lastTouched) {
    lastDebounce = now;
  }

  // 检测触摸按下瞬间 → 切换档位
  if (now - lastDebounce > debounceDelay) {
    if (isTouched && !lastTouched) {
      speedGear++;            // 档位+1
      if (speedGear > maxGear) {
        speedGear = 1;        // 循环回到1档
      }
      Serial.print("当前档位：");
      Serial.println(speedGear);
    }
    lastTouched = isTouched;
  }

  // ===================== 2. 呼吸灯主逻辑 =====================
  analogWrite(LED_PIN, brightness);
  brightness += fadeAmount;

  // 反向呼吸
  if (brightness <= 0 || brightness >= 255) {
    fadeAmount = -fadeAmount;
  }

  // ===================== 3. 根据档位控制速度 =====================
  switch (speedGear) {
    case 1: delay(20); break;   // 慢
    case 2: delay(10); break;   // 中
    case 3: delay(4);  break;   // 快
  }
}

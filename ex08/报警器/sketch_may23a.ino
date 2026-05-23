#include <WiFi.h>
#include <WebServer.h>

// ==================== 硬件配置 ====================
#define LED_PIN    2       // 报警LED
#define TOUCH_PIN  4       // 触摸引脚
#define THRESHOLD  500      // 触摸阈值

// ==================== 你的 WiFi 配置 ====================
const char* ssid     = "OnePlus Ace 3";
const char* password = "273688dgx";

WebServer server(80);

// ==================== 系统状态 ====================
bool isArmed   = false;   // 是否布防
bool isAlarm   = false;   // 是否报警
int touchVal;

// ==================== 网页 ====================
String getHTML() {
  String html = "<!DOCTYPE html>";
  html += "<html><head><meta charset='UTF-8'>";
  html += "<meta name='viewport' content='width=device-width, initial-scale=1.0'>";
  html += "<title>物联网安防报警器</title>";
  html += "<style>";
  html += "body{text-align:center; margin-top:60px; font-size:22px;}";
  html += ".btn{padding:15px 30px; margin:20px; font-size:20px; border:none; border-radius:8px;}";
  html += ".arm{background:#2ecc71; color:white;}";
  html += ".disarm{background:#e74c3c; color:white;}";
  html += ".status{margin:30px 0; font-weight:bold;}";
  html += "</style></head><body>";
  
  html += "<h1>ESP32 安防报警系统</h1>";
  html += "<div class='status'>系统状态：";
  if(isArmed) html += "✅ 已布防";
  else html += "🔒 已撤防";
  html += "</div>";
  
  html += "<div class='status'>报警状态：";
  if(isAlarm) html += "🚨 报警中！";
  else html += "✅ 正常";
  html += "</div>";
  
  html += "<button class='btn arm' onclick=\"location.href='/arm'\">布防 (Arm)</button>";
  html += "<button class='btn disarm' onclick=\"location.href='/disarm'\">撤防 (Disarm)</button>";
  
  html += "</body></html>";
  return html;
}

// ==================== 服务器路由 ====================
void handleRoot() {
  server.send(200, "text/html", getHTML());
}

// 布防
void handleArm() {
  isArmed = true;
  isAlarm = false;
  digitalWrite(LED_PIN, LOW);
  server.send(200, "text/html", getHTML());
}

// 撤防
void handleDisarm() {
  isArmed = false;
  isAlarm = false;
  digitalWrite(LED_PIN, LOW);
  server.send(200, "text/html", getHTML());
}

// ==================== 报警闪烁 ====================
void alarmFlash() {
  static unsigned long lastFlash = 0;
  static bool ledState = false;
  
  if(millis() - lastFlash >= 100) {  // 100ms 快闪
    lastFlash = millis();
    ledState = !ledState;
    digitalWrite(LED_PIN, ledState);
  }
}

// ==================== 初始化 ====================
void setup() {
  Serial.begin(115200);
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);
  
  // 连接WiFi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWiFi 连接成功");
  Serial.print("访问IP：");
  Serial.println(WiFi.localIP());

  // 路由
  server.on("/", handleRoot);
  server.on("/arm", handleArm);
  server.on("/disarm", handleDisarm);
  
  server.begin();
}

// ==================== 主循环 ====================
void loop() {
  server.handleClient();

  // 读取触摸
  touchVal = touchRead(TOUCH_PIN);

  // 布防状态 + 触摸 = 触发报警（自锁）
  if (isArmed && !isAlarm && touchVal < THRESHOLD) {
    isAlarm = true;
    Serial.println("🚨 触发报警！");
  }

  // 报警中：LED 狂闪
  if (isAlarm) {
    alarmFlash();
  }
}

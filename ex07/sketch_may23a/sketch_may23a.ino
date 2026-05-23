#include <WiFi.h>
#include <WebServer.h>

// ==================== 配置 ====================
#define TOUCH_PIN 4
const char* ssid = "OnePlus Ace 3";
const char* password = "273688dgx";

WebServer server(80);

// ==================== 主页面 ====================
String getHTML() {
  String html = "<!DOCTYPE html>";
  html += "<html lang='zh-CN'>";
  html += "<head><meta charset='UTF-8'>";
  html += "<meta name='viewport' content='width=device-width, initial-scale=1.0'>";
  html += "<title>实时传感器仪表盘</title>";
  html += "<style>";
  html += "body{text-align:center; font-family:Arial; background:#1a1a1a; color:white; padding:50px 0;}";
  html += ".panel{background:#222; padding:40px; border-radius:20px; width:80%; margin:0 auto; box-shadow:0 0 20px #000;}";
  html += ".title{font-size:24px; margin-bottom:30px; color:#00ccff;}";
  html += ".value{font-size:80px; font-weight:bold; color:#ffcc00; margin:40px 0;}";
  html += ".tip{font-size:18px; color:#aaa;}";
  html += "</style>";
  html += "</head>";
  html += "<body>";
  html += "<div class='panel'>";
  html += "<div class='title'>ESP32 实时触摸传感器仪表盘</div>";
  html += "<div class='value' id='val'>0</div>";
  html += "<div class='tip'>数值越小，触摸越接近</div>";
  html += "</div>";

  // AJAX 实时刷新
  html += "<script>";
  html += "function update(){fetch('/data').then(r=>r.text()).then(d=>{document.getElementById('val').innerText=d});}";
  html += "setInterval(update, 100);"; // 每100ms刷新一次
  html += "</script>";
  html += "</body></html>";
  return html;
}

// ==================== 路由 ====================
void handleRoot() {
  server.send(200, "text/html", getHTML());
}

// 返回实时传感器数据
void handleData() {
  int val = touchRead(TOUCH_PIN);
  server.send(200, "text/plain", String(val));
}

// ==================== 初始化 ====================
void setup() {
  Serial.begin(115200);
  
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWiFi OK");
  Serial.print("IP: ");
  Serial.println(WiFi.localIP());

  server.on("/", handleRoot);
  server.on("/data", handleData); // 传感器数据接口
  server.begin();
}

void loop() {
  server.handleClient();
}

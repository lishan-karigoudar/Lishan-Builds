// Smash Counter ESP32 v0.24 — IP Address Fix
// Lishan, 9 June 2026
//
// Fix: WiFi IP address text position corrected.
// Round screen clips text near edges — moved inward.
// Final working position: cursor (72, 212) and (74, 222)
// Shows "LishanTech" and "192.168.4.1" cleanly.
//
// Battery calibration attempted:
// Battery connected + USB simultaneously.
// Serial Monitor showed 0 — battery wire connection
// via tape not making proper electrical contact on ADC pin.
// Battery IS powering board correctly (screen works wirelessly).
// ADC measurement needs proper JST connector soldering.
// Calibration deferred.
//
// Battery connector situation:
// JST 1.25mm connector arrived.
// Battery has 3 wires (red/black/white) — balance connector.
// Papa connected with tape as temporary solution.
// Proper soldering needed for accurate ADC reading.

#include <Wire.h>
#include <math.h>
#include <Adafruit_GC9A01A.h>
#include <Adafruit_GFX.h>
#include <Preferences.h>
#include <WiFi.h>
#include <WebServer.h>

#define TFT_DC   8
#define TFT_CS   9
#define TFT_MOSI 11
#define TFT_SCLK 10
#define TFT_RST  14
#define TFT_BL   2
#define SDA_PIN  6
#define SCL_PIN  7
#define QMI8658_ADDR 0x6B
#define RESET_BTN 0
#define BAT_PIN  1

const char* ssid = "LishanTech";
const char* password = "smash123";

Adafruit_GC9A01A tft(TFT_CS, TFT_DC, TFT_MOSI, TFT_SCLK, TFT_RST);
Preferences prefs;
WebServer server(80);

volatile int smashCount = 0;
volatile int bestScore = 0;
volatile bool smashDetected = false;
volatile float lastPeakG = 0;
volatile unsigned long sessionStart = 0;
String lastIntensity = "none";
float lastG = 0;
unsigned long lastDisplayUpdate = 0;

#define SPM_WINDOW 10
unsigned long smashTimes[SPM_WINDOW];
int smashTimeIndex = 0;

void writeReg(uint8_t reg, uint8_t val) {
  Wire.beginTransmission(QMI8658_ADDR);
  Wire.write(reg);
  Wire.write(val);
  Wire.endTransmission();
}

void readRegs(uint8_t reg, uint8_t* buf, uint8_t len) {
  Wire.beginTransmission(QMI8658_ADDR);
  Wire.write(reg);
  Wire.endTransmission(false);
  Wire.requestFrom((uint8_t)QMI8658_ADDR, len);
  for (uint8_t i = 0; i < len; i++) buf[i] = Wire.read();
}

int calcSPM() {
  unsigned long now = millis();
  int count = 0;
  for (int i = 0; i < SPM_WINDOW; i++) {
    if (smashTimes[i] > 0 && (now - smashTimes[i]) <= 60000) count++;
  }
  return count;
}

int getBatteryPct() {
  int raw = analogRead(BAT_PIN);
  float voltage = (raw / 4095.0) * 3.3 * 2;
  int pct = (int)((voltage - 3.0) / (4.2 - 3.0) * 100);
  if (pct > 100) pct = 100;
  if (pct < 0) pct = 0;
  return pct;
}

String getIntensity(float g) {
  if (g >= 12.0) return "MAX";
  else if (g >= 8.0) return "HARD";
  else if (g >= 5.0) return "MEDIUM";
  else return "LIGHT";
}

String getIntensityColor(String intensity) {
  if (intensity == "MAX") return "#FF0000";
  else if (intensity == "HARD") return "#FF8C00";
  else if (intensity == "MEDIUM") return "#00CC00";
  else return "#0088FF";
}

void handleRoot() {
  unsigned long elapsed = (millis() - sessionStart) / 1000;
  int hrs = elapsed / 3600;
  int mins = (elapsed % 3600) / 60;
  int secs = elapsed % 60;
  char timer[10];
  sprintf(timer, "%02d:%02d:%02d", hrs, mins, secs);
  int spm = calcSPM();
  int bat = getBatteryPct();
  String ic = getIntensityColor(lastIntensity);
  String html = "<!DOCTYPE html><html><head>";
  html += "<meta charset='UTF-8'><meta name='viewport' content='width=device-width,initial-scale=1'>";
  html += "<meta http-equiv='refresh' content='1'>";
  html += "<title>LishanTech</title><style>";
  html += "body{background:#000;color:#fff;font-family:Arial;text-align:center;margin:0;padding:20px;}";
  html += ".title{color:#00FFFF;font-size:28px;font-weight:bold;}";
  html += ".sub{color:#888;font-size:14px;margin-bottom:20px;}";
  html += ".num{font-size:120px;font-weight:bold;color:#00FFFF;line-height:1;}";
  html += ".lbl{color:#888;font-size:16px;margin-bottom:15px;}";
  html += ".row{display:flex;justify-content:space-around;margin:15px 0;}";
  html += ".box{background:#111;border-radius:12px;padding:12px;width:28%;}";
  html += ".val{font-size:22px;font-weight:bold;color:#00FFFF;}";
  html += ".blbl{font-size:11px;color:#888;margin-top:3px;}";
  html += ".badge{font-size:28px;font-weight:bold;padding:8px 18px;border-radius:8px;display:inline-block;margin:8px 0;}";
  html += ".tmr{color:#00FFFF;font-size:16px;}";
  html += ".bat{color:#888;font-size:14px;margin-top:8px;}";
  html += "</style></head><body>";
  html += "<div class='title'>🏸 LishanTech</div>";
  html += "<div class='sub'>SmashCounter v1.0</div>";
  html += "<div class='num'>" + String(smashCount) + "</div>";
  html += "<div class='lbl'>smashes</div>";
  html += "<div class='row'>";
  html += "<div class='box'><div class='val'>" + String(spm) + "</div><div class='blbl'>per min</div></div>";
  html += "<div class='box'><div class='val'>" + String(lastPeakG, 1) + "g</div><div class='blbl'>last peak</div></div>";
  html += "<div class='box'><div class='val'>" + String(bestScore) + "</div><div class='blbl'>best</div></div>";
  html += "</div>";
  html += "<div class='badge' style='background:" + ic + "'>" + lastIntensity + "</div>";
  html += "<div class='tmr'>⏱ " + String(timer) + "</div>";
  html += "<div class='bat'>🔋 " + String(bat) + "%</div>";
  html += "</body></html>";
  server.send(200, "text/html", html);
}

void wifiTask(void* pvParameters) {
  WiFi.softAP(ssid, password);
  server.on("/", handleRoot);
  server.begin();
  while (true) {
    server.handleClient();
    vTaskDelay(10 / portTICK_PERIOD_MS);
  }
}

void drawPowerBar(float g) {
  int barX = 205, barY = 60, barW = 10, barH = 120;
  tft.fillRect(barX, barY, barW, barH, 0x0000);
  float level = g / 20.0;
  if (level > 1.0) level = 1.0;
  int fillH = (int)(barH * level);
  uint16_t color;
  if (level < 0.25) color = 0x001F;
  else if (level < 0.5) color = 0x07E0;
  else if (level < 0.75) color = 0xFD20;
  else color = 0xF800;
  tft.fillRect(barX, barY + barH - fillH, barW, fillH, color);
  tft.drawRect(barX, barY, barW, barH, 0x4208);
}

void drawBattery() {
  int pct = getBatteryPct();
  tft.fillRect(155, 22, 35, 16, 0x0000);
  tft.drawRect(155, 22, 28, 14, 0xFFFF);
  tft.fillRect(183, 26, 3, 6, 0xFFFF);
  uint16_t color = 0x07E0;
  if (pct < 20) color = 0xF800;
  else if (pct < 50) color = 0xFD20;
  int fillW = (int)(26 * pct / 100.0);
  tft.fillRect(156, 23, fillW, 12, color);
  tft.setTextSize(1);
  tft.setTextColor(0xFFFF);
  tft.setCursor(158, 27);
  tft.print(pct);
  tft.print("%");
}

void drawTimer() {
  unsigned long elapsed = (millis() - sessionStart) / 1000;
  int hrs = elapsed / 3600;
  int mins = (elapsed % 3600) / 60;
  int secs = elapsed % 60;
  tft.fillRect(65, 170, 110, 12, 0x0000);
  tft.setTextSize(1);
  tft.setTextColor(0x07FF);
  tft.setCursor(68, 172);
  if (hrs < 10) tft.print("0");
  tft.print(hrs);
  tft.print(":");
  if (mins < 10) tft.print("0");
  tft.print(mins);
  tft.print(":");
  if (secs < 10) tft.print("0");
  tft.print(secs);
}

void drawSPM() {
  tft.fillRect(65, 185, 110, 12, 0x0000);
  tft.setTextSize(1);
  tft.setTextColor(0xFFE0);
  tft.setCursor(68, 187);
  tft.print("SPM: ");
  tft.print(calcSPM());
}

void drawBest() {
  tft.fillRect(65, 200, 110, 12, 0x0000);
  tft.setTextSize(1);
  tft.setTextColor(0xF800);
  tft.setCursor(68, 202);
  tft.print("BEST: ");
  tft.print(bestScore);
}

void drawNumber() {
  tft.fillRect(40, 85, 160, 80, 0x0000);
  tft.setTextSize(8);
  tft.setTextColor(0x07FF);
  if (smashCount < 10) tft.setCursor(85, 88);
  else if (smashCount < 100) tft.setCursor(60, 88);
  else tft.setCursor(35, 88);
  tft.print(smashCount);
}

void drawSplash() {
  tft.fillScreen(0x0000);
  tft.drawCircle(120, 120, 118, 0x07FF);
  tft.drawCircle(120, 120, 115, 0x07FF);
  tft.setTextColor(0x07FF);
  tft.setTextSize(3);
  tft.setCursor(30, 70);
  tft.print("SMASH");
  tft.setCursor(15, 105);
  tft.print("COUNTER");
  tft.setTextColor(0xFFFF);
  tft.setTextSize(1);
  tft.setCursor(55, 150);
  tft.print("by Lishan");
  tft.setCursor(40, 165);
  tft.print("v1.0  2026");
  delay(2500);
}

void drawInitialScreen() {
  tft.fillScreen(0x0000);
  tft.drawCircle(120, 120, 118, 0x07FF);
  tft.setTextColor(0xFFFF);
  tft.setTextSize(2);
  tft.setCursor(55, 40);
  tft.print("SMASH");
  tft.setCursor(50, 65);
  tft.print("COUNTER");
  drawNumber();
  drawTimer();
  drawSPM();
  drawBest();
  drawBattery();
  tft.setTextSize(1);
  tft.setTextColor(0x07E0);
  tft.setCursor(72, 212);
  tft.print("LishanTech");
  tft.setCursor(74, 222);
  tft.print("192.168.4.1");
  drawPowerBar(0);
}

void setup() {
  Serial.begin(115200);
  pinMode(TFT_BL, OUTPUT);
  digitalWrite(TFT_BL, HIGH);
  pinMode(RESET_BTN, INPUT_PULLUP);
  esp_sleep_enable_ext0_wakeup(GPIO_NUM_0, 0);
  prefs.begin("smash", false);
  bestScore = prefs.getInt("best", 0);
  prefs.end();
  tft.begin();
  tft.setRotation(0);
  drawSplash();
  sessionStart = millis();
  drawInitialScreen();
  Wire.begin(SDA_PIN, SCL_PIN);
  delay(100);
  writeReg(0x02, 0x60);
  writeReg(0x03, 0x23);
  writeReg(0x04, 0x23);
  writeReg(0x08, 0x03);
  xTaskCreatePinnedToCore(wifiTask, "WiFiTask", 8192, NULL, 1, NULL, 0);
}

void loop() {
  if (digitalRead(RESET_BTN) == LOW) {
    unsigned long pressStart = millis();
    while (digitalRead(RESET_BTN) == LOW) { delay(10); }
    unsigned long pressDuration = millis() - pressStart;
    if (pressDuration > 3000) {
      tft.fillScreen(0x0000);
      tft.setTextColor(0x07FF);
      tft.setTextSize(2);
      tft.setCursor(30, 100);
      tft.print("Sleeping...");
      tft.setCursor(20, 130);
      tft.print("Press BOOT");
      tft.setCursor(25, 150);
      tft.print("to wake up");
      delay(1500);
      digitalWrite(TFT_BL, LOW);
      esp_deep_sleep_start();
    } else {
      smashCount = 0;
      sessionStart = millis();
      memset(smashTimes, 0, sizeof(smashTimes));
      drawInitialScreen();
    }
  }
  if (millis() - lastDisplayUpdate > 500) {
    drawNumber();
    drawBest();
    drawSPM();
    drawTimer();
    drawBattery();
    lastDisplayUpdate = millis();
  }
  uint8_t buf[6];
  readRegs(0x35, buf, 6);
  int16_t ax = (int16_t)(buf[1] << 8 | buf[0]);
  int16_t ay = (int16_t)(buf[3] << 8 | buf[2]);
  int16_t az = (int16_t)(buf[5] << 8 | buf[4]);
  float g = sqrt((float)ax*ax + (float)ay*ay + (float)az*az) / 4096.0;
  if (abs(g - lastG) > 0.3) {
    drawPowerBar(g);
    lastG = g;
  }
  if (g > 10.0 && !smashDetected) {
    smashCount++;
    smashDetected = true;
    lastPeakG = g;
    lastIntensity = getIntensity(g);
    smashTimes[smashTimeIndex % SPM_WINDOW] = millis();
    smashTimeIndex++;
    if (smashCount > bestScore) {
      bestScore = smashCount;
      prefs.begin("smash", false);
      prefs.putInt("best", bestScore);
      prefs.end();
    }
  }
  if (g < 1.5) smashDetected = false;
  delay(30);
}

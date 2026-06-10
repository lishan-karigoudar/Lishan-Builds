// Smash Counter ESP32 v0.26 — Haptic Motor
// Lishan, 9 June 2026
//
// New feature: Haptic buzz feedback
// Single buzz on every smash detected.
// Triple buzz when SPM target reached for current mode.
//
// Motor circuit:
// GPIO15 → 1K resistor → BC547 base
// BC547 collector → vibration motor positive
// BC547 emitter → GND
// Motor negative → GND
// Motor VCC from 3.3V
//
// Components already ordered from robu.in:
// Vibration motor 3V 4x8mm — received ✅
// BC547 NPN transistor — received ✅
// 1K resistor — received ✅
// Motor not yet physically wired — code ready for when connected.
//
// goalBuzzed flag prevents repeated triple buzz —
// resets when BOOT button pressed to start new session.
//
// No speaker on board — vibration is the only feedback available.
// Vibration is actually better on a badminton court —
// you feel it on your wrist even when it is noisy.
//
// Known issue: buzz() uses delay() which blocks main loop.
// Causes freeze and crash with WiFi task.
// Fix: use non-blocking buzz in v0.27.

#include <Wire.h>
#include <math.h>
#include <Adafruit_GC9A01A.h>
#include <Adafruit_GFX.h>
#include <Preferences.h>
#include <WiFi.h>
#include <WebServer.h>
#include <CST816S.h>

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
#define TOUCH_INT 5
#define TOUCH_RST 13
#define HAPTIC_PIN 15

const char* ssid = "LishanTech";
const char* password = "smash123";

Adafruit_GC9A01A tft(TFT_CS, TFT_DC, TFT_MOSI, TFT_SCLK, TFT_RST);
CST816S touch(SDA_PIN, SCL_PIN, TOUCH_RST, TOUCH_INT);
Preferences prefs;
WebServer server(80);

int currentMode = 1;
int modeTargets[] = {7, 10, 15};
String modeNames[] = {"EASY", "MEDIUM", "HARD"};
uint16_t modeColors[] = {0x07E0, 0xFD20, 0xF800};

bool showModeScreen = false;
bool goalBuzzed = false;

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

void buzz(int times, int duration) {
  for (int i = 0; i < times; i++) {
    digitalWrite(HAPTIC_PIN, HIGH);
    delay(duration);
    digitalWrite(HAPTIC_PIN, LOW);
    delay(100);
  }
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
  bool goalAchieved = spm >= modeTargets[currentMode];
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
  html += ".goal{font-size:18px;font-weight:bold;margin-top:10px;padding:8px;border-radius:8px;}";
  html += ".achieved{background:#004400;color:#00FF00;}";
  html += ".pending{background:#440000;color:#FF4444;}";
  html += "</style></head><body>";
  html += "<div class='title'>🏸 LishanTech</div>";
  html += "<div class='sub'>SmashCounter v2.0 | Mode: " + modeNames[currentMode] + "</div>";
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
  if (goalAchieved) {
    html += "<div class='goal achieved'>🎯 GOAL ACHIEVED! " + String(spm) + "/" + String(modeTargets[currentMode]) + " SPM</div>";
  } else {
    html += "<div class='goal pending'>⏳ TARGET: " + String(modeTargets[currentMode]) + " SPM | NOW: " + String(spm) + "</div>";
  }
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
  int spm = calcSPM();
  tft.fillRect(65, 185, 110, 12, 0x0000);
  tft.setTextSize(1);
  tft.setTextColor(0xFFE0);
  tft.setCursor(68, 187);
  tft.print("SPM: ");
  tft.print(spm);
  tft.print("/");
  tft.print(modeTargets[currentMode]);
}

void drawGoal() {
  int spm = calcSPM();
  tft.fillRect(55, 155, 130, 12, 0x0000);
  tft.setTextSize(1);
  if (spm >= modeTargets[currentMode]) {
    tft.setTextColor(0x07E0);
    tft.setCursor(58, 157);
    tft.print("GOAL ACHIEVED!");
  } else {
    tft.setTextColor(0xF800);
    tft.setCursor(58, 157);
    tft.print("TARGET: ");
    tft.print(modeTargets[currentMode]);
    tft.print(" SPM");
  }
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
  tft.fillRect(40, 85, 160, 70, 0x0000);
  tft.setTextSize(7);
  tft.setTextColor(0x07FF);
  if (smashCount < 10) tft.setCursor(88, 88);
  else if (smashCount < 100) tft.setCursor(63, 88);
  else tft.setCursor(38, 88);
  tft.print(smashCount);
}

void drawModeIndicator() {
  tft.fillRect(40, 40, 110, 12, 0x0000);
  tft.setTextSize(1);
  tft.setTextColor(modeColors[currentMode]);
  tft.setCursor(43, 42);
  tft.print("Mode: ");
  tft.print(modeNames[currentMode]);
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
  tft.print("v2.0  2026");
  delay(2500);
}

void drawInitialScreen() {
  tft.fillScreen(0x0000);
  tft.drawCircle(120, 120, 118, 0x07FF);
  tft.setTextColor(0xFFFF);
  tft.setTextSize(2);
  tft.setCursor(55, 58);
  tft.print("SMASH");
  drawModeIndicator();
  drawNumber();
  drawGoal();
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
  goalBuzzed = false;
}

void drawModeSelectionScreen() {
  tft.fillScreen(0x0000);
  tft.drawCircle(120, 120, 118, 0x4208);
  tft.setTextColor(0xFFFF);
  tft.setTextSize(2);
  tft.setCursor(45, 25);
  tft.print("SELECT MODE");
  tft.fillRoundRect(35, 60, 170, 35, 8, 0x03E0);
  tft.setTextColor(0x0000);
  tft.setTextSize(2);
  tft.setCursor(75, 70);
  tft.print("EASY");
  tft.setTextSize(1);
  tft.setCursor(110, 90);
  tft.print("7 SPM");
  tft.fillRoundRect(35, 105, 170, 35, 8, 0xFD20);
  tft.setTextColor(0x0000);
  tft.setTextSize(2);
  tft.setCursor(60, 115);
  tft.print("MEDIUM");
  tft.setTextSize(1);
  tft.setCursor(110, 135);
  tft.print("10 SPM");
  tft.fillRoundRect(35, 150, 170, 35, 8, 0xF800);
  tft.setTextColor(0xFFFF);
  tft.setTextSize(2);
  tft.setCursor(75, 160);
  tft.print("HARD");
  tft.setTextSize(1);
  tft.setCursor(110, 180);
  tft.print("15 SPM");
  tft.fillRoundRect(60, 195, 120, 25, 8, 0x4208);
  tft.setTextColor(0xFFFF);
  tft.setTextSize(1);
  tft.setCursor(88, 203);
  tft.print("EXIT");
}

void setup() {
  Serial.begin(115200);
  pinMode(TFT_BL, OUTPUT);
  digitalWrite(TFT_BL, HIGH);
  pinMode(RESET_BTN, INPUT_PULLUP);
  pinMode(HAPTIC_PIN, OUTPUT);
  digitalWrite(HAPTIC_PIN, LOW);
  esp_sleep_enable_ext0_wakeup(GPIO_NUM_0, 0);
  prefs.begin("smash", false);
  bestScore = prefs.getInt("best", 0);
  currentMode = prefs.getInt("mode", 1);
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
  touch.begin();
  xTaskCreatePinnedToCore(wifiTask, "WiFiTask", 8192, NULL, 1, NULL, 0);
}

void loop() {
  if (touch.available()) {
    int tx = touch.data.x;
    int ty = touch.data.y;
    uint8_t gesture = touch.data.gestureID;
    if (!showModeScreen) {
      if (gesture == SWIPE_RIGHT) {
        showModeScreen = true;
        drawModeSelectionScreen();
      }
    } else {
      if (ty >= 60 && ty <= 95) {
        currentMode = 0;
        showModeScreen = false;
        prefs.begin("smash", false);
        prefs.putInt("mode", currentMode);
        prefs.end();
        drawInitialScreen();
      } else if (ty >= 105 && ty <= 140) {
        currentMode = 1;
        showModeScreen = false;
        prefs.begin("smash", false);
        prefs.putInt("mode", currentMode);
        prefs.end();
        drawInitialScreen();
      } else if (ty >= 150 && ty <= 185) {
        currentMode = 2;
        showModeScreen = false;
        prefs.begin("smash", false);
        prefs.putInt("mode", currentMode);
        prefs.end();
        drawInitialScreen();
      } else if (ty >= 195 && ty <= 220) {
        showModeScreen = false;
        drawInitialScreen();
      }
    }
  }
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
      showModeScreen = false;
      drawInitialScreen();
    }
  }
  if (!showModeScreen) {
    if (millis() - lastDisplayUpdate > 500) {
      drawNumber();
      drawBest();
      drawSPM();
      drawTimer();
      drawGoal();
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
      buzz(1, 50);
      if (!goalBuzzed && calcSPM() >= modeTargets[currentMode]) {
        buzz(3, 100);
        goalBuzzed = true;
      }
    }
    if (g < 1.5) smashDetected = false;
  }
  delay(30);
}

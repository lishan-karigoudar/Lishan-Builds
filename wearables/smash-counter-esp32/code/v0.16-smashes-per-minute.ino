// Smash Counter ESP32 v0.16 — Smashes Per Minute
// Lishan, 27 May 2026
//
// New feature: Smashes Per Minute (SPM)
// Shows how many smashes in the last 60 seconds.
// Updates every second alongside the timer.
// Uses a rolling window of last 10 smash timestamps.
//
// How SPM works:
// Every smash — timestamp saved to smashTimes[] array
// Every second — count how many timestamps are within 60 seconds
// That count = current SPM
// Array size = 10 (SPM_WINDOW) — enough for most sessions
//
// Screen layout:
// HH:MM:SS timer — y=172
// SPM: X — y=187 (yellow)
// BEST: XX — y=202 (red)
// All centred below the big number

#include <Wire.h>
#include <math.h>
#include <Adafruit_GC9A01A.h>
#include <Adafruit_GFX.h>
#include <Preferences.h>

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

Adafruit_GC9A01A tft(TFT_CS, TFT_DC, TFT_MOSI, TFT_SCLK, TFT_RST);
Preferences prefs;

int smashCount = 0;
int bestScore = 0;
bool smashDetected = false;
float lastG = 0;
unsigned long sessionStart = 0;
unsigned long lastTimerUpdate = 0;

#define SPM_WINDOW 10
unsigned long smashTimes[SPM_WINDOW];
int smashTimeIndex = 0;
int spmCount = 0;

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

void drawPowerBar(float g) {
  int barX = 205;
  int barY = 60;
  int barW = 10;
  int barH = 120;
  tft.fillRect(barX, barY, barW, barH, 0x0000);
  float maxG = 20.0;
  float level = g / maxG;
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

int calcSPM() {
  unsigned long now = millis();
  int count = 0;
  for (int i = 0; i < SPM_WINDOW; i++) {
    if (smashTimes[i] > 0 && (now - smashTimes[i]) <= 60000) count++;
  }
  return count;
}

void drawSPM() {
  int spm = calcSPM();
  tft.fillRect(65, 185, 110, 12, 0x0000);
  tft.setTextSize(1);
  tft.setTextColor(0xFFE0);
  tft.setCursor(68, 187);
  tft.print("SPM: ");
  tft.print(spm);
}

void drawBest() {
  yield();
  tft.fillRect(65, 200, 110, 12, 0x0000);
  tft.setTextSize(1);
  tft.setTextColor(0xF800);
  tft.setCursor(68, 202);
  tft.print("BEST: ");
  tft.print(bestScore);
  yield();
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
  yield();
  delay(2500);
}

void drawNumber() {
  yield();
  tft.fillRect(40, 85, 160, 80, 0x0000);
  yield();
  tft.setTextSize(8);
  tft.setTextColor(0x07FF);
  if (smashCount < 10) tft.setCursor(85, 88);
  else if (smashCount < 100) tft.setCursor(60, 88);
  else tft.setCursor(35, 88);
  tft.print(smashCount);
  yield();
}

void drawInitialScreen() {
  tft.fillScreen(0x0000);
  yield();
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
  drawPowerBar(0);
}

void setup() {
  Serial.begin(115200);
  pinMode(TFT_BL, OUTPUT);
  digitalWrite(TFT_BL, HIGH);
  pinMode(RESET_BTN, INPUT_PULLUP);
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
  Serial.println("Ready!");
}

void loop() {
  if (digitalRead(RESET_BTN) == LOW) {
    smashCount = 0;
    sessionStart = millis();
    spmCount = 0;
    memset(smashTimes, 0, sizeof(smashTimes));
    drawInitialScreen();
    delay(300);
  }
  if (millis() - lastTimerUpdate > 1000) {
    drawTimer();
    drawSPM();
    lastTimerUpdate = millis();
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
    smashTimes[smashTimeIndex % SPM_WINDOW] = millis();
    smashTimeIndex++;
    spmCount++;
    if (smashCount > bestScore) {
      bestScore = smashCount;
      prefs.begin("smash", false);
      prefs.putInt("best", bestScore);
      prefs.end();
    }
    delay(100);
    drawNumber();
    drawBest();
    drawSPM();
  }
  if (g < 1.5) smashDetected = false;
  delay(50);
}

// Smash Counter ESP32 v0.14 — Power Bar
// Lishan, 21 May 2026
//
// LISHAN'S OWN IDEA — not prompted.
//
// Lishan's exact words:
// "I also thought about keeping smash level on the right side
// which is vertical like when you do a light smash it rises to
// blue then a medium smash it will go to green then you do a
// normal smash it gets orange and your hardest smash will go
// to red."
//
// This is a professional sports wearable feature.
// Real sports sensors use exactly this kind of power indicator.
//
// How the power bar works:
// Vertical bar on right side of round screen
// x=205-215, y=60 to 180 — 120px tall
// Updates continuously — not just on smash detection
// Color based on g-force level:
// Blue  = 0-25% of max (light movement)
// Green = 25-50% of max (medium)
// Orange = 50-75% of max (hard)
// Red   = 75-100% of max (maximum smash)
// Max g set to 20.0 for scaling
//
// Test results — 21 May 2026:
// Blue ✅ Green ✅ Orange ✅
// Red needs real court smash — not triggered sitting in chair
// Lishan: "I saw b g and o" — Blue Green Orange confirmed
//
// Battery concern raised by Lishan:
// "If we do soldering and the battery spoils we will never
// be separated and if I want to keep new code USB and battery
// will make ESP32-S3 heat up blast to ashes"
// Smart engineering thinking — correctly identified risk.
// Solution: MX1.25 is plug/unplug — no permanent connection.
// Battery: existing 3.7V batteries work — need MX1.25 connector.
// Cost: ₹9 per connector — ₹45 for 5 from robu.in.

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
  tft.fillRect(40, 90, 160, 80, 0x0000);
  yield();
  tft.setTextSize(8);
  tft.setTextColor(0x07FF);
  if (smashCount < 10) tft.setCursor(85, 95);
  else if (smashCount < 100) tft.setCursor(60, 95);
  else tft.setCursor(35, 95);
  tft.print(smashCount);
  yield();
}

void drawBest() {
  yield();
  tft.fillRect(40, 175, 160, 20, 0x0000);
  tft.setTextSize(1);
  tft.setTextColor(0xF800);
  tft.setCursor(45, 178);
  tft.print("BEST: ");
  tft.print(bestScore);
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
  tft.setTextSize(2);
  tft.setTextColor(0xFFFF);
  tft.setCursor(50, 195);
  tft.print("smashes");
  drawNumber();
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
    drawInitialScreen();
    delay(300);
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
    if (smashCount > bestScore) {
      bestScore = smashCount;
      prefs.begin("smash", false);
      prefs.putInt("best", bestScore);
      prefs.end();
    }
    Serial.print("SMASH! Count: ");
    Serial.println(smashCount);
    delay(100);
    drawNumber();
    drawBest();
  }
  if (g < 1.5) smashDetected = false;
  delay(50);
}

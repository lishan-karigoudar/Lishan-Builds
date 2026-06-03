// Smash Counter ESP32 v0.10 — Display Working Final
// Lishan, 21 May 2026
//
// 🎉 DISPLAY WORKING ON ROUND LCD 🎉
//
// Split drawScreen() into two functions:
// drawInitialScreen() — draws full screen once at startup
// drawNumber() — only redraws the number area on each smash
//
// This fixed the crash completely.
// fillScreen() only called once — not on every smash.
// On smash: only fillRect() on number area — much faster.
//
// Also added reset button using BOOT button (GPIO0):
// Press BOOT → count resets to 0 → screen redraws
//
// First session output — 21 May 2026:
// Screen shows: SMASH COUNTER 0 smashes on startup
// Smash → count increments on screen AND Serial Monitor
// BOOT button → resets to 0
//
// Text looks pixelated — basic Arduino font.
// Acceptable for prototype. Better fonts planned later.
//
// Threshold: 10.0g
// For real court with racket: try 11.0g or 12.0g
//
// Battery connection attempted at end of session.
// MX1.25 connector on board — battery plugs in directly.

#include <Wire.h>
#include <math.h>
#include <Adafruit_GC9A01A.h>
#include <Adafruit_GFX.h>

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

int smashCount = 0;
bool smashDetected = false;

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

void drawNumber() {
  tft.fillRect(50, 90, 180, 80, 0x0000);
  tft.setTextSize(8);
  tft.setTextColor(0x07FF);
  if (smashCount < 10) tft.setCursor(100, 95);
  else if (smashCount < 100) tft.setCursor(75, 95);
  else tft.setCursor(50, 95);
  tft.print(smashCount);
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
  tft.setTextSize(2);
  tft.setTextColor(0xFFFF);
  tft.setCursor(60, 175);
  tft.print("smashes");
  drawNumber();
}

void setup() {
  Serial.begin(115200);
  pinMode(TFT_BL, OUTPUT);
  digitalWrite(TFT_BL, HIGH);
  pinMode(RESET_BTN, INPUT_PULLUP);
  tft.begin();
  tft.setRotation(0);
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

  if (g > 10.0 && !smashDetected) {
    smashCount++;
    smashDetected = true;
    Serial.print("SMASH! Count: ");
    Serial.println(smashCount);
    delay(100);
    drawNumber();
  }
  if (g < 1.5) smashDetected = false;
  delay(50);
}

// Smash Counter ESP32 v0.13 — Persistent Best Score
// Lishan, 21 May 2026
//
// LISHAN'S OWN IDEA — not prompted.
//
// Problem with v0.12:
// Best score resets to 0 when BOOT button pressed or power lost.
// Lishan's observation:
// "If you do 38 smashes in 1 match it will show 38 highest score
// and it will keep as 38 as highest score and you play another
// match and you do 57 smashes it will change to 57 as highest
// score and even if reset your progress wont be lost."
//
// Solution: Preferences library — built into ESP32.
// Saves data to internal flash memory permanently.
// Survives reset. Survives power off. Survives battery removal.
//
// How it works:
// On startup: load saved bestScore from flash
// On new best: save new bestScore to flash immediately
// On BOOT press: resets smashCount to 0 — bestScore stays
//
// This is called non-volatile storage — real products use this
// for saving user settings and high scores.

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
  tft.fillRect(50, 90, 180, 80, 0x0000);
  yield();
  tft.setTextSize(8);
  tft.setTextColor(0x07FF);
  if (smashCount < 10) tft.setCursor(100, 95);
  else if (smashCount < 100) tft.setCursor(75, 95);
  else tft.setCursor(50, 95);
  tft.print(smashCount);
  yield();
}

void drawBest() {
  yield();
  tft.fillRect(50, 175, 180, 20, 0x0000);
  tft.setTextSize(1);
  tft.setTextColor(0xF800);
  tft.setCursor(55, 178);
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
  tft.setCursor(60, 195);
  tft.print("smashes");
  drawNumber();
  drawBest();
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

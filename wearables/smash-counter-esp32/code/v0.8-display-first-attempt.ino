// Smash Counter ESP32 v0.8 — Display First Attempt
// Lishan, 21 May 2026
//
// First attempt to show smash count on round LCD display.
// Used GC9A01A display driver — correct chip for this board.
//
// Libraries installed:
// - Adafruit GC9A01A by Adafruit
// - Adafruit GFX Library by Adafruit
//
// Problem: wrong SPI pin numbers used.
// Board crashed immediately on display init.
// Correct pins found in next version v0.9.
//
// Wrong pins used here:
// TFT_DC  = 2  ❌ correct = 8
// TFT_CS  = 10 ❌ correct = 9
// TFT_MOSI = 11 ✅
// TFT_SCLK = 12 ❌ correct = 10
// TFT_RST = -1 ❌ correct = 14
// TFT_BL  = 3  ❌ correct = 2

#include <Wire.h>
#include <math.h>
#include <Adafruit_GC9A01A.h>
#include <Adafruit_GFX.h>

#define TFT_DC   2
#define TFT_CS   10
#define TFT_MOSI 11
#define TFT_SCLK 12
#define TFT_RST  -1
#define TFT_BL   3
#define SDA_PIN  6
#define SCL_PIN  7
#define QMI8658_ADDR 0x6B

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

void drawScreen() {
  tft.fillScreen(0x0000);
  tft.drawCircle(120, 120, 118, 0x07FF);
  tft.setTextColor(0xFFFF);
  tft.setTextSize(2);
  tft.setCursor(55, 40);
  tft.print("SMASH");
  tft.setCursor(50, 65);
  tft.print("COUNTER");
  tft.setTextSize(6);
  tft.setTextColor(0x07FF);
  if (smashCount < 10) tft.setCursor(100, 100);
  else if (smashCount < 100) tft.setCursor(75, 100);
  else tft.setCursor(50, 100);
  tft.print(smashCount);
  tft.setTextSize(2);
  tft.setTextColor(0xFFFF);
  tft.setCursor(60, 175);
  tft.print("smashes");
}

void setup() {
  Serial.begin(115200);
  pinMode(TFT_BL, OUTPUT);
  digitalWrite(TFT_BL, HIGH);
  tft.begin();
  tft.setRotation(0);
  drawScreen();
  Wire.begin(SDA_PIN, SCL_PIN);
  delay(100);
  writeReg(0x02, 0x60);
  writeReg(0x03, 0x23);
  writeReg(0x04, 0x23);
  writeReg(0x08, 0x03);
  Serial.println("Ready!");
}

void loop() {
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
    drawScreen();
  }
  if (g < 1.5) smashDetected = false;
  delay(50);
}

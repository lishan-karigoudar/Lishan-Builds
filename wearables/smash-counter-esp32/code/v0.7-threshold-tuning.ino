// Smash Counter ESP32 v0.7 — Threshold Tuning
// Lishan, 21 May 2026
//
// Calibrating threshold for real smash detection.
// Started at 2.5g — any shake triggered it.
// Tested progressively higher values:
//
// 2.5g — triggered by any movement ❌
// 4.0g — still too sensitive ❌
// 6.0g — better but still triggering on normal movement ❌
// 10.0g — feels right for hard smash motion ✅
//
// Lishan's note: for real court with racket,
// may need 11.0g or 12.0g — to test on court.
//
// Key learning:
// Threshold calibration is iterative.
// Start low, increase until false triggers stop.
// Same process as micro:bit — different numbers.

#include <Wire.h>
#include <math.h>

#define QMI8658_ADDR 0x6B
#define SDA_PIN 6
#define SCL_PIN 7

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

void setup() {
  Serial.begin(115200);
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
  }
  if (g < 1.5) smashDetected = false;
  delay(50);
}

// Smash Counter ESP32 v0.6 — QMI8658 WORKING Smash Detection
// Lishan, 21 May 2026
//
// 🎉 FIRST WORKING ESP32 SMASH COUNTER 🎉
//
// After 7 days of debugging — 14 May to 21 May —
// the QMI8658 built-in sensor is finally reading correctly.
//
// What finally worked:
// - Fresh sketch — no leftover Waveshare library files
// - Correct board: ESP32S3 Dev Module
// - Correct pins: Wire.begin(6, 7) — SDA GPIO6, SCL GPIO7
// - Correct address: QMI8658_ADDR 0x6B
// - Direct I2C register reads — no library needed
// - math.h for sqrt() function
//
// First output on Serial Monitor — 21 May 2026:
// SMASH! Count: 1
// SMASH! Count: 2
// SMASH! Count: 3
// SMASH! Count: 4
// SMASH! Count: 5
// SMASH! Count: 6
// SMASH! Count: 7
// SMASH! Count: 8
//
// The garbage characters before the count are ESP32 boot
// messages — normal, cannot be suppressed on this board.
// Ignore them — the smash counter code is working correctly.
//
// Current threshold: 2.5g
// This needs court calibration — same process as micro:bit.
// Gentle shot, medium shot, hardest smash — measure each.
// Set threshold from real data.
//
// Next steps:
// 1. Calibrate threshold on real court
// 2. Add display output — show count on round LCD
// 3. Add battery — cut USB cable, go wireless

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

  uint8_t whoami = 0;
  readRegs(0x00, &whoami, 1);
  Serial.print("WHO_AM_I: 0x");
  Serial.println(whoami, HEX);

  writeReg(0x02, 0x60); // enable sensors
  writeReg(0x03, 0x23); // accel 4g 125Hz
  writeReg(0x04, 0x23); // gyro 512dps 125Hz
  writeReg(0x08, 0x03); // enable accel + gyro

  Serial.println("Ready!");
}

void loop() {
  uint8_t buf[6];
  readRegs(0x35, buf, 6);

  int16_t ax = (int16_t)(buf[1] << 8 | buf[0]);
  int16_t ay = (int16_t)(buf[3] << 8 | buf[2]);
  int16_t az = (int16_t)(buf[5] << 8 | buf[4]);

  float g = sqrt((float)ax*ax + (float)ay*ay + (float)az*az) / 4096.0;

  if (g > 2.5 && !smashDetected) {
    smashCount++;
    smashDetected = true;
    Serial.print("SMASH! Count: ");
    Serial.println(smashCount);
  }

  if (g < 1.5) smashDetected = false;

  delay(50);
}

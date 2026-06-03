// Smash Counter ESP32 v0.4 — LSM6DS3 Wrong Chip
// Lishan, 18-20 May 2026
//
// ⚠️ WRONG SENSOR — ARCHIVED FOR REFERENCE ⚠️
//
// What happened:
// After QMI8658 v0.3 failed, tried LSM6DS3 library.
// LSM6DS3 is a completely different sensor chip.
// Our board has QMI8658 — not LSM6DS3.
// This code never worked. Archived as wrong path record.
//
// Errors seen:
// - Library not found in web editor
// - Guru Meditation Error — board crashing and restarting
// - Serial Monitor showing only boot messages, no sensor data
//
// Root causes identified later:
// 1. Wrong board selected — XIAO_ESP32S3 instead of ESP32S3 Dev Module
// 2. Wire.begin() called without SDA/SCL pins — needs GPIO6, GPIO7
// 3. USB CDC on Boot not enabled — Serial.println not showing
// 4. Wrong ESP32 board version — needs exactly 2.0.12

#include <Wire.h>
#include <LSM6DS3.h>

LSM6DS3 myIMU(I2C_MODE, 0x6A);
int smashCount = 0;
bool smashDetected = false;

void setup() {
  Serial.begin(115200);
  if (myIMU.begin() != 0) {
    Serial.println("IMU not found!");
    while (1);
  }
  Serial.println("Ready! Smash counter starting...");
}

void loop() {
  float x = myIMU.readFloatAccelX();
  float y = myIMU.readFloatAccelY();
  float z = myIMU.readFloatAccelZ();

  float totalAccel = sqrt(x*x + y*y + z*z);

  if (totalAccel > 2.5 && !smashDetected) {
    smashCount++;
    smashDetected = true;
    Serial.print("SMASH! Count: ");
    Serial.println(smashCount);
  }

  if (totalAccel < 1.5) {
    smashDetected = false;
  }

  delay(50);
}

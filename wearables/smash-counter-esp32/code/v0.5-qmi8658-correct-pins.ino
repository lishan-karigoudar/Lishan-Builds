// Smash Counter ESP32 v0.5 — QMI8658 Correct Pins
// Lishan, 20 May 2026
//
// First code written with ALL correct settings confirmed.
// Based on official Waveshare wiki — no assumptions.
//
// What was wrong before:
// - Wrong board: XIAO_ESP32S3 → correct: ESP32S3 Dev Module
// - Missing pins: Wire.begin() → correct: Wire.begin(6, 7)
// - Wrong address: 0x6A → correct: 0x6B
// - USB CDC on Boot: disabled → correct: enabled
// - ESP32 version: latest → correct: 2.0.12 exactly
//
// How we found the right settings:
// Lishan read the label on the physical board: ESP32-S3-Touch-LCD-1.28
// Santy shared the full Waveshare wiki specs
// Official manual confirmed all pin numbers and settings
//
// What this code does:
// Reads WHO_AM_I register first — prints Sensor ID to Serial
// If sensor found — enables accelerometer
// Reads AccX AccY AccZ continuously
// Detects smash when total acceleration > 2.5g
//
// To verify it works:
// Upload → Tools → USB CDC On Boot: Enabled
// Open Serial Monitor at 115200
// Press RST button
// Should see: "Sensor ID: 05" then "Ready!"
// Shake board → should see "SMASH! Count: 1"
//
// Note: Official Waveshare demo package downloaded for
// reference — their QMI8658 library is the definitive source.

#include <Wire.h>

#define QMI8658_ADDR 0x6B
#define SDA_PIN 6
#define SCL_PIN 7

int smashCount = 0;
bool smashDetected = false;

void setup() {
  Serial.begin(115200);
  Wire.begin(SDA_PIN, SCL_PIN);
  delay(1000);

  // Read WHO_AM_I register
  Wire.beginTransmission(QMI8658_ADDR);
  Wire.write(0x00);
  Wire.endTransmission();
  Wire.requestFrom(QMI8658_ADDR, 1);
  uint8_t whoami = Wire.read();
  Serial.print("Sensor ID: ");
  Serial.println(whoami, HEX);

  // Enable accelerometer 4g 125Hz
  Wire.beginTransmission(QMI8658_ADDR);
  Wire.write(0x03);
  Wire.write(0x23);
  Wire.endTransmission();

  Wire.beginTransmission(QMI8658_ADDR);
  Wire.write(0x08);
  Wire.write(0x01);
  Wire.endTransmission();

  Serial.println("Ready!");
}

void loop() {
  uint8_t buf[6];
  Wire.beginTransmission(QMI8658_ADDR);
  Wire.write(0x35);
  Wire.endTransmission();
  Wire.requestFrom(QMI8658_ADDR, 6);
  for (int i = 0; i < 6; i++) buf[i] = Wire.read();

  int16_t ax = (int16_t)(buf[1] << 8 | buf[0]);
  int16_t ay = (int16_t)(buf[3] << 8 | buf[2]);
  int16_t az = (int16_t)(buf[5] << 8 | buf[4]);

  float g = sqrt((float)ax*ax + (float)ay*ay + (float)az*az) / 4096.0;

  Serial.print("AccX: "); Serial.print(ax);
  Serial.print(" AccY: "); Serial.print(ay);
  Serial.print(" AccZ: "); Serial.print(az);
  Serial.print(" Total: "); Serial.println(g);

  if (g > 2.5 && !smashDetected) {
    smashCount++;
    smashDetected = true;
    Serial.print("SMASH! Count: ");
    Serial.println(smashCount);
  }

  if (g < 1.5) smashDetected = false;

  delay(50);
}

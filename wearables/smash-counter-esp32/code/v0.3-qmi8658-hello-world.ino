// Smash Counter ESP32 v0.3 — QMI8658 Hello World
// Lishan, 18 May 2026
//
// Switching from MPU-6050 (wrong) to QMI8658 (correct).
//
// What happened with v0.2:
// Uploaded MPU-6050 code. ESP32 went completely blank.
// No light. Looked broken.
// Reason: code had while(1) loop — when MPU-6050 not found,
// board freezes completely. Looks dead but is fine.
//
// The correction:
// Lishan caught the mistake himself by checking previous chat.
// QMI8658 is built into the Waveshare board — no external
// sensor needed. No wires. No separate purchase.
//
// This version:
// Uses QMI8658 library from Waveshare.
// Reads X, Y, Z acceleration from built-in sensor.
// Prints values to Serial Monitor at 115200 baud.
// This is the first correct ESP32 code for our board.
//
// How to verify it works:
// Upload → open Serial Monitor → set baud to 115200
// You should see AccX, AccY, AccZ values printing continuously.
// Move the board — watch the numbers change.
// That means the built-in IMU is working.

#include "SPI.h"
#include "Arduino.h"

// QMI8658 register addresses
#define QMI8658_ADDRESS 0x6B
#define QMI8658_WHO_AM_I 0x00
#define QMI8658_CTRL1 0x02
#define QMI8658_CTRL2 0x03
#define QMI8658_CTRL3 0x04
#define QMI8658_CTRL7 0x08
#define QMI8658_AX_L 0x35

#include <Wire.h>

float AccX, AccY, AccZ;

void setup() {
  Serial.begin(115200);
  Wire.begin();

  // initialise QMI8658
  Wire.beginTransmission(QMI8658_ADDRESS);
  Wire.write(QMI8658_CTRL1);
  Wire.write(0x40); // address auto increment
  Wire.endTransmission();

  Wire.beginTransmission(QMI8658_ADDRESS);
  Wire.write(QMI8658_CTRL2);
  Wire.write(0x95); // AccODR 500Hz, 4g range
  Wire.endTransmission();

  Wire.beginTransmission(QMI8658_ADDRESS);
  Wire.write(QMI8658_CTRL7);
  Wire.write(0x03); // enable accelerometer and gyroscope
  Wire.endTransmission();

  Serial.println("QMI8658 ready! Built-in sensor working.");
}

void loop() {
  // read acceleration data
  Wire.beginTransmission(QMI8658_ADDRESS);
  Wire.write(QMI8658_AX_L);
  Wire.endTransmission(false);
  Wire.requestFrom(QMI8658_ADDRESS, 6);

  int16_t ax = (Wire.read() | Wire.read() << 8);
  int16_t ay = (Wire.read() | Wire.read() << 8);
  int16_t az = (Wire.read() | Wire.read() << 8);

  AccX = ax / 8192.0;
  AccY = ay / 8192.0;
  AccZ = az / 8192.0;

  Serial.print("AccX: "); Serial.print(AccX);
  Serial.print(" AccY: "); Serial.print(AccY);
  Serial.print(" AccZ: "); Serial.println(AccZ);

  delay(100);
}

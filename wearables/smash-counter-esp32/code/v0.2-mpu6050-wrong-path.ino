// Smash Counter ESP32 v0.2 — MPU-6050 Wrong Path
// Lishan, 14-18 May 2026
//
// ⚠️ THIS WAS THE WRONG DIRECTION ⚠️
//
// What happened:
// Claude gave MPU-6050 code on 14 May before confirming
// that the Waveshare board has QMI8658 IMU built in.
// Lishan spent 4 days setting up libraries, debugging,
// and trying to upload this code — all for the wrong sensor.
//
// The mistake:
// MPU-6050 is an external sensor — needs wires to connect.
// Our board (Waveshare ESP32-S3-Touch-LCD-1.28) has
// QMI8658 built in — no external sensor needed at all.
// This code is archived to document the wrong path taken.
//
// What Lishan learned anyway:
// - How to install Arduino IDE 2.3.8
// - How to add ESP32 board support via board manager URL
// - How to install libraries via Library Manager
// - How to use Arduino Web Editor when local IDE has issues
// - How to install Arduino Cloud Agent on Mac
// - How to handle Mac security blocking unsigned apps
// - How to upload code to ESP32 via USB-C
// - What baud rate means in Serial Monitor
//
// Timeline:
// 14 May — Arduino IDE installed, libraries added, code written
// 15 May — Switched to Arduino Web Editor (Mac out of space)
// 18 May — Board arrived, Cloud Agent installed, code uploaded
//
// What to use instead:
// See v0.3-qmi8658-hello-world.ino — uses built-in QMI8658 sensor.
//
// Libraries used (wrong ones for our board):
// - Adafruit MPU6050 by Adafruit
// - Adafruit Sensor by Adafruit
// - MPU6050 by Electronic Cats

#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <Wire.h>

Adafruit_MPU6050 mpu;
int smashCount = 0;
bool smashDetected = false;

void setup() {
  Serial.begin(115200);
  if (!mpu.begin()) {
    Serial.println("MPU6050 not found!");
    while (1);
  }
  Serial.println("Ready! Smash counter starting...");
}

void loop() {
  sensors_event_t a, g, temp;
  mpu.getEvent(&a, &g, &temp);

  float totalAccel = sqrt(a.acceleration.x * a.acceleration.x +
                          a.acceleration.y * a.acceleration.y +
                          a.acceleration.z * a.acceleration.z);

  if (totalAccel > 25 && !smashDetected) {
    smashCount++;
    smashDetected = true;
    Serial.print("SMASH! Count: ");
    Serial.println(smashCount);
  }

  if (totalAccel < 15) {
    smashDetected = false;
  }

  delay(50);
}

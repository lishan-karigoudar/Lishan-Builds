// Smash Counter ESP32 v0.1 — Arduino IDE Setup
// Lishan, 14 May 2026
//
// First day on ESP32. Arduino IDE 2.3.8 installed.
//
// Setup steps completed:
// 1. Downloaded Arduino IDE 2.3.8
// 2. Added ESP32 board support via board manager URL:
//    https://raw.githubusercontent.com/espressif/arduino-esp32/gh-pages/package_esp32_index.json
// 3. Installed ESP32 board package
// 4. Libraries installed: Servo@1.3.0, Stepper@1.1.3
//
// Key learning from this session:
// We do NOT need the MPU-6050 sensor.
// Earlier plan (XIAO ESP32C3) had no built-in IMU — needed external MPU-6050.
// Waveshare ESP32-S3-Touch-LCD-1.28 has QMI8658 6-axis IMU built in.
// Accelerometer + gyroscope already on the board. Nothing extra needed.
//
// Hardware confirmation chain:
// ESP32-S3 chip — dual core 240MHz ✅
// QMI8658 IMU built in ✅
// 1.28 inch round touch display built in ✅
// Battery header built in ✅
// Edge Impulse supports ESP32-S3 ✅
// Arduino IDE supports ESP32-S3 ✅
//
// Components arriving in 1-2 days.
// Next step: select ESP32-S3 board in Arduino IDE,
// connect board via USB-C, upload first hello world sketch.
//
// Note: "No board selected" shown in IDE — board not arrived yet.
// This sketch is ready to upload the moment board arrives.

void setup() {
  // runs once when board powers on
  Serial.begin(115200);
  Serial.println("Hello Lishan! ESP32 is alive!");
}

void loop() {
  // runs forever after setup
  // nothing here yet — board not arrived
}

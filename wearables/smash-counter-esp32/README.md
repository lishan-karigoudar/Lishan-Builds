# Smash Counter — ESP32 v2

**Status:** 🔄 In progress — hardware ordered  
**Hardware:** Waveshare ESP32-S3-Touch-LCD-1.28  
**Started:** 11 May 2026  
**Target:** Demo Day — March 2027

## Why we moved from micro:bit

micro:bit served its purpose perfectly — 22 versions, 
court tested, 100% ML accuracy. But player feedback was clear:

- LED display not readable on court
- Bluetooth unreliable during smash impact
- Too big for a comfortable wearable
- Not durable enough for daily court use

Every problem pointed to the same solution — better hardware.

## The hardware

Waveshare ESP32-S3-Touch-LCD-1.28 — everything in one board:

| Feature | Detail |
|---------|--------|
| Processor | Dual core LX7 240MHz |
| Display | 1.28 inch round touch, 240×240, 65K colors |
| IMU | QMI8658 6-axis — accelerometer + gyroscope |
| Connectivity | WiFi + BLE 5 built in |
| Battery | 3.7V MX1.25 LiPo header |
| Memory | 2MB PSRAM, 16MB Flash |
| Price | ₹2,049 from Hubtronics.in |

Battery: 600mAh LiPo — approximately 4 hours, 2 full sessions  
Strap: 20mm NATO strap — already owned  
Case: 3D printed sealed enclosure — in progress  

## Feature roadmap

### Demo day — March 2027
- Smash counter per session
- Shot power — LOW / MED / HIGH
- Session timer
- Live data on round watch display
- Basic BLE to phone

### 3 months
- Shot type detection — smash vs drop vs clear
- Fatigue indicator — power drop over session
- Personal best tracking

### 6 months
- Individual ML model per player
- Coach dashboard — one coach tracks 30 kids

## The pitch
"Phase 1 — counts your smashes and power.  
Phase 2 — learns YOUR game and tells you when you're improving.  
Phase 3 — connects to a coach dashboard so one coach  
can track 30 kids at once."

## Confirmed demo venue
Alwin — restaurant owner — has offered a kiosk spac

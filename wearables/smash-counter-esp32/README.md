# Smash Counter — ESP32 v2

**Status:** 🔄 Active — v0.21 working wirelessly on battery
**Hardware:** Waveshare ESP32-S3-Touch-LCD-1.28  
**Started:** 11 May 2026  
**Last updated:** 2 June 2026  
**Next session:** Weekend — school started
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

## Versions

### Wrong paths — archived for learning
| Version | Date | Status | Notes |
|---------|------|--------|-------|
| v0.1 | 14 May | ✅ | Arduino IDE setup |
| v0.2 | 14-18 May | ⚠️ Wrong path | MPU-6050 wrong sensor |
| v0.3 | 18 May | ⚠️ Wrong path | QMI8658 first attempt — baud issues |
| v0.4 | 18-20 May | ⚠️ Wrong path | LSM6DS3 wrong chip |
| v0.5 | 20 May | ✅ | QMI8658 correct pins confirmed |

### Working versions
| Version | Date | Status | Notes |
|---------|------|--------|-------|
| v0.6 | 21 May | ✅ | First working smash detection |
| v0.7 | 21 May | ✅ | Threshold tuned to 10g |
| v0.8 | 21 May | ⚠️ | Display first attempt wrong pins |
| v0.9 | 21 May | ✅ | Display correct pins |
| v0.10 | 21 May | ✅ | Display + reset button |
| v0.11 | 21 May | ✅ | Splash screen + best score |
| v0.12 | 21 May | ✅ | yield() crash fix |
| v0.13 | 21 May | ✅ | Persistent best score — Lishan's idea |
| v0.14 | 21 May | ✅ | Power bar B/G/O/R — Lishan's idea |
| v0.15 | 27 May | ✅ | Session timer HH:MM:SS |
| v0.16 | 27 May | ✅ | Smashes per minute |
| v0.17 | 27 May | ⚠️ | BLE blocked by library conflict |
| v0.18 | 27 May | ✅ | WiFi dashboard — pivot from BLE |
| v0.19 | 27 May | ✅ | Dual core — WiFi stable |
| v0.20 | 2 Jun | ✅ | Deep sleep long press |
| v0.21 | 2 Jun | ✅ Active | Timer based display — fully stable |

## Media
Photos, videos, and CAD files → [Google Drive](LINK_TO_BE_ADDED)

# Smash Counter — micro:bit v1

**Status:** Active — ML gesture detection working  
**Hardware:** BBC micro:bit V2, CR2025 battery, custom wristband mount  
**Started:** 12 April 2026

## What it does
Detects badminton smash shots using ML gesture recognition.
Trained on createai.microbit.org — recognises smash vs still.
Smash accuracy: 100%. Still accuracy: 99%.
Button A resets counter. Button B shows current count.

## The journey
Started with block coding — 19 attempts to get the logic right.
Moved to JavaScript when counting flag logic got too complex for blocks.
Tested 4 different acceleration thresholds on a real court.
Hit hardware ceiling — micro:bit accelerometer maxes out at 8G.
Moved to ML gesture detection — AI learns the shape of the motion.
First ML model trained 6 May 2026. 100% accuracy first time.

## Versions

### Threshold based detection — April 2026
| Version | File | Date | Status | Notes |
|---------|------|------|--------|-------|
| v0.1 | v0.1-shake-detection.ts | 12 Apr 2026 | ✅ Done | First attempt — shake gesture, too sensitive |
| v0.2 | v0.2-acceleration-threshold.ts | 14 Apr 2026 | ✅ Done | Moved to JS, threshold 1200mg |
| v0.3 | v0.3-threshold-1500-counting-flag.ts | 14 Apr 2026 | ✅ Done | 1500mg + counting flag, fixed double counting |
| v0.4 | v0.4-threshold-2500-testing.ts | 16 Apr 2026 | ✅ Done | 2500mg too high, missed light smashes |
| v0.5 | v0.5-final-1500mg.ts | 18 Apr 2026 | ✅ Done | 1500mg final, court tested, on wrist with battery |
| v0.6 | v0.6-power-meter-led-bars.ts | 23 Apr 2026 | ✅ Done | First power meter with LED bars |
| v0.7 | v0.7-power-text-pwr-med-low.ts | 23 Apr 2026 | ✅ Done | Text labels PWR/MED/LOW |
| v0.8 | v0.8-raw-number-display.ts | 23 Apr 2026 | ✅ Done | Raw number display for data collection |
| v0.9 | v0.9-tuned-thresholds-4000-2000.ts | 23 Apr 2026 | ✅ Done | Tuned thresholds from real data |
| v0.10 | v0.10-shake-gesture-power.ts | 23 Apr 2026 | ✅ Done | Switched to shake gesture |
| v0.11 | v0.11-peak-detection-loop.ts | 26 Apr 2026 | ✅ Done | Peak detection loop, 10 samples over 500ms |
| v0.12 | v0.12-peak-detection-final.ts | 26 Apr 2026 | ✅ Done | Peak detection with raw number display |
| v0.13 | v0.13-continuous-peak-real-data.ts | 27 Apr 2026 | ✅ Done | Forever loop peak, real court data used |
| v0.14 | v0.14-combined-counter-power.ts | 27 Apr 2026 | ✅ Done | Combined counter + power, overcounting problems |
| v0.15 | v0.15-counting-flag-fix.ts | 27 Apr 2026 | ✅ Done | Counting flag fix, still undercounting |
| v0.16 | v0.16-button-based-counting.ts | 27 Apr 2026 | ✅ Done | Button based counting, all showing PWR |
| v0.17 | v0.17-smart-button-final.ts | 27 Apr 2026 | ✅ Done | Final threshold version, hardware limit found |

### ML gesture detection — May 2026
| Version | File | Date | Status | Notes |
|---------|------|------|--------|-------|
| v0.18 | v0.18-bluetooth-no-pairing-required.ts | 6 May 2026 | ✅ Done | Bluetooth fix — no pairing required mode |

## Real court data
Collected 26 April 2026:  
Gentle shot = 1555mg  
Medium shot = 2896mg  
Hardest smash = 3533mg  

## ML model
Platform: createai.microbit.org  
Project: badminton shot detector  
Actions trained: Smash, Still  
Samples per action: 7  
Smash accuracy: 100%  
Still accuracy: 99%  

## Folders
- `code/` — All versions of the code
- `hardware/` — Circuit diagrams, wristband photos, measurements

## Media
Photos, videos, and CAD files → [Google Drive](https://drive.google.com/drive/folders/1QbrVaggSvclCn4dF7djXamwQNvUSNRC0?usp=sharing)

# Smash Counter — micro:bit v1

**Status:** Active — court tested with players  
**Hardware:** BBC micro:bit V2, CR2025 battery, custom wristband mount  
**Started:** June 2025

## What it does
Counts badminton smash shots using acceleration threshold detection on the wrist.
Displays count on micro:bit LED display.
Button A resets counter. Button B shows current count.

## The journey
Started with block coding — 19 attempts to get the logic right.
Moved to JavaScript when the counting flag logic got too complex for blocks.
Tested 4 different acceleration thresholds on a real court before finding
the right value. Every version is saved in the code folder.

## Versions
| Version | File | Status | Notes |
|---------|------|--------|-------|
| v0.1 | v0.1-shake-detection.ts | ✅ Done | First attempt — shake gesture, too sensitive |
| v0.2 | v0.2-acceleration-threshold.ts | ✅ Done | Moved to JS, threshold 1200mg, still too sensitive |
| v0.3 | v0.3-threshold-1500-counting-flag.ts | ✅ Done | 1500mg + counting flag, solved double counting |
| v0.4 | v0.4-threshold-2500-testing.ts | ✅ Done | Tested 2500mg on court, missed light smashes |
| v0.5 | v0.5-final-1500mg.ts | ✅ Active | Back to 1500mg, court tested, on wrist with battery |

## Folders
- `code/` — All versions of the code
- `hardware/` — Circuit diagrams, wristband photos, measurements

## Media
Photos, videos, and CAD files → [Google Drive](https://drive.google.com/drive/folders/1QbrVaggSvclCn4dF7djXamwQNvUSNRC0?usp=sharing)

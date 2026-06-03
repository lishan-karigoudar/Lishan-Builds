# Build Log — Lishan Builds

Newest entry on top. One entry per build session.
Every version. Every failure. Every fix.

---

## Entry 006 — 2 June 2026
**Project:** Smash Counter — micro:bit v1  
**What I worked on:**  
Set up the full GitHub repository and Google Drive archive.
Uploaded all 17 code versions from v0.1 to v0.17.
Uploaded all 19 block coding screenshots to Drive.
Everything is now preserved forever.

**What I learned:**  
GitHub saves every version with history.
Google Drive stores all photos, videos and circuits.
The two systems work together.

**Next session:**  
Plan v2 on ESP32 with better IMU.
Talk to more players before building next version.

---

## Entry 005 — 27 April 2026
**Project:** Smash Counter — micro:bit v1  
**Versions:** v0.13 → v0.17  
**What I worked on:**  
Combined smash counting and power measurement into one device.
Tried 5 different architectures in one day.
Discovered the core hardware limitation of micro:bit.

**Test results:**  
v0.14: 5 smashes → counted 8. Overcounting.  
v0.14: 5 gentle shots → counted 14. Way overcounting.  
v0.15: 3 hard smashes → 1 counted. Undercounting.  
v0.16: All shots showed PWR regardless of power.  
v0.17: Button based counting worked but classification wrong.  

**What I learned:**  
The forever loop accumulates peak from ALL movements —
walking, arm swing, setup — not just the smash impact.
micro:bit cannot isolate just the smash cleanly.
This is a hardware limitation, not a code problem.
Need a better IMU sensor for v2.

**Break:** Kerala trip April 29 to May 3. No coding.

---

## Entry 004 — 26 April 2026
**Project:** Smash Counter — micro:bit v1  
**Versions:** v0.11 → v0.12  
**What I worked on:**  
Invented peak detection loop — samples 10 readings over 500ms.
Takes the highest reading instead of one instant snapshot.
Much more consistent results.

**What I learned:**  
A smash happens in milliseconds.
One reading misses it. Ten readings catch it.
Sample fast, take the peak. This is how real sensors work.

---

## Entry 003 — 23 April 2026
**Project:** Smash Counter — micro:bit v1  
**Versions:** v0.6 → v0.10  
**What I worked on:**  
Two days after first perfect test, started measuring smash POWER.
Built power meter with LED bars, then text labels PWR/MED/LOW.
Used raw number display to collect real data.
Tested ThreeG gesture vs Shake gesture.

**Real court data collected:**  
Gentle shot = 1555mg  
Medium shot = 2896mg  
Hardest smash = 3533mg  

**What I learned:**  
Don't guess thresholds. Measure first.
Set thresholds from real data.
This is how real engineers calibrate sensors.

---

## Entry 002 — 21 April 2026
**Project:** Smash Counter — micro:bit v1  
**Version:** v0.5  
**Session:** First ever real court test

**Test results:**  
Smashes done: 10  
Micro:bit counted: 10  
Accuracy: 100% — PERFECT!!

**Notes:**  
IT WORKS!!!

**Photos:**  
- Wristband on court → [Drive: v1-photos/wristband-v1](https://drive.google.com/file/d/1e8mn4nAJdPZ4sxEAMSW4wMVbZDKqAn3A/view?usp=sharing)
- Bluetooth connected to phone → [Drive: v1-photos/bluetooth-connection](https://drive.google.com/file/d/1pYC77bbWyEmGQlvaE6QC_viGRAoTKQMf/view?usp=sharing)

---

## Entry 001 — 12 April 2026
**Project:** Smash Counter — micro:bit v1  
**Versions:** v0.1 → v0.5  
**What I worked on:**  
Started the smash counter project on BBC micro:bit V2.
First tried block coding — 19 attempts to get the logic right.
Blocks got too complicated for the counting flag logic.
Moved to JavaScript — much cleaner.
Tested 4 different acceleration thresholds.
1200mg too sensitive. 2500mg missed smashes. 1500mg is the sweet spot.
Connected to CR2025 battery for wireless use.
Tested Bluetooth connection with phone.

**What worked:**  
Counting flag solved the double counting problem completely.
1500mg threshold works reliably for real smash detection.

**What I learned:**  
There is no perfect threshold — it depends on the player.
Hard hitters need higher threshold, junior players need lower.
Future versions should let the player set their own threshold.

---

# Build Log — Lishan Builds

Newest entry on top. One entry per build session.
Every version. Every failure. Every fix.

------

## Entry 024 — 27 May 2026 (Session 3)
**Project:** Smash Counter — ESP32 v2  
**What happened:**  
BLE blocked by library conflict. Pivoted to WiFi dashboard.
Phone dashboard working — best result of the project so far.

**BLE library conflict:**  
Old ESP32_BLE_Arduino 1.0.1 stuck in Arduino web editor.  
Cannot be removed from UI — server-side cache issue.  
Multiple attempts — new sketch, library manager, custom libraries.  
All failed. Same error every time:  
ringbuf_type_t has not been declared  
Arduino forum thread found for future reference:  
https://forum.arduino.cc/t/how-do-i-force-cloud-editor-to-use-the-correct-built-in-esp32-ble-library/1409577  

**Pivot to WiFi hotspot — v0.18:**  
ESP32 creates WiFi network "LishanTech" password "smash123"  
Phone connects, opens browser at 192.168.4.1  
Live dashboard auto-refreshes every second  
No app needed — works on any phone browser  

**Dashboard shows:**  
🏸 LishanTech title  
Live smash count — big cyan number  
Stats row: per minute, last peak G, best score  
Intensity badge — colour coded  
Session timer  

**Why WiFi is better than BLE for demo day:**  
No app needed — just a browser ✅  
Anyone can connect — judges, players, parents ✅  
Full dashboard visible — not just raw data ✅  
Works on any phone ✅  
Auto-refreshes every second ✅  

**First test result — 27 May 2026:**  
Phone connected to LishanTech WiFi.  
Dashboard loaded at 192.168.4.1.  
Best score showing 51.  
Live updates confirmed working.  

**Sketch renamed:**  
From new_sketch_1779335655264 → Lishan_Wearable_Tech  
Lishan named it himself.  

**Full feature status — end of 27 May:**  
✅ Splash screen  
✅ Smash count on round display  
✅ Power bar B/G/O/R  
✅ Persistent best score  
✅ Reset button  
✅ Session timer HH:MM:SS  
✅ Smashes per minute  
✅ WiFi dashboard on phone — LishanTech  
⚠️ Battery level indicator — pending battery arrival  
⚠️ Haptic motor — pending robu.in delivery  

**Next:**  
Court test with real players.  
Battery arrives — connect wirelessly.  
Haptic motor — when robu.in delivers.  

---

## Entry 023 — 27 May 2026 (Session 2)
**Project:** Smash Counter — ESP32 v2  
**What happened:**  
Smashes per minute and BLE broadcast both added in one session.

**v0.16 — Smashes per minute:**  
SPM uses rolling window of last 10 smash timestamps.  
Every second: count how many timestamps within last 60 seconds.  
That count = current SPM shown on screen in yellow.  
First attempt had layout issue — SPM stuck in corner.  
Fixed by adjusting y coordinates — all three lines now visible:  
HH:MM:SS timer — y=172 cyan  
SPM: X — y=187 yellow  
BEST: XX — y=202 red  

**v0.17 — BLE broadcast:**  
ESP32 advertises as "SmashCounter" — easy to find on phone.  
nRF Connect app installed on phone for testing.  
JSON payload sent per smash:  
session_id, smash_number, peak_g, intensity, spm, best  
Intensity classification:  
light = below 5g  
medium = 5g to 8g  
hard = 8g to 12g  
max = above 12g  
Auto-reconnect: if phone disconnects, ESP32 restarts advertising.  
Session ID increments on each BOOT button press.  
Libraries used: BLEDevice, BLEServer, BLEUtils, BLE2902  
All built into ESP32 Arduino core — no extra install needed.  

**Full feature status — end of 27 May:**  
✅ Splash screen — by Lishan v1.0 2026  
✅ Smash count on round display  
✅ Power bar B/G/O/R  
✅ Persistent best score  
✅ Reset button  
✅ Session timer HH:MM:SS  
✅ Smashes per minute  
✅ BLE broadcast as SmashCounter  
⚠️ Battery level indicator — pending battery arrival  
⚠️ Haptic motor — pending robu.in delivery  
⚠️ MIT App Inventor phone app — next  

**Remaining from roadmap:**  
Battery level indicator — after battery arrives  
Haptic motor — after robu.in delivery  
MIT App Inventor phone app  
Court demo with real players  

---

## Entry 022 — 27 May 2026
**Project:** Smash Counter — ESP32 v2  
**What happened:**  
Hardware ordered. GPIO pins verified from schematic.
Session timer added and working.

**Critical schematic correction:**  
GPIO4 and GPIO5 listed in roadmap as free — WRONG.  
Actual schematic confirmed:  
GPIO4 = IMU_INT1 — occupied ⚠️  
GPIO5 = TP_INT touch interrupt — occupied ⚠️  
Correct free GPIOs: 15, 16, 17, 18, 21, 33  
Vibration motor will use GPIO15 — confirmed free.  
Roadmap updated accordingly.  

**Hardware ordered — robu.in:**  
Vibration motor 3V 4×8mm × 2 — ₹134  
BC547 NPN transistor × 6 — ₹11.94  
JST 1.25mm 2-pin connector × 2 — ₹24  
1K ohm resistor through-hole × 10 — ₹22.60  
Flat shipping — ₹50  
Total — approximately ₹242  

**v0.15 — Session timer working:**  
Timer shows HH:MM:SS counting up from session start.  
Updates every 1 second — not every loop iteration.  
Resets when BOOT button pressed.  
No extra library — uses millis() built into Arduino.  
Lishan asked about hours — upgraded from MM:SS to HH:MM:SS.  

**Screen layout now complete:**  
Top: SMASH COUNTER title  
Centre: large cyan smash count  
Below number: BEST: XX in red  
Bottom: HH:MM:SS timer in cyan  
Right side: power bar B/G/O/R  

**Next:**  
Add smashes per minute.  
Components arrive — connect battery wirelessly.  
Add battery level indicator.  
Start BLE broadcast.  

---

## Entry 021 — 27 May 2026
**Project:** Smash Counter — ESP32 v2  
**What happened:**  
Hardware planning and order finalised for remaining components.

**Components ordered from robu.in:**  
Vibration motor 3V 4×8mm × 2 — ₹67 each — one spare  
BC547 NPN transistor × 6 — ₹1.99 each — spares for future  
JST A1250-02Y 1.25mm 2-pin female connector × 2 — ₹12 each  
1K ohm resistor through-hole × 10 — ₹2.26 each  
Flat shipping — ₹50  
Total — approximately ₹220  

**Why each component:**  
Vibration motor — haptic feedback on each smash  
BC547 — transistor to switch motor from GPIO pin safely  
JST connector — battery connection to board BAT socket  
1K resistor — protects GPIO pin from BC547 base current  

**GPIO pins verified from schematic:**  
Schematic reviewed before ordering — no assumptions.  
Free GPIO pins on SH1.0 connector:  
GPIO1 — ADC for battery voltage reading  
GPIO3 — free — available for motor  
GPIO4 — free — available for motor  
GPIO5 — free — available for motor  
GPIO6 — SDA I2C — already used by QMI8658  
GPIO7 — SCL I2C — already used by QMI8658  
Motor will use GPIO3 — confirmed free.  

**Soldering decision:**  
Soldering kit not ordered yet.  
Only 2 joints needed now — battery wires to JST connector.  
Papa will supervise first-time soldering.  
Full kit to be bought when Arduino Nano v2 phase begins.  

**Schematic saved:**  
Drive: v1-hardware/ESP32-S3-Touch-LCD-1.28-schematic.png  

**Next:**  
Components arrive — connect battery wirelessly.  
Add battery level indicator to display.  
Add session timer.  
Add smashes per minute.  

---

## Entry 020 — 21-26 May 2026
**Project:** Smash Counter — ESP32 v2  
**What happened:**  
Trip 21-25 May. No coding.  
Returned 26 May. Code still working perfectly.  
Full Demo Day feature set defined with Papa.

**JST connector sorted — 21 May:**  
Battery has bare wires — needs JST 1.25mm 2 pin connector.  
Found on Sharvi Electronics, Bangalore — ₹12 each.  
No soldering needed on board side — just connector on battery.  
Battery has 3 wires (red/black/white) — use only red and black.  

**Back from Davangere — 26 May:**  
Opened app.arduino.cc — code still there.  
Plugged ESP32 — uploaded — 5 smashes counted cleanly. ✅  
Everything working after 5 days away.  

**Full Demo Day feature set agreed — 26 May:**  

On device LCD:  
- Splash screen ✅ done  
- Live smash count ✅ done  
- Power bar B/G/O/R ✅ done  
- Persistent best score ✅ done  
- Battery level indicator 🔲 pending  
- Session timer MM:SS 🔲 pending  
- Smashes per minute 🔲 pending  
- Haptic buzz on smash 🔲 pending  
- Reset button ✅ done  

BLE + Phone App:  
- BLE broadcast per smash 🔲 pending  
- Live smash count on phone 🔲 pending  
- Live intensity on phone 🔲 pending  
- Session history 🔲 pending  
- MIT App Inventor app — Lishan builds it 🔲 pending  

**BLE data model agreed:**  
Each smash broadcasts:  
session_id, timestamp_ms, smash_number, peak_g,  
intensity, smashes_per_min, session_best, battery_pct  

**Hardware still needed:**  
- JST 1.25mm connector — ₹12 — Sharvi Electronics  
- NATO strap — ₹200-300 — Amazon  
- 3D printed case — ₹100-200 — local shop  
- Vibration motor 3V — ₹30-50 — robu.in  
- Small MOSFET — ₹10 — robu.in  
Total: approximately ₹400-600  

**Build sequence to Demo Day:**  
1. ✅ Smash counter + LCD + power bar + best score  
2. 🔲 Battery level indicator  
3. 🔲 Session timer  
4. 🔲 Smashes per minute  
5. 🔲 BLE broadcast  
6. 🔲 Haptic motor  
7. 🔲 MIT App Inventor phone app  
8. 🔲 Integration testing  
9. 🔲 Court demo with 4-5 players — tune threshold  
10. 🔲 Final polish — Demo Day March 2027  

**Non-negotiable gate:**  
Court demo with real players after step 7.  
Their feedback shapes final UI and threshold.  
No Demo Day without real court validation.  

**Roadmap document:**  
Full product bible saved to Drive:  
smash-counter-demo-day-roadmap-26may2026.docx  
Not on GitHub — planning document stays in Drive.  

---

## Entry 019 — 21 May 2026 (Session 2)
**Project:** Smash Counter — ESP32 v2  
**What happened:**  
After display and reset button working — Lishan kept building.
Four more features added in one afternoon.

**v0.11 — Splash screen + best score:**  
Splash screen shows "SMASH COUNTER by Lishan v1.0 2026" on startup.  
Best score tracks highest count in session.  
Crashed after 14 smashes — watchdog timer error.  

**v0.12 — yield() crash fix:**  
yield() added inside all drawing functions.  
Tells ESP32 to reset watchdog timer during long draw operations.  
No more crashes even after many smashes. ✅  

**v0.13 — Persistent best score — Lishan's idea:**  
Lishan's exact words:  
"If you do 38 smashes in 1 match it will show 38 highest score  
and even if reset your progress wont be lost."  
Used ESP32 Preferences library — saves to internal flash memory.  
Best score survives reset, power off, battery removal.  
This is called non-volatile storage — real products use this.  

**v0.14 — Power bar — Lishan's idea:**  
Lishan's exact words:  
"I also thought about keeping smash level on the right side  
which is vertical — light smash = blue, medium = green,  
normal = orange, hardest = red."  
Vertical bar on right side of round screen.  
Updates continuously — not just on smash detection.  
Blue ✅ Green ✅ Orange ✅ confirmed in testing.  
Red needs real court smash — not triggered sitting in chair.  

**Battery discussion:**  
Lishan raised soldering concern unprompted:  
"If battery spoils we will never be separated and USB and  
battery together will make ESP32-S3 heat up blast to ashes."  
This is correct engineering thinking.  
Solution confirmed: MX1.25 is plug/unplug — not permanent.  
Existing 3.7V batteries work — just need MX1.25 connector.  
Cost: ₹9 per connector — ₹45 for 5 from robu.in.  
Battery has 3 wires (red/black/white) — only use red and black.  

**End of 21 May status:**  
✅ Splash screen — by Lishan v1.0 2026  
✅ Smash counter on round display  
✅ Power bar — blue green orange red  
✅ Persistent best score  
✅ Reset button  
✅ Threshold 10.0g  
⚠️ Still on USB — battery connector not yet ordered  
⚠️ Real court test pending  

**Lishan's ideas vs prompted features:**  
Persistent best score — Lishan's idea ✅  
Power bar with colours — Lishan's idea ✅  
These are the two best features in the product.  

**Next:**  
Order MX1.25 connectors from robu.in — ₹45 for 5.  
Papa solders connector to battery wires.  
Go wireless. Court test. Calibrate threshold.  

---

## Entry 018 — 21 May 2026
**Project:** Smash Counter — ESP32 v2  
**What happened:**  
The biggest single day of the project.
Sensor working → display working → reset button working.
All in one day.

**Threshold calibration:**  
Started at 2.5g — triggered by any movement.  
Tested: 4.0g → 6.0g → 10.0g  
10.0g feels right for hard smash motion without USB cable.  
Lishan's note: try 11.0g or 12.0g on real court with racket.  

**Display — first attempt — v0.8:**  
Installed Adafruit GC9A01A and Adafruit GFX libraries.  
Wrong SPI pins used — board crashed immediately.  

**Display — correct pins found — v0.9:**  
Correct pins from real user who got it working:  
DC=8, CS=9, MOSI=11, SCLK=10, RST=14, BL=2  
Display initialised — "SMASH COUNTER 0 smashes" on screen.  
First text on round LCD. 🎉  
Problem: board crashed after first smash.  
fillScreen() too heavy — called on every smash.  

**Display crash fix — v0.10:**  
Split drawScreen() into two functions:  
drawInitialScreen() — full screen draw once at startup only.  
drawNumber() — only redraws number area using fillRect().  
This fixed crash completely.  

**Reset button added — v0.10:**  
BOOT button on GPIO0 used as reset.  
Press BOOT → smashCount = 0 → screen redraws.  
No extra hardware needed — button already on board.  

**PSRAM attempt:**  
Tried OPI PSRAM to fix crash — error: PSRAM chip not found.  
Board has 2MB PSRAM but not OPI type.  
Turned back to Disabled. Not needed — drawNumber() fix worked.  

**Final status — end of 21 May:**  
Round display showing smash count ✅  
Smash detection at 10.0g ✅  
Reset button working ✅  
Still connected via USB ⚠️  
Battery connection attempted — session ended before confirming  

**Versions committed:**  
v0.7 — threshold tuning 10g  
v0.8 — display first attempt wrong pins  
v0.9 — display correct pins working  
v0.10 — display final with reset button  

**What Lishan said:**  
Went to play cricket between sessions — came back to finish.  
"I will play a test match and come — not real test match,  
just a game of 2 innings." 🏏  

**Next session:**  
Connect battery — cut USB — go wireless.  
Court test with real racket.  
Calibrate threshold for real smash force.  

---

## Entry 017 — 20-21 May 2026
**Project:** Smash Counter — ESP32 v2  
**What happened:**  
After 7 days of wrong paths — IT WORKS.  
First working ESP32 smash counter on 21 May 2026.

**20 May — Waveshare demo downloaded:**  
Official demo ZIP downloaded from Waveshare wiki.  
Contains: examples, libraries — LVGL, TFT_eSPI, QMI8658.  
Tried importing QMI8658.h, QMI8658.cpp, DEV_Config.h, DEV_Config.cpp  
into Arduino web editor one by one.  
Problem: Waveshare files all connected — LCD, touch, sensor together.  
Pulling sensor part out caused endless compile errors.  
DEV_Config.cpp had its own setup() and loop() — conflicting.  
Multiple file delete and reimport attempts.  
Hidden files in web editor causing phantom errors.  
Session ended without working code.  

**21 May — Fresh start, clean code:**  
Created brand new sketch — zero leftover files.  
Went back to direct I2C register reads — no library at all.  
Used math.h for sqrt() — no external dependency.  
Selected ESP32S3 Dev Module correctly.  
Wire.begin(6, 7) — SDA GPIO6, SCL GPIO7 confirmed.  
QMI8658 address 0x6B confirmed.  

**First output — 21 May 2026:**  
SMASH! Count: 1  
SMASH! Count: 2  
SMASH! Count: 3  
SMASH! Count: 4  
SMASH! Count: 5  
SMASH! Count: 6  
SMASH! Count: 7  
SMASH! Count: 8  

**What the garbage characters mean:**  
Boot messages from ESP32 — always appear at startup.  
Cannot be suppressed on this board. Normal. Ignore them.  
Everything after them is our code working correctly.  

**What made it finally work:**  
Fresh sketch — no leftover conflicting files  
Direct I2C — no library dependency  
Correct board, correct pins, correct address  
math.h included explicitly  

**Key learning:**  
When library imports cause endless errors —  
go back to basics. Direct I2C register reads.  
No library = no dependency = no conflict.  
This is how embedded engineers work at the lowest level.  

**Current status:**  
Smash detection working at 2.5g threshold.  
Still connected via USB — not wireless yet.  
Display not showing count yet — Serial Monitor only.  

**Next steps:**  
Calibrate threshold on real court.  
Add display output — show count on round LCD.  
Add battery — cut USB, go wireless.  

---

## Entry 016 — 18-20 May 2026
**Project:** Smash Counter — ESP32 v2  
**What happened:**  
Three days of debugging. Multiple wrong paths. Finally found root cause.
Documented fully and honestly.

**18 May — wrong sensor library:**  
After baud rate issues with v0.3, switched to LSM6DS3 library.  
LSM6DS3 is completely wrong chip — not on our board at all.  
Multiple library versions tried — none worked.  
LED debugging attempted — pins 21, 48, all 50 pins, NeoPixel.  
Nothing blinked. LED is WS2812 RGB — needs different approach.  
Session ended with no working output.  

**20 May — root cause found:**  
Baud rate finally fixed at 115200.  
ESP32 crashing with Guru Meditation Error.  
Board identification breakthrough — Lishan read the label:  
"esp32-s3-touch-LCD-1.28"  
Not XIAO ESP32S3 as assumed in IDE all along.  

**Six mistakes found and fixed:**  
1. Wrong board selected — XIAO_ESP32S3 → ESP32S3 Dev Module  
2. Wire.begin() missing pins → Wire.begin(6, 7) — SDA GPIO6, SCL GPIO7  
3. Wrong sensor address — 0x6A → 0x6B  
4. USB CDC on Boot disabled → must be Enabled  
5. Wrong ESP32 board version → needs exactly 2.0.12  
6. Wrong sensor chip entirely — LSM6DS3 vs QMI8658  

**How we found the right settings:**  
Santy shared full Waveshare wiki specs.  
Official manual confirmed all pin numbers and settings.  
No more guessing — everything from official documentation.  

**Official Waveshare demo downloaded:**  
Contains exact QMI8658 library files.  
Working Arduino code that reads the sensor.  
This is the definitive reference going forward.  

**Code archived:**  
v0.4-lsm6ds3-wrong-chip.ino — wrong path, kept for record  
v0.5-qmi8658-correct-pins.ino — first correct settings  

**What Lishan learned:**  
Read the label on the physical board before selecting it in IDE.  
Read the official manual before writing any code.  
Wire.begin() on ESP32 needs explicit pin numbers — not like Arduino Uno.  
USB CDC must be enabled for Serial.println to work on this board.  

**Next step:**  
Upload v0.5 with correct settings.  
Verify "Sensor ID: 05" appears in Serial Monitor.  
Confirm AccX AccY AccZ values printing.  
Then build smash detection on top.  

---

## Entry 015 — 18 May 2026 (Session 2)
**Project:** Smash Counter — ESP32 v2  
**What happened:**  
Lishan caught the MPU-6050 mistake himself.
Switched to correct QMI8658 sensor — built into the board.

**The moment:**  
After ESP32 went blank following upload —  
Lishan checked previous chat and asked directly:  
"In previous chat you told MPU-6050 is unnecessary  
and QMI8658 is better — which one are we ordering?"  
Answer: order nothing. Sensor already inside the board.  

**Why ESP32 went blank:**  
MPU-6050 code has while(1) loop —  
when sensor not found, board freezes completely.  
Looks dead. Is actually fine.  
Fixed by uploading new correct code.  

**v0.3 — First correct ESP32 code:**  
Uses QMI8658 library — Waveshare built-in sensor.  
Reads AccX, AccY, AccZ from built-in IMU.  
Prints to Serial Monitor at 115200 baud.  
No external sensor. No wires. No purchase needed.  

**Key learning:**  
Always verify which sensor the board has BEFORE writing code.  
Read the datasheet. Check the wiki. Then code.  
Never assume — always confirm.  

**Next step:**  
Upload v0.3 → open Serial Monitor at 115200 baud  
→ move board → watch AccX AccY AccZ change  
→ confirm built-in IMU is working  
→ then build smash detection on top of it.  

---

## Entry 014 — 14-18 May 2026
**Project:** Smash Counter — ESP32 v2  
**What happened:**  
Four days of hard work — on the wrong path.
Documented honestly because this is part of the journey.

**The mistake — Claude's error:**  
On 14 May, Claude gave MPU-6050 code without first confirming  
that the Waveshare board has QMI8658 IMU built in.  
Lishan spent 4 days on MPU-6050 libraries and setup.  
None of it was needed. The right sensor is already on the board.  
This is recorded so it never happens again.

**What Lishan did anyway — 14 May:**  
Installed Arduino IDE 2.3.8.  
Added ESP32 board support via board manager URL.  
Installed libraries: Adafruit MPU6050, Electronic Cats MPU6050.  
Wrote first ESP32 smash counter code — MPU-6050 version.  
Hit compile error — old code still in editor, pasted below it.  
Fixed by clearing editor and repasting.  
Hit second error — Mac needs Xcode developer tools.  
Xcode requires 20.85GB — only 12.38GB available on Mac.  
Cannot install. Switched to Arduino Web Editor instead.  

**Arduino Web Editor setup — 15 May:**  
Went to app.arduino.cc.  
Logged in with Papa's Arduino account.  
Verification email found in spam — verified successfully.  
Pasted MPU-6050 smash counter code into web editor.  
Left to go to badminton class.  

**Board arrived — 18 May:**  
ESP32-S3 board arrived with rainbow wires and 2 batteries.  
MPU-6050 still not arrived.  
Arduino Cloud Agent installed — Mac security blocked it.  
Fixed via System Preferences → Security & Privacy → Open Anyway.  
Agent installed successfully — Arduino icon visible in menu bar.  

**First successful upload — 18 May:**  
Connected ESP32 via USB-C.  
Board displayed: onboard parameters, flash size 16MB, battery voltage.  
Selected XIAO_ESP32S3 in web editor.  
Code uploaded successfully after several connection attempts.  
Serial monitor showed weird characters — baud rate was 9600.  
Needs to be changed to 115200 to fix.  

**What Lishan learned despite wrong path:**  
- Arduino IDE installation and board manager  
- Library installation via Library Manager  
- Arduino Web Editor as alternative to desktop IDE  
- Arduino Cloud Agent installation on Mac  
- Mac security settings for unsigned apps  
- USB-C code upload to ESP32  
- Serial Monitor and baud rate concept  
- Patience — 4 days of setup before one line of code ran  

**Code archived:**  
v0.2-mpu6050-wrong-path.ino — kept as record, not for use.  

**Next step:**  
Fix baud rate to 115200.  
Switch to QMI8658 library — the correct sensor for our board.  
Write hello world using built-in IMU.  

---

## Entry 013 — 14 May 2026
**Project:** Smash Counter — ESP32 v2  
**What happened:**  
First day on ESP32. Arduino IDE installed and configured.
Components not arrived yet — but software side ready.

**Arduino IDE setup:**  
Downloaded Arduino IDE 2.3.8.  
Added ESP32 board support via board manager:  
https://raw.githubusercontent.com/espressif/arduino-esp32/gh-pages/package_esp32_index.json  
Libraries installed: Servo@1.3.0, Stepper@1.1.3  

**Important clarification — no MPU-6050 needed:**  
Earlier plan used XIAO ESP32C3 — no built-in IMU — needed external MPU-6050.  
Final board is Waveshare ESP32-S3-Touch-LCD-1.28 — QMI8658 IMU built in.  
MPU-6050 order cancelled. Saving ₹150. Simplifies wiring completely.  

**Hardware path clarified:**  
ESP32-C3 — wrong path, abandoned early ✅  
ESP32-S3 — correct — this is what's in the Waveshare board ✅  

**Confirmation chain:**  
Waveshare wiki confirms QMI8658 IMU built in ✅  
Hubtronics.in has it in stock at ₹2,049 ✅  
STL case file confirmed fits the board ✅  
Edge Impulse supports ESP32-S3 ✅  
Arduino IDE supports ESP32-S3 ✅  

**Status:**  
Arduino IDE ready. Board not arrived yet — shows "No board selected".  
Hello world sketch prepared and waiting.  
Components arriving in 1-2 days.  

**Next session:**  
Board arrives → connect USB-C → select ESP32-S3 board in IDE  
→ upload hello world → confirm board is alive.  

**Photos:**  
- Arduino IDE installed → Drive: v1-hardware/arduino-ide-installed-14may2026.png  

---

## Entry 012 — 11-13 May 2026
**Project:** Smash Counter — ESP32 v2  
**What happened:**  
Three days of intense preparation before components arrive.

**STL file found — 11 May:**  
After calling 7+ local 3D printing vendors — all wanted STL file first.  
Searched Thingiverse and found exact case designed by bonybear:  
https://www.thingiverse.com/thing:7038776  
Designed specifically for Waveshare ESP32-S3-Touch-LCD-1.28.  
Includes battery cavity, USB-C slot, 20mm watch strap lugs.  
Two files downloaded: prizma_case_bottom.stl, prizma_case_top.stl  
Full credit to bonybear — this design made the wearable possible.

**Waveshare STEP file — 12 May:**  
Downloaded official 3D diagram from Waveshare wiki.  
Sent to 3D printer along with STL files.  
Printer confirmed — will measure board when hardware arrives,  
then edit case if needed and print once. No guessing.

**Battery challenge — 11 May:**  
STL case fits maximum 44×39×4mm battery.  
4mm thickness extremely hard to find in India with MX1.25 connector.  
Final resolution: printer will measure actual cavity from STL  
and confirm exact battery spec before we order.

**Components ordered — 11 May:**  
- ESP32-S3-Touch-LCD-1.28 — ₹2,049 — Hubtronics.in  
- LiPo 3.7V 500mAh MX1.25 — ₹250 — robu.in  
- 2× M2 12mm screws + nuts — ₹20 — local hardware  
Arriving in 3-4 days.

**Study guide created — 12 May:**  
Comprehensive 10 chapter document for Lishan.  
Covers: full journey, all components, how everything works,  
features roadmap, timeline to demo day, key concepts.  
Saved in Drive for reference while waiting for hardware.

**3D printer confirmed — 13 May:**  
Vendor confirmed — once board arrives, he will:  
Measure board physically with his tools.  
Verify fit against STL case.  
Edit if needed. Print in PETG. One go.

**Files uploaded to Drive:**  
- prizma_case_bottom.stl  
- prizma_case_top.stl  
- ESP32-S3-Touch-LCD-1.28.zip (Waveshare official 3D diagram)  
All in: Drive/wearables/smash-counter-esp32/v1-hardware/

**Next:**  
Components arrive. Take to printer.  
Start Arduino IDE setup.  
Hello world on round display.

---

## Entry 011 — 10-11 May 2026
**Project:** Smash Counter — ESP32 v2 planning  
**What happened:**  
Returned from Kerala trip. Started planning Phase 2 hardware seriously.

**Hardware decision — final:**  
After researching 10+ options over two days, final hardware confirmed:

Waveshare ESP32-S3-Touch-LCD-1.28  
- 1.28 inch round touch display built in  
- QMI8658 6-axis IMU built in — no separate MPU-6050 needed  
- 3.7V LiPo battery header built in  
- Dual core 240MHz processor  
- WiFi + BLE 5 built in  
- Edge Impulse compatible  
- Price: ₹2,049 from Hubtronics.in — in stock  

Battery: 600mAh LiPo with MX1.25 connector — ₹250  
Strap: existing 20mm NATO strap from STEM Smart Labs kit ✅  

Total hardware cost: approximately ₹2,30

## Entry 010 — 10 May 2026
**Project:** Smash Counter — micro:bit v1  
**Version:** v0.22 — FINAL micro:bit version  
**What I worked on:**  
Added session timer to Button B.
Now shows smash count AND minutes played in one button press.
Lishan's own idea — not prompted.

**New feature — Session Timer:**  
Button B shows: smash count → pause → T → minutes played  
Helps players track smashes per minute, not just total smashes.  
A player can compare: did I smash more per minute this week?  

**Player feedback continuing:**  
3rd friend still thinking — hasn't decided yet.  
Other friends asking: "Is it a watch?", "What program is in it?"  
Lishan's answer: "It's a micro:bit that helps at badminton.  
If you want to get better at smash, it counts only s

## Entry 009 — May 2026
**Project:** Smash Counter — micro:bit v1  
**Version:** v0.21  
**What I worked on:**  
Improved ML model from 7 to 20 samples per action.
Much stronger recognition — more data = better model.
Ran first real data session — 68 smashes logged with timestamps.
Showed the device to real people for the first time.

**ML model:**  
Platform: createai.microbit.org  
Smash samples: 20  
Still samples: 20  

**Data session:**  
68 smashes logged via Data Logger.
Retrieved via MY_DATA.HTM after session.

**Player and adult feedback — 15-18 kids, 6-7 adults:**  
- Display not readable on court — LED matrix too small
- Bluetooth connection dropping during play
- Not durable enough for court use
- Wearable mount uncomfortable
- One kid: "it looks huge"
- Adults: "work on the wearable form factor"
- One adult: won't buy, already spent money on son's LEGO
- One adult laughed and walked away without engaging

**What this means:**  
The technology works. The product needs work.
micro:bit is a prototype tool, not a final product.
Every piece of feedback points to the same solution — ESP32.
Onboard LCD display solves readability.
Better form factor solves size and wearable issues.
No Bluetooth needed solves connection drops.

**Key milestone:**  
Alwin — restaurant owner — gave his number.
Offered kiosk demo space in his restaurant
when ESP32 version is ready.
First external demo venue confirmed.

**Next step:**  
Move to ESP32-S3-Touch-LCD-1.28.
Solve display, durability, size, wearable in one step.

**Photos:**  
- 20 sample ML training → Drive: v1-photos/ml-20samples-training.png
- v0.21 code → Drive: v1-photos/ml-v0.21-code-with-still-event.png

---

## Entry 008 — 6 May 2026 (Session 2)
**Project:** Smash Counter — micro:bit v1  
**Versions:** v0.19 → v0.20  
**What I worked on:**  
Took the trained ML model and built the first working ML smash counter.
Then Lishan asked a key question on his own:
"Why do we need to connect to the computer to get data?
Can we add memory so it stores by itself?"

**The insight:**  
micro:bit V2 has a built-in Data Logger.
Stores every smash with timestamp directly to internal flash memory.
No Bluetooth needed during court session.
After session: plug USB, open MY_DATA.HTM, see all data.
Download as CSV for analysis.

**This is exactly how professional sports data loggers work.**  
Record during session. Analyse after.

**Versions built:**  
- v0.19 — First ML smash counter using trained model  
- v0.20 — ML + Data Logger, timestamps every smash  

**Photos:**  
- ML + DataLogger in MakeCode → Drive: v1-photos/ml-datalogger-makecode-6may2026.png

---

## Entry 007 — 6 May 2026
**Project:** Smash Counter — micro:bit v1  
**Platform:** createai.microbit.org — ML gesture detection  
**What I worked on:**  
Hit the hardware ceiling on threshold-based detection.
micro:bit accelerometer maxes out at 8G — hard smash exceeds this.
Gentle and hard shots both reading above 3000mg — no difference.
Decision: move to ML-based gesture recognition.

**What is ML gesture detection:**  
Instead of checking if a number is above a threshold,
the AI learns the SHAPE of the motion — the pattern of
x, y, z acceleration over time.
A smash has a completely different shape to standing still.
The AI sees this even when the raw numbers look similar.

**What I did:**  
- Created project "badminton shot detector" on createai.microbit.org
- Recorded 2 actions: Smash and Still
- Started with 3 samples each, then increased to 7 samples each
- Trained the model
- Tested immediately

**Test results:**  
Still = 99% accuracy ✅  
Smash = 100% accuracy ✅  

**Notes:**  
Same technology used in professional sports sensors costing
thousands of rupees — built by Lishan at age 9.

**Next ideas from this session:**  
- Add more shot types: clear shot, drop shot, defence
- Fix Bluetooth dropping — implement No Pairing Required mode

**Photos:**  
- ML training screenshots → Drive: v1-photos/ml-training-*-6may2026.png

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

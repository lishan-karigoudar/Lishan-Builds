// Smash Counter v0.21 — ML Smash + Still + Data Logger
// Lishan, May 2026
//
// What changed from v0.20:
// Added ml.onStart for Still event.
// When still detected — shows GO and current count.
// Smash event — increments, shows number, logs to datalogger.
//
// ML model improved:
// Went from 7 samples to 20 samples per action.
// Much stronger model — more data = better recognition.
// Smash: 20 samples. Still: 20 samples.
//
// First real data session with this version:
// 68 smashes logged with timestamps.
// Data retrieved via MY_DATA.HTM after session.
//
// Player feedback collected this session:
// Spoke to 15-18 kids and 6-7 adults.
// Badminton and non-badminton players both tested.
//
// Feedback summary:
// - Display not readable on court (LED matrix too small)
// - Bluetooth connection dropping during play
// - Not durable enough for court use
// - Wearable mount uncomfortable
// - One kid: "it looks huge"
// - Adults: "work on the wearable form factor"
// - One adult: won't buy, already spent money on son's LEGO
// - One adult laughed and walked away
//
// Key insight from feedback:
// The TECHNOLOGY works. The PRODUCT needs work.
// micro:bit is a prototype tool, not a final product.
// Next version must solve: display, durability, size, wearable.
// ESP32 with onboard LCD display addresses all of these.
//
// Special note:
// Alwin (restaurant owner) gave his number.
// Will provide kiosk demo space in his restaurant
// when ESP32 version is ready.
// First external demo venue confirmed.

let smashes = 0

datalogger.setColumnTitles("smashes")

input.onButtonPressed(Button.A, function () {
    smashes = 0
    basic.showString("GO")
})

ml.onStart(ml.event.Smash, function () {
    smashes += 1
    basic.showNumber(smashes)
    datalogger.log(
        datalogger.createCV("smashes", smashes)
    )
})

input.onButtonPressed(Button.B, function () {
    basic.showNumber(smashes)
})

ml.onStart(ml.event.Still, function () {
    basic.showString("GO")
    basic.showNumber(smashes)
})

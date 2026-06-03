// Smash Counter v0.17 — Smart Button Final
// Lishan, 27 April 2026
//
// What changed from v0.16:
// Button A now resets highest before each rally.
// Player presses A before each shot to clear the peak.
// Then smashes. Then presses B to log and classify.
// This isolates each shot properly.
//
// Button B logic:
// Reads peak, classifies PWR/MED/LOW, increments smashes,
// shows power label, shows smash count, then resets highest.
// All in one button press.
//
// Test results — 27 April 2026:
// 3 gentle shots → 1 PWR. Still showing PWR for gentle shots.
// 3 medium shots → 1 PWR. Same problem.
// Hardest smash → 1 PWR. Correct.
//
// Conclusion after full day of testing 27 April:
// The threshold calibration from 26 April (gentle=1555,
// medium=2896, hardest=3533) is correct.
// The problem is the forever loop accumulates movement
// from before the shot — walking, arm swing, setup.
// micro:bit cannot isolate just the smash impact cleanly.
// This is a hardware limitation.
// Next step: move to ESP32 with better IMU for v2.
//
// Last version on micro:bit before Kerala trip Apr 29 - May 3.
// Resumed after trip.

let peak = 0
let highest = 0
let smashes = 0

basic.forever(function () {
    peak = input.acceleration(Dimension.Strength)
    if (peak > highest) {
        highest = peak
    }
})

input.onButtonPressed(Button.A, function () {
    highest = 0
    basic.showString("GO")
})

input.onButtonPressed(Button.B, function () {
    if (highest > 3000) {
        smashes += 1
        basic.showString("PWR")
    } else if (highest > 2000) {
        smashes += 1
        basic.showString("MED")
    } else {
        basic.showString("LOW")
    }
    basic.showNumber(smashes)
    highest = 0
})

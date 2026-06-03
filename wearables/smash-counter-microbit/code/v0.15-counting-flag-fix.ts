// Smash Counter v0.15 — Counting Flag Fix
// Lishan, 27 April 2026
//
// What changed from v0.14:
// Added counting flag to stop double counting.
// Threshold set to 2000mg minimum to trigger a count.
// Forever loop tracks highest peak continuously.
// Button B shows count then power label.
//
// Test results — 27 April 2026:
// 3 hardest smashes → 1 counted, showed MED. PROBLEM: missed 2.
// 3 medium shots → 4 counted, showed MED. PROBLEM: overcounting.
// 3 gentle shots → 4 counted, showed MED. PROBLEM: overcounting.
//
// Problems found:
// 1500ms pause still not preventing all double counts.
// Power classification showing MED for everything.
// The highest variable only stores last reading, not true peak.
// Need to rethink the architecture completely.

let highest = 0
let smashes = 0
let counting = false

basic.forever(function () {
    let current = input.acceleration(Dimension.Strength)
    if (current > 2000 && !counting) {
        counting = true
        smashes += 1
        highest = current
        basic.pause(1500)
        counting = false
    }
})

input.onButtonPressed(Button.B, function () {
    basic.showNumber(smashes)
    basic.pause(1000)
    if (highest > 3000) {
        basic.showString("PWR")
    } else if (highest > 2000) {
        basic.showString("MED")
    } else {
        basic.showString("LOW")
    }
})

input.onButtonPressed(Button.A, function () {
    smashes = 0
    highest = 0
    basic.showString("GO")
})

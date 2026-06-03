// Smash Counter v0.16 — Button Based Counting
// Lishan, 27 April 2026
//
// New architecture — completely different approach.
// Instead of automatic detection, let the player press
// Button B after each shot to log it.
// Forever loop tracks peak continuously.
// Button B reads the peak, classifies it, counts it, resets.
// Button A shows total then resets everything.
//
// Why this approach:
// Automatic detection kept overcounting or undercounting.
// Button press gives the player control over what gets logged.
// More reliable than trying to detect the exact smash moment.
//
// Test results — 27 April 2026:
// 3 gentle shots → 1 PWR. PROBLEM: all showing PWR.
// 3 medium shots → 1 PWR. PROBLEM: all showing PWR.
// 3 hardest smashes → 1 PWR. OK but only 1 counted.
//
// Problem found:
// Forever loop accumulates peak across ALL movements.
// Even picking up the micro:bit registers as a high peak.
// Need to reset highest before each shot, not after.
// Fixed in v0.17.

let peak = 0
let highest = 0
let smashes = 0

basic.forever(function () {
    peak = input.acceleration(Dimension.Strength)
    if (peak > highest) {
        highest = peak
    }
})

input.onButtonPressed(Button.B, function () {
    if (highest > 2000) {
        smashes += 1
        if (highest > 3000) {
            basic.showString("PWR")
        } else {
            basic.showString("MED")
        }
    } else {
        basic.showString("LOW")
    }
    highest = 0
})

input.onButtonPressed(Button.A, function () {
    basic.showNumber(smashes)
    basic.pause(2000)
    smashes = 0
    highest = 0
    basic.showString("GO")
})

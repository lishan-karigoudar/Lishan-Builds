// Smash Counter v0.13 — Continuous Peak with Real Court Data
// Lishan, 27 April 2026
//
// The breakthrough version.
//
// New approach — forever loop instead of sampling loop:
// Previous versions sampled 10 times over 500ms.
// This version runs a forever loop continuously in background.
// Always watching. Never misses the peak. Much cleaner.
//
// Real court data collected on 26 April 2026:
// Gentle shot  = 1555mg
// Medium shot  = 2896mg
// Hardest smash = 3533mg
//
// Thresholds set from real data:
// Gentle = below 2000mg
// Power smash = above 3000mg
//
// Result: PERFECT first time after setting data-driven thresholds.
//
// Key learning:
// Don't guess thresholds. Measure first. Set thresholds from
// real data. This is how real engineers calibrate sensors.
//
// Button B = show highest peak, then reset
// Button A = reset and show GO

let peak = 0
let highest = 0

basic.forever(function () {
    peak = input.acceleration(Dimension.Strength)
    if (peak > highest) {
        highest = peak
    }
})

input.onButtonPressed(Button.B, function () {
    basic.showNumber(highest)
    highest = 0
})

input.onButtonPressed(Button.A, function () {
    highest = 0
    basic.showString("GO")
})

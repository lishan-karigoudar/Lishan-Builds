// Smash Counter v0.9 — Tuned Thresholds 4000/2000
// Lishan, 26 April 2026
//
// What changed from v0.8:
// Used real data from v0.8 to set better thresholds.
// Hard smash = around 4000mg+
// Medium smash = around 2000mg+
// Soft smash = below 2000mg
//
// Still using ThreeG gesture trigger.
// Problem found:
// ThreeG gesture sometimes missed fast smashes.
// Switched to Shake gesture in v0.10.

let power = 0

input.onGesture(Gesture.ThreeG, function () {
    power = input.acceleration(Dimension.Strength)
    if (power > 4000) {
        basic.showString("PWR")
    } else if (power > 2000) {
        basic.showString("MED")
    } else {
        basic.showString("LOW")
    }
    basic.pause(1500)
})

input.onButtonPressed(Button.A, function () {
    basic.showString("GO")
})

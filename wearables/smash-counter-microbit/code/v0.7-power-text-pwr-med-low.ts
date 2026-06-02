// Smash Counter v0.7 — Power Text PWR/MED/LOW
// Lishan, 23 April 2026
//
// What changed from v0.6:
// Replaced LED bars with text labels — easier to read quickly.
// PWR / MED / LOW shows on display after each smash.
// Thresholds adjusted: 3000mg = PWR, 1000mg = MED.
//
// Problem found:
// 1000mg threshold for MED was too low — almost everything
// showed as PWR or MED. Raised thresholds in v0.9.

let power = 0

input.onGesture(Gesture.ThreeG, function () {
    power = input.acceleration(Dimension.Strength)
    if (power > 3000) {
        basic.showString("PWR")
    } else if (power > 1000) {
        basic.showString("MED")
    } else {
        basic.showString("LOW")
    }
    basic.pause(1500)
})

input.onButtonPressed(Button.A, function () {
    basic.showString("GO")
})

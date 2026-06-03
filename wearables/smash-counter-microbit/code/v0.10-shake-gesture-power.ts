// Smash Counter v0.10 — Shake Gesture with Power Levels
// Lishan, 26 April 2026
//
// What changed from v0.9:
// Switched from ThreeG gesture to Shake gesture.
// ThreeG was missing some fast smashes.
// Shake gesture is more responsive to wrist motion.
//
// Thresholds kept same: 4000mg = PWR, 2000mg = MED.
//
// Problem found:
// Single instant reading still unreliable.
// Fast smashes peak and drop in milliseconds.
// One reading catches it sometimes, misses it other times.
// Solution: sample multiple readings and take the peak.
// Fixed properly in v0.11.

let power = 0

input.onGesture(Gesture.Shake, function () {
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

// Smash Counter v0.11 — Peak Detection Loop
// Lishan, 26 April 2026
//
// The big insight:
// A smash happens in milliseconds. Reading acceleration at one
// instant is like taking one photo of a moving ball — you might
// catch it or you might miss it completely.
//
// Solution: sample 10 readings over 500ms, keep the highest.
// This guarantees catching the peak of the smash motion.
//
// How the loop works:
// peak = 0 (reset before each smash)
// Loop 10 times, 50ms apart = 500ms total sampling window
// Each loop: read current acceleration
// If current > peak, update peak
// After loop: show PWR/MED/LOW based on peak value
//
// This was a major improvement over single instant reading.
// Much more consistent results across different smash styles.
//
// Problem found:
// Showing PWR/MED/LOW is useful but losing the actual number.
// v0.12 shows the raw peak number for better data.

let peak = 0
let current = 0

input.onGesture(Gesture.Shake, function () {
    peak = 0
    for (let i = 0; i < 10; i++) {
        current = input.acceleration(Dimension.Strength)
        if (current > peak) {
            peak = current
        }
        basic.pause(50)
    }
    if (peak > 4000) {
        basic.showString("PWR")
    } else if (peak > 2000) {
        basic.showString("MED")
    } else {
        basic.showString("LOW")
    }
    basic.pause(1500)
})

input.onButtonPressed(Button.A, function () {
    basic.showString("GO")
})

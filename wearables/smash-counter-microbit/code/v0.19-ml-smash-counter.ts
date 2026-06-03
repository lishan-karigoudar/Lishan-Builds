// Smash Counter v0.19 — First ML Version
// Lishan, 6 May 2026
//
// The big shift — from threshold detection to ML detection.
//
// Previous approach (v0.1 to v0.17):
// Check if acceleration number > threshold.
// Problem: all shots exceeded threshold at court level.
// Could not distinguish gentle from hard.
//
// New approach — ML gesture recognition:
// AI trained on createai.microbit.org
// Project: "badminton shot detector"
// Actions: Smash (7 samples), Still (7 samples)
// AI learns the SHAPE of the motion, not just the number.
// Smash accuracy: 100%. Still accuracy: 99%.
//
// How ml.onStart works:
// The ML model runs continuously in background.
// When it recognises the Smash pattern — fires the event.
// Much more reliable than threshold checking.
//
// This is the same technology used in professional
// sports wearables costing thousands of rupees.

let smashes = 0

ml.onStart(ml.event('"Smash"'), function () {
    smashes += 1
    basic.showNumber(smashes)
})

input.onButtonPressed(Button.A, function () {
    smashes = 0
    basic.showString("GO")
})

input.onButtonPressed(Button.B, function () {
    basic.showNumber(smashes)
})

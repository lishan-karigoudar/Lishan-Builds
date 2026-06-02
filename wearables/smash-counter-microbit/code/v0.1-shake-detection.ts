// Smash Counter v0.1 — Shake Detection
// Lishan Karigoudar, June 2025
//
// How it works:
// - Shake gesture = one smash detected
// - Button A = reset counter, show GO
// - Button B = show current smash count
//
// Notes:
// This was the first version. Used shake gesture as a simple
// proxy for a smash. Problem: any movement triggered it.
// Replaced in v0.2 with ML gesture model.

let smashes = 0

input.onButtonPressed(Button.A, function () {
    smashes = 0
    basic.showString("GO")
})

input.onButtonPressed(Button.B, function () {
    basic.showNumber(smashes)
})

input.onGesture(Gesture.Shake, function () {
    smashes += 1
    basic.showNumber(smashes)
})

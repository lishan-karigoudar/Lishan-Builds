// Smash Counter v0.8 — Raw Number Display
// Lishan, 25 April 2026
//
// What changed from v0.7:
// Stopped showing PWR/MED/LOW labels.
// Now shows the actual acceleration number on screen.
//
// Why:
// Needed to understand the real data before setting thresholds.
// What number does a hard smash actually produce?
// What number does a soft smash produce?
// Can't set good thresholds without knowing the real values.
//
// This version was used purely for data collection.
// Smashed many times, noted the numbers shown.
// This data was used to set better thresholds in v0.9 and v0.11.

let power = 0

input.onGesture(Gesture.ThreeG, function () {
    power = input.acceleration(Dimension.Strength)
    basic.showNumber(power)
    basic.pause(1000)
})

input.onButtonPressed(Button.A, function () {
    basic.showString("GO")
})

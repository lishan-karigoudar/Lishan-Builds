// Smash Counter v0.6 — Power Meter with LED Bars
// Lishan, 23 April 2026
//
// New idea: don't just COUNT smashes — MEASURE their power.
// Two days after first perfect court test, started experimenting
// with showing smash strength on the LED display.
//
// How it works:
// ThreeG gesture triggers measurement
// Shows LED bars based on power level:
// - Full bars (PWR) = above 2500mg
// - Mid bars (MED) = above 1800mg  
// - Single dot (LOW) = below 1800mg
//
// Problem found:
// Reading acceleration at one instant misses the peak.
// Need to sample multiple times to catch the highest point.
// Fixed in v0.11 with peak detection loop.

let power = 0

input.onGesture(Gesture.ThreeG, function () {
    power = input.acceleration(Dimension.Strength)
    if (power > 2500) {
        basic.showString("PWR")
        basic.showLeds(`
            . # . # .
            . # . # .
            . # . # .
            . # . # .
            . # . # .
            `)
    } else if (power > 1800) {
        basic.showLeds(`
            . . . . .
            . # . # .
            . # . # .
            . # . # .
            . . . . .
            `)
    } else {
        basic.showLeds(`
            . . . . .
            . . . . .
            . # . # .
            . . . . .
            . . . . .
            `)
    }
    basic.pause(1500)
})

input.onButtonPressed(Button.A, function () {
    basic.showString("GO")
})

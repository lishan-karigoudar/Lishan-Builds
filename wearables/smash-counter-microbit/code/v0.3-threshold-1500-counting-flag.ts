// Smash Counter v0.3 — Threshold 1500mg + Counting Flag
// Lishan Karigoudar, 2025
//
// What changed from v0.2:
// Raised threshold from 1200mg to 1500mg after court testing.
// 1200mg was triggering on normal arm movements.
// 1500mg felt more accurate for real smash force.
//
// The counting flag now works properly:
// Once a smash is detected, counting = true
// This blocks any new detection for 2000ms (2 seconds)
// Then counting = false — ready for next smash
//
// This solved the double-counting problem completely.
//
// Problem found:
// 1500mg still triggered occasionally on jump smash approach.
// Tested 2500mg in v0.4 to see if higher threshold was better.

let counting = false
let smashes = 0

basic.forever(function () {
    if (input.acceleration(Dimension.Strength) > 1500 && !counting) {
        counting = true
        smashes += 1
        basic.showNumber(smashes)
        basic.pause(2000)
        counting = false
    }
})

input.onButtonPressed(Button.A, function () {
    smashes = 0
    basic.showNumber(smashes)
})

input.onButtonPressed(Button.B, function () {
    basic.showNumber(smashes)
})

// Smash Counter v0.4 — Threshold 2500mg Court Testing
// Lishan, 2025
//
// What changed from v0.3:
// Raised threshold from 1500mg to 2500mg.
// Wanted to test if higher threshold = fewer false triggers.
//
// Court test result:
// 2500mg was too high — missed some real smashes.
// Lighter smashes from the back court were not detected.
// Decided 1500mg was the sweet spot.
// Went back to 1500mg in v0.5.
//
// Key learning:
// There is no perfect threshold — it depends on the player.
// A hard hitter needs higher threshold.
// A junior player needs lower threshold.
// Future versions should let the player set their own threshold.

let counting = false
let smashes = 0

basic.forever(function () {
    if (input.acceleration(Dimension.Strength) > 2500 && !counting) {
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

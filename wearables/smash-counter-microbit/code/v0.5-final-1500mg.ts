// Smash Counter v0.5 — Final Version, 1500mg Threshold
// Lishan, 2025
//
// What changed from v0.4:
// Came back to 1500mg after testing proved 2500mg missed smashes.
// This is the version that went on the wrist for real court testing.
// Connected to CR2025 battery for wireless use.
// Bluetooth connection tested with phone.
//
// How the counting flag works:
// counting = false means ready to detect
// When smash detected → counting = true immediately
// This locks out any new detection for 2000ms
// After 2000ms → counting = false → ready again
// This prevents one smash registering as 2 or 3 counts.
//
// Button A = reset to zero, show 0
// Button B = show current count anytime
//
// Status: Tested on court with real players.
// Next version will move to ESP32 with onboard display.

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

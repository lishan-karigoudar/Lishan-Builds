// Smash Counter v0.12 — Peak Detection Final
// Lishan, 26 April 2026
//
// What changed from v0.11:
// Shows the actual peak number instead of PWR/MED/LOW.
// Button B recalls the last peak reading anytime.
//
// Why raw numbers are better here:
// Every player smashes differently.
// Showing the number lets the player understand their own power.
// A junior player might peak at 2000mg.
// A senior player might peak at 5000mg+.
// Raw data is more honest than fixed labels.
//
// This is the most advanced version on micro:bit.
// Next step: move to ESP32 with onboard display for
// real-time power graphs and session history storage.

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
    basic.showNumber(peak)
    basic.pause(2000)
})

input.onButtonPressed(Button.A, function () {
    basic.showString("GO")
})

input.onButtonPressed(Button.B, function () {
    basic.showNumber(peak)
})

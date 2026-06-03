// Smash Counter v0.14 — Combined Counter and Power Meter
// Lishan, 27 April 2026
//
// Biggest version yet — combines smash counting AND power measurement.
// Button B toggles between showing count and showing power level.
// showMode variable tracks which display mode we are in.
//
// Test results — 27 April 2026:
// Test 1: 5 smashes → counted 8. PROBLEM: overcounting.
//         5 medium shots → counted 5. OK.
//         5 gentle shots → counted 14. PROBLEM: way overcounting.
// Test 2: All shots showed PWR. PROBLEM: threshold too low.
// Test 3: Button A showed GO correctly. OK.
//
// Problems found:
// Shake gesture triggering multiple times per smash.
// Power threshold too low — everything reading as PWR.
// Need counting flag to prevent double counting.
// Fixed in v0.15.

let peak = 0
let highest = 0
let smashes = 0
let showMode = 0

basic.forever(function () {
    peak = input.acceleration(Dimension.Strength)
    if (peak > highest) {
        highest = peak
    }
})

input.onGesture(Gesture.Shake, function () {
    smashes += 1
})

input.onButtonPressed(Button.B, function () {
    if (showMode == 0) {
        basic.showNumber(smashes)
        showMode = 1
    } else {
        if (highest > 3000) {
            basic.showString("PWR")
        } else if (highest > 2000) {
            basic.showString("MED")
        } else {
            basic.showString("LOW")
        }
        highest = 0
        showMode = 0
    }
})

input.onButtonPressed(Button.A, function () {
    smashes = 0
    highest = 0
    showMode = 0
    basic.showString("GO")
})

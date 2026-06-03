// Smash Counter v0.22 — ML + Session Timer
// Lishan, 10 May 2026
//
// Lishan's own idea again — no prompting.
//
// New feature: Session Timer
// Button B now shows TWO things:
// 1. Current smash count
// 2. How many minutes since session started
//
// Why this matters:
// Knowing smashes per session is useful.
// Knowing smashes per MINUTE is more useful.
// A player can now track: did I smash more in 10 minutes
// this week than last week?
//
// How it works:
// startTime recorded when Button A pressed (session start)
// Button B: show count, pause 2 seconds, show "T" then minutes
// minutes = (current time - start time) / 60000ms
//
// Quote from this session — 10 May 2026:
// "I feel like I got a job" — Lishan
//
// This is the last version on micro:bit.
// Moving to ESP32 next.
// Reason: player feedback pointed to hardware limitations.
// micro:bit LED display not readable on court.
// Size too big. Bluetooth unreliable.
// ESP32-S3-Touch-LCD-1.28 solves all of these.
//
// Next hardware: XIAO ESP32C3 + MPU-6050
// Total cost: approximately 900 rupees
// Plan: Stage 1 — sensor data on serial monitor
//        Stage 2 — BLE to Android via nRF Connect

let smashes = 0
let startTime = input.runningTime()

ml.onStart(ml.event.Smash, function () {
    smashes += 1
    basic.showNumber(smashes)
    datalogger.log(
        datalogger.createCV("smashes", smashes)
    )
})

ml.onStart(ml.event.Still, function () {
    basic.showString("GO")
})

input.onButtonPressed(Button.A, function () {
    smashes = 0
    startTime = input.runningTime()
    basic.showString("GO")
})

input.onButtonPressed(Button.B, function () {
    basic.showNumber(smashes)
    basic.pause(2000)
    let minutes = Math.floor((input.runningTime() - startTime) / 60000)
    basic.showString("T")
    basic.showNumber(minutes)
})

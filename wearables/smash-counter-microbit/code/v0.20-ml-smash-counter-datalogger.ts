// Smash Counter v0.20 — ML + Data Logger
// Lishan, 6 May 2026
//
// Lishan's own idea — no prompting needed.
//
// Problem identified:
// Every time we want to see the data, we need to connect
// micro:bit to a computer via Bluetooth or USB.
// Bluetooth keeps dropping during smash impact.
// This is frustrating during court testing.
//
// Lishan's question: "Why not add memory to the micro:bit
// so it stores data by itself? Then connect later to get it."
//
// Solution found: micro:bit V2 has built-in Data Logger.
// Stores data directly to internal flash memory.
// No Bluetooth needed during court session.
// After session: plug USB into MacBook, open MY_DATA.HTM.
// See every smash with exact timestamp. Download as CSV.
//
// This is exactly how professional sports data loggers work.
// Record during session. Analyse after.
//
// What gets logged per smash:
// - time: milliseconds since micro:bit switched on
// - smashes: running total at that moment
//
// To retrieve data:
// 1. Plug micro:bit into computer via USB
// 2. Open MY_DATA.HTM file on the MICROBIT drive
// 3. All smashes with timestamps visible immediately

let smashes = 0

datalogger.setColumnTitles("time", "smashes")

ml.onStart(ml.event('"Smash"'), function () {
    smashes += 1
    basic.showNumber(smashes)
    datalogger.log(
        datalogger.createCV("time", input.runningTime()),
        datalogger.createCV("smashes", smashes)
    )
})

input.onButtonPressed(Button.A, function () {
    smashes = 0
    basic.showString("GO")
})

input.onButtonPressed(Button.B, function () {
    basic.showNumber(smashes)
})

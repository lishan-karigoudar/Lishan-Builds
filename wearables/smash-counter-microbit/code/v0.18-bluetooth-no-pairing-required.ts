// Smash Counter v0.18 — Bluetooth No Pairing Required Fix
// Lishan, 6 May 2026
//
// Problem from all previous versions:
// Bluetooth connection kept dropping during smash motion.
// The impact of the smash was physically disconnecting Bluetooth.
// Every time connection dropped, had to re-pair the device.
// This made court testing frustrating and unreliable.
//
// Root cause:
// Default micro:bit Bluetooth requires pairing handshake.
// During a smash, the sudden movement interrupts the handshake.
// Connection drops and micro:bit waits for re-pairing.
//
// Fix: No Pairing Required mode
// In MakeCode — go to Project Settings
// Turn ON "No Pairing Required" under Bluetooth settings
// This means any device can connect instantly without handshake.
// If connection drops during smash, it reconnects automatically.
// No manual re-pairing needed.
//
// Result:
// Bluetooth stays connected through smash impacts.
// Court testing now uninterrupted.
//
// Note: This is a PROJECT SETTING in MakeCode, not a code change.
// The setting is saved in the .hex file when downloaded.
// Screenshot of the setting saved in Drive: v1-hardware/

// This file documents the fix.
// The actual Bluetooth code runs automatically once setting is on.

bluetooth.startUartService()

bluetooth.onBluetoothConnected(function () {
    basic.showIcon(IconNames.Heart)
})

bluetooth.onBluetoothDisconnected(function () {
    basic.showIcon(IconNames.Sad)
    // With No Pairing Required — reconnects automatically
    // No need to manually re-pair
})

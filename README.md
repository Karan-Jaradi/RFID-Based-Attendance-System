# RFID-Based Attendance System (8051 + Embedded C)

## Introduction

This project implements an **RFID-based attendance system** using the 8051 microcontroller, interfacing an EM-18 RFID reader (via UART) and a 16x2 LCD to automate attendance marking and eliminate proxy attendance.

## Features

- UART-based interfacing with the `EM-18` RFID reader module at 9600 baud (Timer 1, Mode 2)
- 16x2 LCD display (4-control-line mode: `rs`, `rw`, `en` on `P3.7`, `P3.6`, `P3.5`) for real-time status messages
- Stores and compares scanned 12-character card IDs against two valid, pre-registered RFID tags
- Duplicate-scan detection — displays `Already Present` if an already-marked card is scanned again in the same session
- Unauthorized card handling — displays `Access Denied` for any unrecognized card
- Optional buzzer feedback on valid/invalid access (per circuit design)
- Simple, modular embedded C structure — separate functions for LCD command/data writes, UART receive, delay, and ID comparison

## Hardware Components

| Component               | Purpose                                          |
|--------------------------|---------------------------------------------------|
| 8051 Microcontroller (AT89C51) | Core controller — processes RFID data and drives the LCD |
| EM-18 RFID Reader        | Reads the 12-character unique ID from scanned RFID tags |
| RFID Tags/Cards          | Carried by each user; contain a unique ID           |
| 16x2 LCD Display         | Displays scan prompts, welcome messages, and access status |
| Buzzer                   | Audio feedback on valid/invalid access             |
| MAX232 (optional)        | RS-232 level shifting for serial communication, if needed |
| 5V Regulated Power Supply | Powers the microcontroller and peripherals        |

## Module / Signal Reference

### LCD Control Pins (`P3`)

| Signal | Pin    | Description                          |
|--------|--------|----------------------------------------|
| `rs`   | `P3.7` | Register select (command vs. data)     |
| `rw`   | `P3.6` | Read/write select (held low — write only) |
| `en`   | `P3.5` | Enable strobe (latches command/data)   |

### Data Bus

| Signal | Port | Description                                  |
|--------|------|-------------------------------------------------|
| `P1`   | `P1` | 8-bit data bus to the LCD (commands and characters) |

### UART (RFID Reader Interface)

| Register | Value  | Description                     |
|----------|--------|------------------------------------|
| `TMOD`   | `0x20` | Timer 1, Mode 2 (auto-reload)       |
| `SCON`   | `0x50` | 8-bit UART, REN enabled             |
| `TH1`    | `0xFD` | Baud rate = 9600 (at 11.0592 MHz)   |

## Algorithm

1. Initialize the LCD and UART; display `Scan your Card`.
2. Wait for a 12-character RFID tag ID over UART.
3. Compare the received ID against the two stored valid IDs (`valid1`, `valid2`).
4. If the ID matches a valid card:
   - First scan → display `Welcome User 1` / `Welcome User 2`, mark attendance
   - Repeat scan → display `Already Present`
5. If the ID doesn't match any stored card → display `Access Denied`.
6. Clear the LCD, redisplay the scan prompt, and repeat indefinitely.

## Verification

This project was verified through **physical hardware implementation** on a breadboard (8051, EM-18 RFID reader, and 16x2 LCD wired together per the circuit diagram), rather than software simulation:

- The breadboard build confirmed correct UART reception of the 12-character card ID from the EM-18 reader, displayed live on the LCD (e.g., `RFID card Number 02001070D062`).
- Functional testing confirmed:
  - A registered card correctly triggers a welcome message on first scan.
  - The same card scanned again correctly triggers `Already Present` instead of a duplicate welcome message.
  - An unregistered card correctly triggers `Access Denied`.
- The `compare()` function was verified to correctly reject any ID differing in even a single character across its 12-character length.
## Files

- `rfid_attendance.c` — Embedded C source for the 8051 (Keil C51 syntax), interfacing the RFID reader and LCD

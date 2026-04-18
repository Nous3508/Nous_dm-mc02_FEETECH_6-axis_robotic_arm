# Nous_dm-mc02_FEETECH_6-axis_robotic_arm

> A playful project to control a 6-axis robotic arm using FEETECH STS3215 serial servos and an STM32H723VGT6. Think of this repo as the brain and heartbeat for the little robot — with firmware, drivers, and test tools.

## Quick overview

- Platform: STM32H723VGT6
- Primary toolchains: Keil MDK and VS Code
- Programmer / Debugger: ST-Link
- Servo bus: USART10 (PE3 = TX, PE2 = RX)
- Debug/console: UART7 (PE8 = TX, PE7 = RX)
- Servo communication: 1,000,000 bps, 8N1
- License: All rights reserved (no open license)
- Author: Nous3508

## What's inside

- `Core/Src/` – main firmware and peripheral initialisation
- `Device/STS3215/` – FEETECH STS3215 protocol implementation and commios (writeSCS/readSCS and helpers)
- `Device/` – drivers and hardware helpers (e.g., WS2812 support)
- `Core/Inc/` – project headers

## Wiring (important!)

Short summary of serial wiring used by the firmware (double-check on your board before powering up):

- FEETECH servo bus <-> MCU USART10
  - MCU PE3 (USART10_TX) -> servo RX
  - MCU PE2 (USART10_RX) <- servo TX
- Debug / PC console <-> MCU UART7 (USB-Serial)
  - MCU PE8 (UART7_TX) -> USB-to-PC TX
  - MCU PE7 (UART7_RX) <- USB-to-PC RX

Note: if you do a TX↔RX loopback for testing, short PE3 <-> PE2 on the board (only for local tests; disconnect servos first!).

## Build & flash

Keil MDK (recommended):
1. Open the Keil project in the `Project/` folder (or the `.uvprojx`/`.uvproj` file at the repo root). 
2. Build using the ARMCC toolchain (MDK v5 recommended). 
3. Flash with ST-Link (in Keil, Connect -> Download).

VS Code (alternate):
- Use Cortex-Debug + arm-none-eabi toolchain. Configure `launch.json` for ST-Link and point tasks to the build output. This repo is compatible with VS Code-based workflows but Keil is the primary tested environment.

## Quick run & tests

1. Power the board and servos. The firmware waits ~1s on startup to let servos boot.
2. The firmware prints a greeting on the debug console (UART7). Open the serial monitor (115200) to watch logs.
3. To search for servos, call the `Find_STS3215()` functionality (it runs at startup in current build). It prints discovered IDs.

Manual loopback test (useful when debugging USART10):
- Short PE3 <-> PE2, run the built firmware and check the console for loopback debug messages.

## Tips & gotchas

- If you see garbled bytes or framing errors on USART10 at 1 Mbps, try:
  - Ensuring PE2/PE3 alternate function is correct and both pins use the same AF.
  - Setting GPIO speed to VERY_HIGH for the USART pins.
  - Using Oversampling = 8 and, if necessary, OneBitSampling enable on USART for high baud.
  - If repeated issues occur, try a lower baud (115200) to verify wiring and then move back to 1 Mbps.

- The repo contains temporary debug helpers (e.g., `debug_regs_after_tx`) — feel free to remove or clean them once you confirm your hardware is stable.

## Contributing

PRs and issues are welcome — though this project currently retains All Rights Reserved. Please open issues for bugs or feature requests and include logs / serial dumps where possible.

## Contact

Author: Nous3508

---

Enjoy tinkering! If you want, I can also add a one-click build/flash script for VS Code or a small wiring diagram image to the repo.

/**
 * @file board.h
 * @brief Hardware abstraction layer for Ignis V2.2 PCB
 *
 * Project: Ignis - Autonomous flame-shaped lamp with wireless charging
 * Hardware: IgnisV2 PCB with ATtiny212, BQ24073, BQ51013B, WS2812B-2020
 * Version: V2.2
 *
 * This file contains board-specific definitions including pin assignments,
 * hardware configuration constants, and peripheral mappings for the
 * IgnisV2.2 PCB design.
 *
 * @author TheoToys
 * @date September 2025
 * @license MIT License - See LICENSE file for details
 */

#ifndef BOARD_H
#define BOARD_H

#include <Arduino.h>

// Board identification
#define BOARD_VERSION "IgnisV2.2"
#define BOARD_MCU "ATtiny212"

#define PIN_UPDI PIN_PA0    // UPDI programming pin
#define PIN_IND_B PIN_PA1   // Low when inductor receives power
#define PIN_CHRG_B PIN_PA2  // Low when battery is charging
#define PIN_PGOOD_B PIN_PA3 // Low when charging is complete
#define PIN_LED PIN_PA6     // WS2812B data pin
#define PIN_SSW PIN_PA7     // Shock detection switch pin

#define NUM_LEDS 6

#endif // BOARD_H

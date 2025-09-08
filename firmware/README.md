# Ignis Firmware - Ignis Lamp

## Current Project Status

### ✅ Implemented Features

- **WS2812B LED Control** : Implementation with tinyNeoPixel library
- **Light Patterns** : Cycling pattern with 7 colors (Red, Green, Blue, Purple,
  Yellow, Cyan, White)
- **ATtiny212 Configuration** : PlatformIO configured for ATtiny212 (IgnisV2.2)
- **Basic Structure** : Code organized with board.h for hardware abstraction

### 🚧 To Implement

- **Power Management** : Sleep mode management with interrupt wake-up
- **Shock Sensor** : SW18030 integration for shake detection
- **Charge Detection** : Using CHRG_B, PGOOD_B, IND_B pins
- **Advanced Light Patterns** : Heartbeat, fade, realistic flame effects
- **Power Optimizations** : Sleep modes for battery conservation
- **Battery Management** : Charge level monitoring

## Hardware Configuration

- **Microcontroller** : ATtiny212 @ 20MHz (IgnisV2.2 PCB)
- **WS2812B Pin** : PA6 (PIN_LED = PIN_PA6)
- **Shock Sensor Pin** : PA7 (PIN_SSW = PIN_PA7) with RC filter + Schmitt
  trigger
- **Inductor Detection Pin** : PA1 (PIN_IND_B = PIN_PA1) - Low when inductor
  active
- **Battery Charging Pin** : PA2 (PIN_CHRG_B = PIN_PA2) - Low during charging
- **Charge Complete Pin** : PA3 (PIN_PGOOD_B = PIN_PA3) - Low when charging
  complete
- **UPDI Pin** : PA0 (PIN_UPDI = PIN_PA0) for programming
- **LED Count** : 6 WS2812B-2020 LEDs

## Power Management

The firmware aims to implement an optimized power management system:

1. **Active Mode** : WS2812B LED control, sensor monitoring
2. **Inactivity Detection** : Based on SW18030 shock sensor
3. **Sleep Mode** : `SLEEP_MODE_PWR_DOWN` for maximum autonomy
4. **Wake-up** : Interrupt on motion detection or charging
5. **Charge Monitoring** : Using BQ24073 pins for battery status

Test software showed that all 6 LEDs at full brightness consume around

$$
I_{max} = \boxed{380\,\text{mA}} \implies P_{max} = 1.9\,\text{W}
$$

As this is above 1C for the 320mAh battery, power management and brightness
control will be needed to ensure good battery life.

### Power Saving Strategy

- Disable ADC, SPI, timers in sleep mode
- Intelligent PWM for LED brightness control
- Automatic charging station detection (IND_B)
- Adaptive pattern management based on battery level

## Compilation

```bash
# Via VS Code PlatformIO
Ctrl+Shift+P -> "PlatformIO: Build"

# Or via terminal (if pio installed)
cd firmware/
pio run
```

## Upload/Debug

Pymcuprog configuration in `platformio.ini`:

- Programmer : `pymcuprog` via UART
- Device : `attiny212`
- Port : `$UPLOAD_PORT` (defined by PlatformIO)
- Speed : 115200 baud
- UPDI : Programming via PA0

**⚠️ Important** : Generally, the power supplied by the USB-to-serial adapter is
not sufficient to power the board during programming. Attach an external power
supply on the board, if possible by injecting 3.7V (same as battery pack) on the
VSYS drilled pad.

### Pymcuprog Installation

The `scripts/requirements.py` script automatically installs pymcuprog if needed.

```bash
# Manual installation if required
pip install pymcuprog
```

## Next Steps

1. **Hardware Validation** : Test pins and basic functions
2. **Light Patterns** : Implement realistic flame effects
3. **Shock Sensor** : SW18030 integration with interrupts
4. **Power Management** : Sleep modes and intelligent wake-up
5. **Battery Monitoring** : Charge and autonomy management
6. **Autonomy Tests** : Validate 2-3 hour target

## Debug and Development

- Code compiled and tested on IgnisV2.2 PCB
- Using tinyNeoPixel for WS2812B control
- Incremental development for hardware validation
- Power consumption and light pattern testing

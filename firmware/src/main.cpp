#include <Arduino.h>
#include <avr/interrupt.h>

// Pin definitions
#define BUTTON_PIN 2 // PA2

// Timing constants (in ms)
#define DEBOUNCE_MIN_MS 30
#define DEBOUNCE_LONG_MS 2000

enum e_btnactionrequest_t { BTN_ACTION_NONE, BTN_ACTION_SHORT_PRESS, BTN_ACTION_LONG_PRESS };

typedef struct {
  uint32_t press_time;
  uint32_t release_time;
  e_btnactionrequest_t action_request;
} button_state_t;

volatile button_state_t button_state = {0, 0, BTN_ACTION_NONE};

// ISR pour interruption sur changement d'état du bouton
ISR(PORTA_PORT_vect) {
  bool pressed = digitalRead(BUTTON_PIN) == LOW; // Active low
  uint32_t now = millis();
  if (pressed) {
    button_state.press_time = now;
  } else {
    button_state.release_time = now;
    uint32_t duration = button_state.release_time - button_state.press_time;
    if (duration < DEBOUNCE_MIN_MS) {
      // Ignore
    } else if (duration < DEBOUNCE_LONG_MS) {
      // Short press
      button_state.action_request = BTN_ACTION_SHORT_PRESS;
    } else {
      // Long press
      button_state.action_request = BTN_ACTION_LONG_PRESS;
    }
  }

  // Clear interrupt flag
  PORTA.INTFLAGS = PIN2_bm;
}

void handle_button_event() {
  switch (button_state.action_request) {
  case BTN_ACTION_SHORT_PRESS:
    // TODO: Handle short press action
    break;
  case BTN_ACTION_LONG_PRESS:
    // TODO: Handle long press action
    break;
  default:
    break;
  }

  // Reset button state
  button_state.action_request = BTN_ACTION_NONE;
}

void setup() {
  pinMode(BUTTON_PIN, INPUT_PULLUP);

  // Configuration interruption pour ATtiny212
  // Activer interruption sur PORTA pin 2
  PORTA.PIN2CTRL = PORT_ISC_BOTHEDGES_gc; // Interruption sur les deux fronts

  sei(); // Activer interruptions globales

  // TODO: Disable unused peripherals for low power if needed
}

void loop() {
  if (button_state.action_request == BTN_ACTION_NONE) {
    handle_button_event();
  } else {
    // Put uC to sleep
  }
}

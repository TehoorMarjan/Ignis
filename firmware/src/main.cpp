#include "board.h"
#include <avr/interrupt.h>
#include <tinyNeoPixel_Static.h>

byte pixels[NUM_LEDS * 3];

tinyNeoPixel leds = tinyNeoPixel(NUM_LEDS, PIN_LED, NEO_GRB + NEO_KHZ800, pixels);

int current_led = 0;
enum State { RED = 0, GREEN = 1, BLUE = 2, PURPLE = 3, YELLOW = 4, CYAN = 5, WHITE = 6 };
State current_state = RED;

void setup() {
  // pinMode(PIN_SSW, INPUT);
  pinMode(PIN_LED, OUTPUT);

  // Configuration interruption pour ATtiny212
  // Activer interruption sur PORTA pin 2
  // PORTA.PIN2CTRL = PORT_ISC_BOTHEDGES_gc; // Interruption sur les deux fronts

  // sei(); // Activer interruptions globales

  leds.fill(); // Éteindre toutes les LEDs
  leds.show(); // Initialiser les LEDs
}

void loop() {
  current_led++;
  int red, green, blue;
  if (current_led >= NUM_LEDS) {
    current_led = 0;
    delay(500);  // Attendre 500 ms du plus pour laisser le temps de calculer la conso
    leds.fill(); // Éteindre toutes les LEDs
    leds.show(); // Mettre à jour les LEDs
    delay(500);  // Attendre 500 ms avant de rallumer la LED
    current_state = static_cast<State>((current_state + 1) % 7); // Passer à l'état suivant
  }
  switch (current_state) {
  case RED:
    red = 255;
    green = 0;
    blue = 0;
    break;
  case GREEN:
    red = 0;
    green = 255;
    blue = 0;
    break;
  case BLUE:
    red = 0;
    green = 0;
    blue = 255;
    break;
  case PURPLE:
    red = 128;
    green = 0;
    blue = 128;
    break;
  case YELLOW:
    red = 255;
    green = 255;
    blue = 0;
    break;
  case CYAN:
    red = 0;
    green = 255;
    blue = 255;
    break;
  case WHITE:
    red = 255;
    green = 255;
    blue = 255;
    break;
  default:
    red = 0;
    green = 0;
    blue = 0;
    break;
  }
  leds.setPixelColor(current_led, red, green, blue); // Allumer la LED actuelle en rouge
  leds.show();
  delay(500);
}

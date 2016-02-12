#include "FastLED.h"
#define NUM_LEDS 80 
#define DATA_PIN 11
#define CLOCK_PIN 13

CRGB leds[NUM_LEDS];


void setup() { FastLED.addLeds<WS2801, DATA_PIN, CLOCK_PIN>(leds, NUM_LEDS); }
void loop() {
    leds[0] = CRGB::White; FastLED.show(); delay(30);
    leds[0] = CRGB::Blue; FastLED.show(); delay(30);
}

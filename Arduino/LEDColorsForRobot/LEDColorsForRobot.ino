#include "FastLED.h"

#define NUM_LEDS 150
//#define DATA_PIN 11
#define POWER_UP (-1)

CRGB leds[NUM_LEDS];
int command;
int prevCommand;
int foreground = 0; 

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
  FastLED.addLeds<NEOPIXEL, 11>(leds, NUM_LEDS); 
  FastLED.clear();
  FastLED.setBrightness(80); 
  Serial.begin(9600);
  command = prevCommand = POWER_UP;
}

void loop() {
  //blink(2);
  if (Serial.available() > 0) {
    command = Serial.read();
    if (command == '\r' || command == '\n' || command == ' ')
      command = prevCommand; 
  }
  if (command != prevCommand) {
    prevCommand = command; 
  }
  dispatchCommand(command);
}

void dispatchCommand(int command){
  uint32_t color;
  switch(command){
    case POWER_UP:                   
    case 'a':
      solid_color(CRGB::Crimson);
      break; 
    case 'b':
      solid_color(CRGB::Blue);
      break;
    case 'g':
      solid_color(CRGB::Green);
      break;
    case 'p':
      missing_dot_chase(CRGB::Red, 25);   
      break;
    case 'r':
      solid_color(CRGB::Red);
      break;
    case 'w':
      solid_color(CRGB::White);
      break;
    default:
      solid_color(CRGB::Orange);
  }
  return;
}

void blink(int count)
{
  for (int i=0; i<count; i++) {
    digitalWrite(LED_BUILTIN, HIGH);   // turn the LED on (HIGH is the voltage level)
    delay(250);
    digitalWrite(LED_BUILTIN, LOW);    // turn the LED off by making the voltage LOW
    delay(250);
  }
  delay(750);
}

void solid_color(uint32_t color)
{
  for(int i = 0; i < NUM_LEDS; i++) {
    leds[i] = color;
  }
  // Show the leds (only one of which is set to white, from above)
  FastLED.show();
}

void color_chase(uint32_t color, uint8_t wait)
{  
  FastLED.clear();
  FastLED.setBrightness(100);
  // Move a single led
  for(int led_number = 0; led_number < NUM_LEDS; led_number++)
  {
    // Turn our current led ON, then show the leds
    leds[led_number] = color;

    // Show the leds (only one of which is set to white, from above)
    FastLED.show();

    // Wait a little bit
    delay(50);

    // Turn our current led back to black for the next loop around
    leds[led_number] = CRGB::Black;
  }
}

void althernate_colors(uint32_t fgcolor, uint32_t bgcolor) {
  for(int led_number = 0; led_number < NUM_LEDS; led_number++)
  {
    if (led_number % 3 == foreground)
      leds[led_number] = fgcolor;
    else
      leds[led_number] = bgcolor;
  }
  FastLED.show();
  foreground = (foreground+1) % 3;
}

void missing_dot_chase(uint32_t color, uint8_t wait){
  int led_number; 
  int led_brightness = 80; 
  static int white_led_number = 0; 
    FastLED.setBrightness(led_brightness);
    for(led_number = 0; led_number < NUM_LEDS; led_number++) 
      leds[led_number] = color;
    leds[white_led_number] = CRGB::Snow; 
    if(white_led_number > 0) {
      leds[white_led_number-1] = color;
    }
    if(white_led_number < (NUM_LEDS - 1)){
      white_led_number++;
    } 
    else {
      white_led_number = 0; 
    }
    FastLED.show();
    delay(20); 
}


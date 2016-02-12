#include "FastLED.h"

#define NUM_LEDS 80
#define DATA_PIN 11
#define CLOCK_PIN 13
#define POWER_UP (-1)

CRGB leds[NUM_LEDS];
int command;
int prevCommand;
int foreground = 0; 
uint32_t color; 

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);

  FastLED.addLeds<WS2801, RGB>(leds, NUM_LEDS); 
  FastLED.clear();

  FastLED.setBrightness(80); 

  Serial.begin(9600);

  command = prevCommand = POWER_UP;
  color = commandToColor(command);
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

void loop() {
  blink(2);
#if 0
  if (Serial.available() > 0) {
    command = Serial.read();

    if (command == '\r' || command == '\n' || command == ' ')
      command = prevCommand; 
  }
  if (command != prevCommand) {
    prevCommand = comand; 
    color = commandToColor(command); 
  }
#else
  color = CRGB::White; 
#endif
 // color = CRGB::Green; 
    color_chase(color, 100);
      delay(100);
for(int i = 0; i < NUM_LEDS; i++) {
  leds[i] = color;
}

}

uint32_t commandToColor(int command){
  uint32_t color;
  switch(command){
    case POWER_UP:
      color = CRGB::White; 
      break; 
    case 'a':
//      color = CRGB::Rainbow;
      break;
    case 'b':
      color = CRGB::Blue;
      break;
    case 'g':
      color = CRGB::Green;
      break;
    case 'p':
      missing_dot_chase(CRGB::Red, 25);   
      break;
    case 'r':
      color = CRGB::Red;
      break;
    case 'w':
      color = CRGB::White;
      break;
    default:
      color = CRGB::Orange;
  }
  return color; 
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
  int led_brightness = 100; 
    FastLED.setBrightness(led_brightness);
    for(led_number = 0; led_number < NUM_LEDS; led_number++) leds[led_number] = color;     
      
    for(led_number = 0; led_number < NUM_LEDS; led_number++) {
      leds[led_number] = CRGB::Gray; 
      if( led_number > 0 && led_number < NUM_LEDS) {
        leds[led_number-1] = color;   
      }
      FastLED.show();
      delay(wait); 
    }
}


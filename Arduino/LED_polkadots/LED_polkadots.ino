// LED_polkadots
//
// Joseph Jackson
// Sample code for FIRST FRC Team #3504, Girls of Steel
// Displays a light pattern, changing the color when a character is received over the serial port
//
// Works with the AndyMark AM-2640 LED Strip based on the WS2801 chipset
// This code requires that the fastspi library be put in your arduino\libraries folder

//https://code.google.com/p/fastspi/
//https://code.google.com/p/fastspi/wiki/CRGBreference
#include "FastSPI_LED2.h"

// How many leds are in the strip? AndyMark's 2.5 meter strip has 80 leds 
#define NUM_LEDS 80
//Remember on the AM-2640 LED strip's yellow wire is ground! (don't blame us they come that way)
// Data pin that led data will be written out over
#define DATA_PIN 11	//Green wire from AM-2640's power connector
// Clock pin SPI 
#define CLOCK_PIN 13    //Blue wire from AM-2640's power connector

// Before a command character is sent to the serial port, it will be set to this constant
// Check for this value and set whatever light display you want for the "just powered on" state
#define POWER_UP (-1)

// This is an array of leds.  One item for each led in your strip.
CRGB leds[NUM_LEDS];
int command;
int prevCommand;
int foreground = 0;
uint32_t color;


// Debugging routine that blinks the small built-in LED on the Arduino board
// On the Arduino Ethernet, it's next to the green power LED by the power connector
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

// This function sets up the leds and tells the controller about them
void setup() 
{
  pinMode(LED_BUILTIN, OUTPUT);
  //blink(4);
  
  //This is the chipset in the AM-2640 LED strip
  FastLED.addLeds<WS2801, RGB>(leds, NUM_LEDS);
  FastLED.clear();
  // Set brightness from 1-100
  FastLED.setBrightness(80);

  // Open the serial port, configuring the speed to 9600 baud
  Serial.begin(9600);
  
  // Initialize the command and previous command to a special value
  command = prevCommand = POWER_UP;
  color = commandToColor(command);
}

// This function runs over and over, and is where you do the magic to light
// your leds.
void loop()
{
  blink(2);
  // If something has come in on the serial port, read it in
#if 0
  if (Serial.available() > 0) {
    command = Serial.read();
    // If the command is a carriage return, newline, or space,
    // ignore it by reverting to the previous command value
    // That way, print and println from the cRIO have the same effect
    if (command == '\r' || command == '\n' || command == ' ')
      command = prevCommand;
  }
  if (command != prevCommand) {
    // This is a good place to run code once per new command that comes in
    prevCommand = command;
    color = commandToColor(command);
  }
#else
  color = CRGB::Red;
#endif
  // This code runs once everytime through the loop regardless of whether a new command arrived
  // The function can't be an infinite loop or we'll never read more commands!!
//  alternate_colors(CRGB::White, color);
  color_chase(color, 100);
  // Slow the loop down to produce a visual effect that we can see (vs a general blur of color)
  delay(100);
}

// Translate a command (a single character sent over the serial cable) to a color
uint32_t commandToColor(int command)
{
  // Color constants are defined here:
  //https://code.google.com/p/fastspi/wiki/CRGBreference	
  uint32_t color;
  switch(command) {
  case 'w':
    color = CRGB::White;
    break;
  // Include POWER_UP on whichever case should run at power-on
  case POWER_UP:
  case 'r':
    color = CRGB::Red;
    break;
  case 'b':
    color = CRGB::Blue;
    break;
  case 'g':
    color = CRGB::Green;
    break;
  default:
    // If an unknown command is sent, the default case will happen
    color = CRGB::Black;
  }
  return color;
}

// Set a pattern of foreground led, then two background colors, repeating.
// Over time, the foreground LED is moved up the line.
void alternate_colors(uint32_t fgcolor, uint32_t bgcolor)
{
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

//Move an "empty" dot down the strip
void missing_dot_chase(uint32_t color, uint8_t wait)
{
  int led_number;

  for (int led_brightness = 100; led_brightness > 10; led_brightness/=2)
  {
    FastLED.setBrightness(led_brightness);
    // Start by turning all pixels on:
    for(led_number = 0; led_number < NUM_LEDS; led_number++) leds[led_number] = color;

    // Then display one pixel at a time:
    for(led_number = 0; led_number < NUM_LEDS; led_number++)
    {
      leds[led_number] = CRGB::Black; // Set new pixel 'off'
      if( led_number > 0 && led_number < NUM_LEDS)
      {
        leds[led_number-1] = color; // Set previous pixel 'on'
      }
      FastLED.show();
      delay(wait);
    }
  }
}







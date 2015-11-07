/*
* Turn on the LED when we get a byte via the Serial port
 */

// Pin 13 has an LED connected on most Arduino boards.
// give it a name:
int led = 9;

void blink(int count)
{
  for (int i=0; i<count; i++) {
    digitalWrite(led, HIGH);   // turn the LED on (HIGH is the voltage level)
    delay(250);
    digitalWrite(led, LOW);    // turn the LED off by making the voltage LOW
    delay(250);
  }
  delay(750);               // wait for a second
}

// the setup routine runs once when you press reset:
void setup() {                
  Serial.begin(9600);
  // initialize the digital pin as an output.
  pinMode(led, OUTPUT);     
  digitalWrite(led, LOW);    // turn the LED off by making the voltage LOW
  blink(3);
  delay(1000);
}

// the loop routine runs over and over again forever:
void loop() {
  if (Serial.available()) {
    digitalWrite(led, HIGH);   // turn the LED on (HIGH is the voltage level)
    delay(1000);               // wait for a second
    digitalWrite(led, LOW);    // turn the LED off by making the voltage LOW
    delay(1000);               // wait for a second
    unsigned int input = Serial.read();
    for (int i=7; i>=0; i--) {
      if (((input>>i) & 0x01) == 0) {
        // bit == 0
        blink(1);
      } 
      else {
        blink(2);
      }
    }
    delay(500);
  } 
  else {
    digitalWrite(led, LOW);    // turn the LED off by making the voltage LOW
    delay(10);               // wait for a second
  }
}




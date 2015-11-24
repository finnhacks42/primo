// This script tests the RED indicator LEDs.
// If all is going well, they should come on in order for half a second each.
// If any of the LEDs do not come on, check the soldering/connections on your PCB (use a multimeter on the connectivity/beep setting)


int indx = 0;
int redLeds[] = {30,26,22,18,19,23,27,31,32,28,24,20,33,29,25,21};

// the setup function runs once when you press reset or power the board
void setup() {
  for (int i = 0; i < 16; i++){
    pinMode(redLeds[i],OUTPUT);
  }
}

// the loop function runs over and over again forever
void loop() {
  int led = redLeds[indx];
  indx ++;
  indx  = indx % 16;
  Serial.println(indx);
  digitalWrite(led, HIGH);   // turn the LED on (HIGH is the voltage level)
  delay(500);              // wait for a second
  digitalWrite(led, LOW);    // turn the LED off by making the voltage LOW
  delay(100);              // wait for a second
}

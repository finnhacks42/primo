int indx = 0;
int redLeds[] = {18,19,20,21,22,23,24,25,26,27,28,29,30,31,32,33};
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

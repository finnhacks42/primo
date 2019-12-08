// Run this script to test if the wheels on the robot are working correctly.
// The robot should go forward, left, right, forward, left, right, etc

#define FORWARD_TIME 20000
#define PAUSE_TIME 2000
#define LEFT_FORWARD_PIN 9
#define LEFT_REVERSE_PIN 10
#define RIGHT_FORWARD_PIN 6
#define RIGHT_REVERSE_PIN 5
#define INTERRUPT_PIN 7  // interrupt 4 on pro-micro
#define LEFT_SPEED 115
#define RIGHT_SPEED 125

//------------------------------------------------------------------------------------------------

void setup() {
  Serial.begin(115200);
  delay(2000);
  pinMode(LEFT_FORWARD_PIN, OUTPUT);
  pinMode(LEFT_REVERSE_PIN, OUTPUT);
  pinMode(RIGHT_FORWARD_PIN, OUTPUT);
  pinMode(RIGHT_REVERSE_PIN, OUTPUT);
}

void loop() {
  leftForward();
  rightForward();
  delay(FORWARD_TIME);
  off();
  delay(PAUSE_TIME);
}

void off(){
  analogWrite(LEFT_FORWARD_PIN,0);
  analogWrite(LEFT_REVERSE_PIN,0);
  analogWrite(RIGHT_FORWARD_PIN,0);
  analogWrite(RIGHT_REVERSE_PIN,0);
}

void leftForward(){
  analogWrite(LEFT_REVERSE_PIN,0);
  analogWrite(LEFT_FORWARD_PIN,LEFT_SPEED);
}

void rightForward(){
  analogWrite(RIGHT_REVERSE_PIN,0);
  analogWrite(RIGHT_FORWARD_PIN,RIGHT_SPEED);
}


// Run this script to test if the wheels on the robot are working correctly.
// The robot should go forward, left, right, forward, left, right, etc



#define leftForwardPin 2
#define leftReversePin 7
#define leftEnablePin 3
#define rightForwardPin 8
#define rightReversePin 5
#define rightEnablePin 9
#define leftSpeed 125
#define rightSpeed 125
#define pauseTime 2000
#define forwardTime 1000
#define leftTime 1000
#define rightTime 1000

void setup() {
  pinMode(leftForwardPin,OUTPUT);
  pinMode(leftReversePin,OUTPUT);
  pinMode(leftEnablePin,OUTPUT);
  pinMode(rightForwardPin,OUTPUT);
  pinMode(rightReversePin,OUTPUT);
  pinMode(rightEnablePin,OUTPUT);

}

void loop() {
  forward();
  delay(pauseTime);
  right();
  delay(pauseTime);
  left();
  delay(pauseTime);

}

void on(){
   analogWrite(leftEnablePin,leftSpeed);
   analogWrite(rightEnablePin,rightSpeed);
}

void off(){
  digitalWrite(leftEnablePin,LOW);
  digitalWrite(rightEnablePin,LOW);
}

void motorForward(int forwardPin, int reversePin){
  digitalWrite(forwardPin,HIGH);
  digitalWrite(reversePin,LOW);
}

void motorReverse(int forwardPin, int reversePin){
  digitalWrite(forwardPin,LOW);
  digitalWrite(reversePin,HIGH);
}

void forward(){
  Serial.println("forward");
  motorForward(leftForwardPin,leftReversePin);
  motorForward(rightForwardPin,rightReversePin);
  on();
  delay(forwardTime);
  off();
}

void right(){
  Serial.println("right");
  motorForward(leftForwardPin,leftReversePin);
  motorReverse(rightForwardPin,rightReversePin);
  on();
  delay(rightTime);
  off();
}

void left(){
  Serial.println("left");
  motorForward(rightForwardPin, rightReversePin);
  motorReverse(leftForwardPin,leftReversePin);
  on();
  delay(leftTime);
  off();
}


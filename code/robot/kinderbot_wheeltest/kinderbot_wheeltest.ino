// Run this script to test if the wheels on the robot are working correctly.
// The robot should go forward, left, right, forward, left, right, etc

//-----SECTION TO COPY TO KinderBot.ino--------------
#define leftSpeed 165
#define rightSpeed 180
#define forwardTime 1000
#define leftTime 550
#define rightTime 480
//---------------------------------------------------

#define leftForwardPin 8
#define leftReversePin 5
#define leftEnablePin 9
#define rightForwardPin 2
#define rightReversePin 7
#define rightEnablePin 3

#define calibrationTime 20000
#define pauseTime 2000

void setup() {
  pinMode(leftForwardPin,OUTPUT);
  pinMode(leftReversePin,OUTPUT);
  pinMode(leftEnablePin,OUTPUT);
  pinMode(rightForwardPin,OUTPUT);
  pinMode(rightReversePin,OUTPUT);
  pinMode(rightEnablePin,OUTPUT);

}

void loop() {
  forward_left_right();
}

void forward_left_right(){
  forward();
  delay(pauseTime);
  right();
  delay(pauseTime);
  left();
  delay(pauseTime);
}

void forward_long(){
  Serial.println("forward");
  motorForward(leftForwardPin,leftReversePin);
  motorForward(rightForwardPin,rightReversePin);
  on();
  delay(calibrationTime);
  off();
  delay(2*pauseTime);
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


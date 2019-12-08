// Run this script to test if the wheels on the robot are working correctly.
// The robot should go forward, left, right, forward, left, right, etc

#include <Wire.h> //I2C Arduino Library
#include <Adafruit_Sensor.h>
#include <Adafruit_HMC5883_U.h>


#define address 0x1E //0011110b, I2C 7bit address of HMC5883

//-----SECTION TO COPY TO KinderBot.ino--------------
#define forwardTime 800
#define leftTime 450
#define rightTime 450
#define DEFAULT_LEFT 200
#define DEFAULT_RIGHT 200
//---------------------------------------------------

#define piezoPin 15
#define leftForwardPin 14
#define leftReversePin 16
#define leftEnablePin 10

#define rightForwardPin 8
#define rightReversePin 6
#define rightEnablePin 9

#define calibrationTime 20000
#define pauseTime 2000
#define LEFT 1
#define RIGHT 2
#define FORWARD 3


int leftSpeed=180;
int rightSpeed=180;
float k = 1;


Adafruit_HMC5883_Unified mag = Adafruit_HMC5883_Unified(12345);

float x,z; // heading data
float initial_heading;
float angle;
unsigned long initial_time;
unsigned long current_time;
float current_heading;
int state;

void setup() {
  Serial.begin(9600);
  
  /* Initialise the sensor */
  if(!mag.begin())
  {
    /* There was a problem detecting the HMC5883 ... check your connections */
    Serial.println("Ooops, no HMC5883 detected ... Check your wiring!");
    while(1);
  }

  pinMode(leftForwardPin, OUTPUT);
  pinMode(leftReversePin, OUTPUT);
  pinMode(leftEnablePin, OUTPUT);
  pinMode(rightForwardPin, OUTPUT);
  pinMode(rightReversePin, OUTPUT);
  pinMode(rightEnablePin, OUTPUT);
  delay(3000);
}

void loop() {
  forward();
  delay(1000);
  turn_left();
  delay(1000);
  turn_right();
  delay(1000);
}

//try out hard stop (mommentum may be accounting for a substantial amount of turn.

float angle_diff(float a1, float a2) {
  float result = a1 - a2;
  result += (result > 180) ? -360 : (result <- 180) ? 360 : 0;
  return result;
}

void go_forward() {
  Serial.println("forward");
  initial_heading = read_heading();
  initial_time = millis();
  motorForward(leftForwardPin, leftReversePin);
  motorForward(rightForwardPin, rightReversePin);
  on();
  leftSpeed = DEFAULT_LEFT;
  rightSpeed = DEFAULT_RIGHT;
  while ((millis() - initial_time) < forwardTime) {
      // change the ratio of left to right speed in proportion to the degree to which the heading has changed.
      current_heading = read_heading(); // want to have decreased by 90.
      angle = angle_diff(initial_heading,current_heading);
      // positive angle => turn left, negative angle => turn right
      float tmp = leftSpeed + ((int)(k*angle));
      if (tmp < 254 & tmp > 120) {
        leftSpeed = tmp;
      }
      tmp = rightSpeed - (int)(k*angle);
      if (tmp < 254 & tmp > 120) {
        rightSpeed = tmp;
      }
      adjust_speed(leftSpeed,rightSpeed);
      Serial.print(angle);Serial.print(',');Serial.print(leftSpeed);Serial.print(',');Serial.println(rightSpeed);
      delay(20);
  }
  
}

void turn_right() {
  Serial.println("right");
  initial_heading = read_heading();
  initial_time = millis();
  motorReverse(rightForwardPin, rightReversePin);
  motorForward(leftForwardPin, leftReversePin);
  on();
  while ((millis() - initial_time) < leftTime) {
     delay(20);
     current_heading = read_heading(); // want to have decreased by 90.
     angle = angle_diff(initial_heading,current_heading);
     if (angle > 45) {
       break;
     }
  }  
  off();
}

void turn_left() {
  Serial.println("left");
  initial_heading = read_heading();
  initial_time = millis();
  motorForward(rightForwardPin, rightReversePin);
  motorReverse(leftForwardPin, leftReversePin);
  on();
  while ((millis() - initial_time) < leftTime) {
     delay(20);
     current_heading = read_heading(); // want to have decreased by 90.
     angle = angle_diff(initial_heading,current_heading);
     if (angle < -45) {
       break;
     }
  }  
  off();
}

void forward_right_left() {
  forward();
  delay(pauseTime);
  right();
  delay(pauseTime);
  left();
  delay(2*pauseTime);
}

void forward_long() {
  Serial.println("forward");
  motorForward(leftForwardPin, leftReversePin);
  motorForward(rightForwardPin, rightReversePin);
  on();
  delay(calibrationTime);
  off();
  delay(2 * pauseTime);
}

float read_heading(){
  sensors_event_t event; 
  mag.getEvent(&event);
  x = (event.magnetic.x - 85.224)*(1.06936);
  z = (event.magnetic.z + 12.652)*(.95619);
  float heading = 4068*atan2(x,z)/71;
  Serial.print(x);Serial.print(',');Serial.print(z);Serial.print(',');Serial.println(heading);
  return heading;
}

void on() {
  analogWrite(leftEnablePin, DEFAULT_LEFT);
  analogWrite(rightEnablePin, DEFAULT_RIGHT);
}

void adjust_speed(int leftspeed, int rightspeed) {
  analogWrite(leftEnablePin, leftspeed);
  analogWrite(rightEnablePin, rightspeed);
}

void off() {
  digitalWrite(leftEnablePin, LOW);
  digitalWrite(rightEnablePin, LOW);
}

void motorForward(int forwardPin, int reversePin) {
  digitalWrite(forwardPin, HIGH);
  digitalWrite(reversePin, LOW);
}

void motorReverse(int forwardPin, int reversePin) {
  digitalWrite(forwardPin, LOW);
  digitalWrite(reversePin, HIGH);
}


void forward() {
  Serial.println("forward");
  motorForward(leftForwardPin, leftReversePin);
  motorForward(rightForwardPin, rightReversePin);
  on();
  delay(forwardTime);
  off();
}

void right() {
  Serial.println("right");
  motorForward(leftForwardPin, leftReversePin);
  motorReverse(rightForwardPin, rightReversePin);
  on();
  delay(rightTime);
  off();
}

void left() {
  Serial.println("left");
  motorForward(rightForwardPin, rightReversePin);
  motorReverse(leftForwardPin, leftReversePin);
  on();
  delay(leftTime);
  off();
}


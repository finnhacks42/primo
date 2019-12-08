// This script tests the the combination of the wheels and MPU sensor to drive the robot in straight lines and 90 degree angles. 
#include <Wire.h> //I2C Arduino Library
#include <I2Cdev.h>

#define forwardTime 2000
#define leftTime 1000
#define rightTime 1000
#define DEFAULT_LEFT 117
#define DEFAULT_RIGHT 123

#define piezoPin 15
#define LEFT_FORWARD_PIN 9
#define LEFT_REVERSE_PIN 10
#define RIGHT_FORWARD_PIN 6
#define RIGHT_REVERSE_PIN 5

#define INTERRUPT_PIN 7  // interrupt 4 on pro-micro
#define LEFT 'L'
#define RIGHT 'R'
#define FORWARD 'F'

double leftSpeed=117;
double rightSpeed=123;
char instruction = '0';

void setup() {
  Serial.begin(115200);
  Serial1.begin(9600);
  delay(2000);
  pinMode(LEFT_FORWARD_PIN, OUTPUT);
  pinMode(LEFT_REVERSE_PIN, OUTPUT);
  pinMode(RIGHT_FORWARD_PIN, OUTPUT);
  pinMode(RIGHT_REVERSE_PIN, OUTPUT);
}

void loop() {
  if (Serial1.available()){
    instruction = Serial1.read();
    Serial.write(instruction);
    switch (instruction) {
      case FORWARD:
        go_forward();
        break;

      case LEFT:
        turn(LEFT);
        break;

      case RIGHT:
        turn(RIGHT);
        break; 
    } 
  }
}


void go_forward(){
  Serial.println("Forward");
  leftSpeed = DEFAULT_LEFT;
  rightSpeed = DEFAULT_RIGHT;
  leftForward();
  rightForward();
  delay(forwardTime);
  off();
  delay(1000);
}

void turn(char direct){
  leftSpeed = DEFAULT_LEFT;
  rightSpeed = DEFAULT_RIGHT;
  Serial.println(direct);
  if (direct == LEFT) {
     leftReverse();
     rightForward();
  }
  else {
      leftForward();
      rightReverse();
  }
  delay(leftTime);
}

void off(){
  analogWrite(LEFT_FORWARD_PIN,0);
  analogWrite(RIGHT_FORWARD_PIN,0);
  analogWrite(LEFT_REVERSE_PIN,0);
  analogWrite(RIGHT_REVERSE_PIN,0);
}

void leftForward(){
  analogWrite(LEFT_REVERSE_PIN,0);
  analogWrite(LEFT_FORWARD_PIN,leftSpeed);
}

void rightForward(){
  analogWrite(RIGHT_REVERSE_PIN,0);
  analogWrite(RIGHT_FORWARD_PIN,rightSpeed);
}

void leftReverse(){
  analogWrite(LEFT_FORWARD_PIN,0);
  analogWrite(LEFT_REVERSE_PIN,leftSpeed);
}

void rightReverse(){
  analogWrite(RIGHT_FORWARD_PIN,0);
  analogWrite(RIGHT_REVERSE_PIN,rightSpeed);
}

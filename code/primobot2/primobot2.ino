#include <SoftwareSerial.h>

/*
http://primo.io

This sketch is part of the Primo Prototype Documentation.
http://docs.primo.io

Tested on the Arduino UNO.
Load this into Cubetto, the little cube robot.
*/

#define FORWARD 1
#define BACKWARD 0

//serial protocol
#define STOP 'O'
#define LEFT 'L'
#define RIGHT 'R'
#define FORWARD 'F'
#define INIT 'I'

//left motor
const int leftEnable = 2;
const int leftForward = 9;
const int leftReverse = 5;

//right motor
const int rightEnable = 8;
const int rightForward = 7;
const int rightReverse = 3;

//encoders
const int leftEncoder = A1;
const int rightEncoder = A0;

char instruction = 'O';

SoftwareSerial mySerial(10, 11); // RX, TX

void setup() {
  Serial.begin(9600);
  delay(1000);
  mySerial.begin(9600);

  //initialize left
  pinMode(leftEnable, OUTPUT);
  pinMode(leftReverse, OUTPUT);
  pinMode(leftForward, OUTPUT);

  //initialize right
  pinMode(rightEnable, OUTPUT);
  pinMode(rightForward, OUTPUT);
  pinMode(rightReverse, OUTPUT);

  //enable motors
  digitalWrite(leftEnable, HIGH);
  digitalWrite(rightEnable, HIGH);

  //initialize aligns the wheels
  initialize();

  delay(2000);
  Serial.println("INITIALIZED");
}

void loop() {

  //read rom the xbee
  if (mySerial.available()) {
    instruction = mySerial.read();
    Serial.write(instruction);
  }

  //decode instruction
  switch (instruction) {    

    case FORWARD:
      initialize();
      digitalWrite(11, HIGH);
      forward(128, 16);
      break;

    case LEFT:
      initialize();
      left(128, 9);
      break;

    case RIGHT:
      initialize();
      right(138, 7);
      break;

    case INIT:
      initialize();
      break;
  
    case STOP:
      stop();
      break;

    default:
      stop();
      break;
  }
}







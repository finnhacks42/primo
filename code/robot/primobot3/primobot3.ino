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

const int leftForward = 2;
const int leftReverse = 9;
const int leftEnable = 5;

//right motor
const int rightReverse = 8;
const int rightForward = 7;
const int rightEnable = 3;

//encoders
const int leftEncoder = A0;
const int rightEncoder = A1;

char instruction = 'O';

SoftwareSerial mySerial(11, 10); // RX, TX

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
  //learnThreshold2();
  Serial.println("INITIALIZED");
}

void loop() {
  //read from the bluetooth
  if (mySerial.available()) {
    instruction = mySerial.read();
    Serial.write(instruction);
  

  //decode instruction
  switch (instruction) {    

  case FORWARD:
    //initialize();
    //digitalWrite(11, HIGH);
    Serial.println("Going Forward");
    forward(255, 20);
    break;

  case LEFT:
    //initialize();
    left(255, 12);
    break;

  case RIGHT:
    //initialize();
    right(255, 12);
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
}










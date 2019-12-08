// Run this script to test if the wheels on the robot are working correctly.
// The robot should go forward, left, right, forward, left, right, etc
#include <Wire.h> //I2C Arduino Library
#include <AutoPID.h>
#include <I2Cdev.h>
#include <MPU6050_6Axis_MotionApps20.h>
#include <EEPROM.h>

#define forwardTime 2000
#define leftTime 1000
#define rightTime 1000
#define DEFAULT_LEFT 100
#define DEFAULT_RIGHT 100

#define piezoPin 15
#define LEFT_FORWARD_PIN 9
#define LEFT_REVERSE_PIN 10
#define RIGHT_FORWARD_PIN 6
#define RIGHT_REVERSE_PIN 5
#define INTERRUPT_PIN 7  // interrupt 4 on pro-micro


#define LEFT 'L'
#define RIGHT 'R'
#define FORWARD 'F'

//-------------------------------------MPU contol/data--------------------------------------------
bool dmpReady = false;  // set true if DMP init was successful
uint8_t mpuIntStatus;   // holds actual interrupt status byte from MPU
uint8_t devStatus;      // return status after each device operation (0 = success, !0 = error)
uint16_t packetSize;    // expected DMP packet size (default is 42 bytes)
uint16_t fifoCount;     // count of all bytes currently in FIFO
uint8_t fifoBuffer[64]; // FIFO storage buffer

// orientation/motion vars
Quaternion q;           // [w, x, y, z]         quaternion container
VectorInt16 aa;         // [x, y, z]            accel sensor measurements
VectorInt16 aaReal;     // [x, y, z]            gravity-free accel sensor measurements
VectorInt16 aaWorld;    // [x, y, z]            world-frame accel sensor measurements
VectorFloat gravity;    // [x, y, z]            gravity vector
float euler[3];         // [psi, theta, phi]    Euler angle container
float ypr[3];           // [yaw, pitch, roll]   yaw/pitch/roll container and gravity vector

volatile bool mpuInterrupt = false;     // indicates whether MPU interrupt pin has gone high
void dmpDataReady() {
    mpuInterrupt = true;
}

//------------------------------------------------------------------------------------------------
double leftSpeed=DEFAULT_LEFT;
double rightSpeed=DEFAULT_RIGHT;

MPU6050 mpu;
double initial_heading;
double angle;
double prev_angle;
//float delta_angle;
unsigned long initial_time;
unsigned long delta_time;
unsigned long lastread; //when was the sensor last read
unsigned long current_time;
unsigned long deadline;
double current_heading;
double rightOvershoot = 25;
double leftOvershoot = 25;
int state;

double setPoint = 0.0;
double speedDifference = 0;

//int is 2 bytes.

//pid settings and gains
#define KP .5
#define KI 0
#define KD 0
AutoPID speedPID(&angle, &setPoint, &speedDifference, -5, 5 , KP, KI, KD);
int eeAddressRight = 0;
int eeAddressLeft = 2;

void setup() {
  eeAddressLeft = sizeof(rightOvershoot);
  Serial.begin(115200);
  delay(2000);
  setupMPU();
  pinMode(LEFT_FORWARD_PIN, OUTPUT);
  pinMode(LEFT_REVERSE_PIN, OUTPUT);
  pinMode(RIGHT_FORWARD_PIN, OUTPUT);
  pinMode(RIGHT_REVERSE_PIN, OUTPUT);
  speedPID.setTimeStep(10);
  delay(10000);
  EEPROM.get(eeAddressRight,rightOvershoot);
  EEPROM.get(eeAddressLeft, leftOvershoot);
  if (isnan(leftOvershoot) || leftOvershoot < 0 || leftOvershoot > 50){
    leftOvershoot = 15;
  }
  if (isnan(rightOvershoot) || rightOvershoot < 0 || rightOvershoot > 50){
    rightOvershoot = 15;
  } 
}

void loop() {
  //Serial.print("leftOvershoot:");Serial.print(leftOvershoot);Serial.print(", rightOvershoot:");Serial.println(rightOvershoot);
  go_forward();
  delay(1000);
  turn(LEFT);
  delay(1000);
  turn(LEFT);
  delay(1000);
  go_forward();
  delay(1000);
  turn(RIGHT);
  delay(1000);
  turn(RIGHT);
  delay(1000);
  EEPROM.put(eeAddressRight,rightOvershoot);
  EEPROM.put(eeAddressLeft,leftOvershoot);
}

float angle_diff(float a1, float a2) {
  float result = a1 - a2;
  if (result > 180) {result = -(360 - result);}
  else if (result < -180) {result = (360 + result);}
  return result;
}

void clearMPU(){ //mpu buffer may have overflowed while we waited for the next instruction.
  mpuInterrupt = false;
  mpuIntStatus = mpu.getIntStatus();
  fifoCount = mpu.getFIFOCount();
  //if ((mpuIntStatus & 0x10) || fifoCount == 1024) {
        mpu.resetFIFO();
        fifoCount = 0;
  //}
}

int clip(float v,int minval, int maxval) {
  // clip an input to a range
  if (v > maxval) {return maxval;}
  if (v < minval) {return minval;}
  return (int)v;
}

void go_forward2(){
  leftSpeed = DEFAULT_LEFT;
  rightSpeed = DEFAULT_RIGHT;
  leftForward();
  rightForward();
  delay(forwardTime);
  off();
}

void go_forward() {
  //Serial.println("forward");
  clearMPU();
  initial_heading = read_heading();
  //Serial.print("initial heading:");Serial.println(initial_heading);
  initial_time = millis();
  speedPID.reset();
  leftSpeed = DEFAULT_LEFT;
  rightSpeed = DEFAULT_RIGHT;
  speedPID.run();
  leftForward();
  rightForward();
  clearMPU();
  while ((millis() - initial_time) < forwardTime) {
      current_heading = read_heading(); 
      if (current_heading < 1000) {
        angle = angle_diff(initial_heading,current_heading);
        // positive angle => turn left, negative angle => turn right
        speedPID.run();
        leftSpeed = DEFAULT_LEFT - speedDifference;
        rightSpeed = DEFAULT_RIGHT + speedDifference;
        leftForward();
        rightForward();
        //Serial.print("input:");Serial.print(angle);Serial.print(" setpoint:");Serial.print(setPoint);Serial.print(" output:");Serial.println(speedDifference);
        Serial.print(angle);Serial.print(',');Serial.print(leftSpeed);Serial.print(',');Serial.println(rightSpeed);
      }
  }
  off();
  speedPID.stop();
}

void turn(char direct){
  leftSpeed = DEFAULT_LEFT;
  rightSpeed = DEFAULT_RIGHT;
  //Serial.println(direct);
  clearMPU();
  initial_heading = read_heading_retry(5);
  double *overshoot;  
  if (direct == LEFT) {
     leftReverse();
     rightForward();
     overshoot = &leftOvershoot; // does this mean that changing overshoot changes the value of leftOvershoot?
  }
  else {
      leftForward();
      rightReverse();
      overshoot = &rightOvershoot;
  }
  double target_angle = 90.0 - *overshoot;
  initial_time = millis();  
  deadline = millis() + leftTime;
  lastread = millis();
  angle = 0; 
  while (millis() < deadline) {
     current_heading = read_heading(); 
     if (current_heading < 1000){
       prev_angle = angle;
       angle = abs(angle_diff(initial_heading,current_heading));
       current_time = millis();
       delta_time = current_time - lastread;
       lastread = current_time;
       Serial.print(delta_time);Serial.print(',');Serial.println(angle);
       if (angle > target_angle) {
         break;
       }
     } 
  }  
  off();
  deadline = millis()+300; //allow 300ms for deceleration
  while (millis() < deadline) { 
    current_heading = read_heading(); 
    if (current_heading < 1000){
       prev_angle = angle;
       angle = abs(angle_diff(initial_heading,current_heading));
       current_time = millis();
       delta_time = current_time - lastread;
       lastread = current_time;
       Serial.print(delta_time);Serial.print(',');Serial.println(angle);
    } 
  }
  //clearMPU();
  current_heading = read_heading_retry(5); // want to have decreased by 90.
  angle = abs(angle_diff(initial_heading,current_heading));
  *overshoot += angle - 90;
  current_time = millis();
  delta_time = current_time - lastread;
  Serial.print(delta_time);Serial.print(',');Serial.println(angle);
  Serial.print("OVS");Serial.print(leftOvershoot);Serial.print(':');Serial.println(rightOvershoot);
}

float read_heading_retry(int retries){
  int attempts = 0;
  float result = 1000;
  while (attempts <= retries) {
    result = read_heading();
    if (result < 1000){
      return result;
    }
    attempts +=1;
  }
  return result;
}

float read_heading(){
    while (!mpuInterrupt && fifoCount < packetSize) {} //wait for data to be available
    float heading = 1000;
    mpuInterrupt = false;
    mpuIntStatus = mpu.getIntStatus();

    // get current FIFO count
    fifoCount = mpu.getFIFOCount();

    // check for overflow (this should never happen unless our code is too inefficient)
    if ((mpuIntStatus & 0x10) || fifoCount == 1024) {
        // reset so we can continue cleanly
        mpu.resetFIFO();
        //Serial.println(F("FIFO overflow!"));
    // otherwise, check for DMP data ready interrupt (this should happen frequently)
    } else if (mpuIntStatus & 0x02) {
        // wait for correct available data length, should be a VERY short wait
        while (fifoCount < packetSize) {fifoCount = mpu.getFIFOCount();}

        int packetsRead = 0;
        while (fifoCount >= packetSize) {

          // read a packet from FIFO
          mpu.getFIFOBytes(fifoBuffer, packetSize);
          
          // track FIFO count here in case there is > 1 packet available
          // (this lets us immediately read more without waiting for an interrupt)
          fifoCount -= packetSize;
          packetsRead ++;
        }
        
        // display Euler angles in degrees
        mpu.dmpGetQuaternion(&q, fifoBuffer);
        mpu.dmpGetGravity(&gravity, &q);
        mpu.dmpGetYawPitchRoll(ypr, &q, &gravity);
        heading = (ypr[0] * 180/M_PI);
    }
  return heading;
}

void setupMPU() {
    // join I2C bus (I2Cdev library doesn't do this automatically)
    Wire.begin();
    Wire.setClock(400000); // 400kHz I2C clock. Comment this line if having compilation difficulties
    // initialize device
    //Serial.println(F("Initializing I2C devices..."));
    mpu.initialize();
    pinMode(INTERRUPT_PIN, INPUT);

    // verify connection
    //Serial.println(F("Testing device connections..."));
    //Serial.println(mpu.testConnection() ? F("MPU6050 connection successful") : F("MPU6050 connection failed"));
    delay(2000);
    // load and configure the DMP
    //Serial.println(F("Initializing DMP..."));
    devStatus = mpu.dmpInitialize();
    // supply your own gyro offsets here, scaled for min sensitivity
    mpu.setXGyroOffset(33);
    mpu.setYGyroOffset(-25);
    mpu.setZGyroOffset(-47);
    mpu.setZAccelOffset(5094); // 1688 factory default for my test chip

    // make sure it worked (returns 0 if so)
    if (devStatus == 0) {
        // turn on the DMP, now that it's ready
        //Serial.println(F("Enabling DMP..."));
        mpu.setDMPEnabled(true);

        // enable Arduino interrupt detection
        //Serial.println(F("Enabling interrupt detection..."));
        attachInterrupt(digitalPinToInterrupt(INTERRUPT_PIN), dmpDataReady, RISING);
        mpuIntStatus = mpu.getIntStatus();

        // set our DMP Ready flag so the main loop() function knows it's okay to use it
        //Serial.println(F("DMP ready! Waiting for first interrupt..."));
        dmpReady = true;
        // get expected DMP packet size for later comparison
        packetSize = mpu.dmpGetFIFOPacketSize();
    } else {
        // ERROR!
        // 1 = initial memory load failed
        // 2 = DMP configuration updates failed
        // (if it's going to break, usually the code will be 1)
        //Serial.print(F("DMP Initialization failed (code "));
        //Serial.print(devStatus);
        //Serial.println(F(")"));
    }
}

void off(){
  analogWrite(LEFT_FORWARD_PIN,0);
  analogWrite(LEFT_REVERSE_PIN,0);
  analogWrite(RIGHT_FORWARD_PIN,0);
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


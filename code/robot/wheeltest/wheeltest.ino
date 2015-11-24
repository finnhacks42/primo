/* H-bridge Sketch
 Simply reverses the motor.
 */
const int  leftReverse= 8;
const int leftForward = 9;
const int leftEnable = 6;

const int rightReverse = 12;
const int rightForward = 13;
const int rightEnable = 7;

//encoders
const int leftEncoder = A4;
const int rightEncoder = A5;

const int thresh = 975;


void setup() {
    // initialize serial communication at 9600 bits per second:
    Serial.begin(9600);
    //pinMode(leftEncoder,INPUT);
    //pinMode(rightEncoder,INPUT);
    pinMode(rightReverse, OUTPUT);      // OUTPUTS
    pinMode(rightForward, OUTPUT); 
    pinMode(rightEnable, OUTPUT);
    pinMode(leftForward,OUTPUT);
    pinMode(leftReverse,OUTPUT);
    pinMode(leftEnable,OUTPUT);
    digitalWrite(rightEnable, LOW);
    digitalWrite(leftEnable,LOW); 
  
    initialize();  
}

// the loop routine runs over and over again forever:
void loop() {
   // Serial.print(analogRead(leftEncoder));
   //  Serial.print(",");
   //  Serial.println(analogRead(rightEncoder));
}

/* STOP FUNCTIONS */
void stopLeft() {
  digitalWrite(leftForward, LOW);
  digitalWrite(leftReverse, LOW);
}

void stopRight() {
  digitalWrite(rightForward, LOW);
  digitalWrite(rightReverse, LOW);
}

void forwardLeft(){
   digitalWrite(leftForward,HIGH);
   digitalWrite(leftReverse,LOW);
   digitalWrite(leftEnable,HIGH); 
}

void forwardRight(){
  digitalWrite(rightForward,HIGH);
  digitalWrite(rightReverse,LOW);
  digitalWrite(rightEnable,HIGH);
}


//rotate wheels until the next sign
void initialize() {
  int vl = analogRead(leftEncoder);
  int vr = analogRead(rightEncoder);
  Serial.print("Left:");
  Serial.println(vl);
  Serial.print("Right:");
  Serial.println(vr);
  
  forwardRight();
  forwardLeft();
  
  for (int i = 0; i < 1000; i++){
     Serial.print(digitalRead(leftEncoder));
     Serial.print(",");
     Serial.println(digitalRead(rightEncoder));
  }
  
 stopRight();
 stopLeft();
 
}

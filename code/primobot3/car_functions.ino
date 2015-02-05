//one wheel is composed of 32 slices
//16 black and 16 white.

int thresh = 150; //threshold between black and white
int LEARN_STEPS = 1000;

int toDigital(int a) {
  if (a < thresh) {return 0;}
  return 1;
}

// sorts from largest to smallest
void sort(int a[], int asize) {
    for(int i=0; i<(asize-1); i++) {
        for(int o=0; o<(asize-(i+1)); o++) {
                if(a[o] > a[o+1]) {
                    int t = a[o];
                    a[o] = a[o+1];
                    a[o+1] = t;
                }
        }
    }
}

int median(int a[], int asize){
  sort(a,asize);
  int half = asize/2;
  int result = a[half];
  return result;
}

//runs both wheels for x steps
void learnThreshold(){
  int leftvalues[LEARN_STEPS];
  int rightvalues[LEARN_STEPS];
  int left;
  int right;
  
   Serial.println("STARTING MOTORS");
  //enable motors
  digitalWrite(leftEnable, HIGH);
  digitalWrite(rightEnable, HIGH);
  //forward left
  digitalWrite(leftForward, HIGH);
  digitalWrite(leftReverse, LOW);
  //forward right
  digitalWrite(rightForward, HIGH);
  digitalWrite(rightReverse, LOW);
 
  
  for (int i = 0; i < LEARN_STEPS; i++){
    left = analogRead(leftEncoder);
    right = analogRead(rightEncoder);
    leftvalues[i] = left;
    rightvalues[i] = right;
    Serial.print(left);
    Serial.print(",");
    Serial.println(right);
  }
  int leftMed = median(leftvalues,LEARN_STEPS);
  int rightMed = median(rightvalues,LEARN_STEPS);
  
  Serial.print("MEDIAN LEFT: ");
  Serial.println(leftMed);
  Serial.print("MEDIAN RIGHT: ");
  Serial.println(rightMed);
  stopLeft();
  stopRight();
}

//rotate wheels until the next sign
void initialize() {
  Serial.println("Begin init");
  
  int vl = toDigital(analogRead(leftEncoder));
  float signal = 0;
  
   //enable motors
  digitalWrite(leftEnable, HIGH);
  digitalWrite(rightEnable, HIGH);
  
  //forward left
 
  digitalWrite(leftForward, HIGH);
  digitalWrite(leftReverse, LOW);
  int vtmp = toDigital(analogRead(leftEncoder));
  unsigned long starttime = millis();
  unsigned long dt = 0;
  while (vtmp == vl && dt < 1000) {
      signal = analogRead(leftEncoder);
      vtmp = toDigital(signal);
      Serial.println(signal);
      dt = millis() - starttime;
  }
  Serial.println("Stopping left");
  stopLeft();
  delay(100);

  int vr = toDigital(analogRead(rightEncoder));
  vtmp = vr;
  //forward right
  digitalWrite(rightForward, HIGH);
  digitalWrite(rightReverse, LOW);
  starttime = millis();
  dt = 0;
  while (vtmp == vr && dt < 1000){
    signal = analogRead(rightEncoder);
    vtmp = toDigital(signal);
    Serial.println(signal);
    dt = millis() - starttime;
  }
  stopRight();
 
}

/* MOTION FUNCTIONS */

void backward(int speed, int steps) {
  int vL, pvL = 0;
  int vR, pvR = 0;

  int counterL, counterR = 0;

  //right backward
  digitalWrite(rightForward, LOW);
  analogWrite(rightReverse, speed);

  //left backward
  digitalWrite(leftForward, LOW);
  analogWrite(leftReverse, speed);


  while (counterL <= steps && counterR <= steps) {
    if (counterL <= steps) {
      vL = toDigital(analogRead(leftEncoder));
      if (vL != pvL) counterL++;
      pvL = vL;
    }
    if (counterR <= steps) {
      vR = toDigital(analogRead(rightEncoder));
      if (vR != pvR) counterR++;
      pvR = vR;
    }
  }
  stop();
}

void forward(int speed, int steps) {
  int vL, pvL = 0;
  int vR, pvR = 0;

  int counterL, counterR = 0;

  //right forward
  digitalWrite(rightForward, speed);
  analogWrite(rightReverse, LOW);

  //left forward
  digitalWrite(leftForward, speed);
  analogWrite(leftReverse, LOW);


  while (counterL <= steps && counterR <= steps) {
    if (counterL <= steps) {
      vL = toDigital(analogRead(leftEncoder));
      if (vL != pvL) counterL++;
      pvL = vL;
    }
    if (counterR <= steps) {
      vR =toDigital(analogRead(rightEncoder));
      if (vR != pvR) counterR++;
      pvR = vR;
    }
  }

  stop();
}

void left(int speed, int steps) {
  int vL, pvL = 0;
  int vR, pvR = 0;

  int counterL, counterR = 0;

  //right forward
  digitalWrite(rightForward, speed);
  analogWrite(rightReverse, LOW);

  //left backward
  digitalWrite(leftForward, LOW);
  analogWrite(leftReverse, speed);

  //count rotation
  while (counterL <= steps && counterR <= steps) {
    if (counterL <= steps) {
      vL = toDigital(analogRead(leftEncoder));
      if (vL != pvL) counterL++;
      pvL = vL;
    }
    if (counterR <= steps) {
      vR = toDigital(analogRead(rightEncoder));
      if (vR != pvR) counterR++;
      pvR = vR;
    }
  }

  stop();
}

void right(int speed, int steps) {

  int vL, pvL = 0;
  int vR, pvR = 0;

  int counterL = 0, counterR = 0;

  pvL = digitalRead(leftEncoder);
  pvR = digitalRead(rightEncoder);

  //right reverse
  digitalWrite(rightForward, LOW);
  analogWrite(rightReverse, speed);

  //left forward
  digitalWrite(leftReverse, LOW);
  analogWrite(leftForward, speed);

  //count rotation
  while (counterL <= steps || counterR <= steps) {
    if (counterL <= steps) {
      vL = toDigital(analogRead(leftEncoder));
      if (vL != pvL) counterL++;
      pvL = vL;
    }
    if (counterR <= steps) {
      vR = toDigital(analogRead(rightEncoder));
      if (vR != pvR) counterR++;
      pvR = vR;
    }
  }

  stop();
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

void stop() {
  stopLeft();
  stopRight();
}

void hardStop() {
  digitalWrite(leftForward, HIGH);
  digitalWrite(leftReverse, HIGH);
  digitalWrite(rightForward, HIGH);
  digitalWrite(rightReverse, HIGH);
}

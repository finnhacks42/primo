//one wheel is composed of 32 slices
//16 black and 16 white.

int thresh = 100; //threshold between black and white
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
void learnThreshold2(){
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
    Serial.print(left);
    Serial.print(",");
    Serial.println(right);
  }
  stopLeft();
  stopRight();
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

void turn_right_wheel(){
  Serial.println("RIGHT WHEEL GO");
  digitalWrite(rightEnable, HIGH);
  digitalWrite(leftEnable, LOW);
  
  digitalWrite(rightForward, HIGH);
  for (int i = 0; i < 500; i++) {
     int enc = toDigital(analogRead(rightEncoder));
     Serial.println(enc); 
  }
  digitalWrite(rightForward,LOW);
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


void forward(int speed, int steps) {
 turn(steps,true,true); 
}

void left(int speed, int steps) {
  turn(steps,false,true);
}

void right(int speed, int steps) {
  turn(steps,true,false);
}


void turn(int steps, boolean leftIsForward, boolean rightIsForward) {
  Serial.println("CALLED TURN");
  int vL = toDigital(analogRead(leftEncoder));
  int pvL = toDigital(analogRead(leftEncoder));
  int vR = toDigital(analogRead(rightEncoder));
  int pvR = toDigital(analogRead(rightEncoder));
  
  int counterL = 0;
  int counterR = 0;
  
  if (leftIsForward){
    digitalWrite(leftForward, HIGH);
    digitalWrite(leftReverse, LOW);
  } else {
     digitalWrite(leftForward, LOW);
     digitalWrite(leftReverse, HIGH);
  }
  
  if (rightIsForward){
    digitalWrite(rightForward, HIGH);
    digitalWrite(rightReverse, LOW);
  } else {
    digitalWrite(rightForward, LOW);
    digitalWrite(rightReverse, HIGH);
  }
  
  // GO
  digitalWrite(leftEnable, HIGH);
  digitalWrite(rightEnable, HIGH);
  
  unsigned long leftChanged = millis();
  unsigned long rightChanged = millis();
  unsigned long time;
  
  boolean leftStopped = false;
  boolean rightStopped = false;
  
  while (counterL <= steps || counterR <= steps) {  
    time = millis();
    if (time - leftChanged > 500){
      counterL = steps;
      Serial.println("Left stalled");
    }
    if (time - rightChanged > 500){
      counterR = steps;
      Serial.println("Right stalled");
    }
    
    if (counterL <= steps) {
      vL = toDigital(analogRead(leftEncoder));
      if (vL != pvL) {
        leftChanged = millis();
        Serial.print("L:");
        counterL++;
        Serial.println(counterL);
        pvL = vL;
      }
    } 
    if (counterL > steps) {
      if (!leftStopped){
        leftStopped = true;
        stopLeft();
      }
    }
    
    if (counterR <= steps) {
      vR =toDigital(analogRead(rightEncoder));
      if (vR != pvR) {
        rightChanged = millis();
        Serial.print("R:");
        counterR++;
        Serial.println(counterR);
        pvR = vR;
      }
    } 
    if (counterR > steps) {
      if (!rightStopped){
        rightStopped = true;
        stopRight();
      } 
    }
  }
  Serial.println("FINISHED LOOP");  
}



/* STOP FUNCTIONS */
void stopLeft() {
  Serial.println("Stop Left");
  digitalWrite(leftEnable,LOW);
  digitalWrite(leftForward, LOW);
  digitalWrite(leftReverse, LOW);
}

void stopRight() {
  Serial.println("Stop right");
  digitalWrite(rightEnable,LOW);
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

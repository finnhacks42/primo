//one wheel is composed of 32 slices
//16 black and 16 white.

int thresh = 150; //threshold between black and white

int toDigital(int a) {
  if (a < thresh) {return 0;}
  return 1;
}

//rotate wheels until the next sign
void initialize() {
  int vl = toDigital(analogRead(leftEncoder));
  
  //forward left
  digitalWrite(leftForward, HIGH);
  digitalWrite(leftReverse, LOW);
  while (toDigital(analogRead(leftEncoder)) == vl);
  stopLeft();
  delay(100);

  int vr = toDigital(analogRead(rightEncoder));

  //forward right
  digitalWrite(rightForward, HIGH);
  digitalWrite(rightReverse, LOW);
  while (toDigital(analogRead(rightEncoder)) == vr);
  stopRight();
}

/* MOTION FUNCTIONS */

//not used
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
      vL = digitalRead(leftEncoder);
      if (vL != pvL) counterL++;
      pvL = vL;
    }
    if (counterR <= steps) {
      vR = digitalRead(rightEncoder);
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
      vL = digitalRead(leftEncoder);
      if (vL != pvL) counterL++;
      pvL = vL;
    }
    if (counterR <= steps) {
      vR = digitalRead(rightEncoder);
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
      vL = digitalRead(leftEncoder);
      if (vL != pvL) counterL++;
      pvL = vL;
    }
    if (counterR <= steps) {
      vR = digitalRead(rightEncoder);
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
      vL = digitalRead(leftEncoder);
      if (vL != pvL) counterL++;
      pvL = vL;
    }
    if (counterR <= steps) {
      vR = digitalRead(rightEncoder);
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

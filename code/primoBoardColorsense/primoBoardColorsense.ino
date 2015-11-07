int red = 2;
int green = 3;
int blue = 4;

//int leds[] = {22,24,26,34,38,40,36,47,42,44,48,49,46,33,29,31};  
//int inputs[] = {A3,A4,A5,A6,A2,A1,A0,A7,A14,A15,A8,A9,A13,A12,A11,A10};


int leds[] = {46,33,29,31,49,48,44,42,38,40,36,47,22,24,26,34};
int inputs[] = {A13,A12,A11,A10,A9,A8,A15,A14,A2,A1,A0,A7,A3,A4,A5,A6};

int colors[] = {red,green,blue};
int up_d = 20; // time to leave led on before reading value
int down_d = 300; //time to turn off before reading next value
  
void setup(){
    Serial.begin(9600);
    pinMode(red, OUTPUT);
    pinMode(blue, OUTPUT);
    pinMode(green, OUTPUT);
    for (int i = 0; i < 16; i++){
      pinMode(leds[i],OUTPUT);
    }  
}
  
  
// can read all red ones, then all blue, then all green - as fast as possible without delay.   
  
void loop() {
  
  // read the LDRs with everything off
  readLDRs();
  for (int c = 0; c < 3; c ++) { 
      int color = colors[c];
      colorOn(color);     
      delay(up_d);
      readLDRs();
      off();
      delay(down_d);
  }
  Serial.println("");
}

void readLDRs(){
    for (int i = 0; i < 16; i++) {
      int input = inputs[i];
      int value = analogRead(input);
      Serial.print(value);
      Serial.print(",");
    }  
}

void colorOn(int color){
    digitalWrite(red,LOW);
    digitalWrite(green,LOW);
    digitalWrite(blue,LOW);
    digitalWrite(color,HIGH);
    for (int i = 0; i < 16; i++) {
      digitalWrite(leds[i],HIGH);
    }
}

void off(){
    digitalWrite(red,LOW);
    digitalWrite(green,LOW);
    digitalWrite(blue,LOW);
    for (int i = 0; i < 16; i++) {
      digitalWrite(leds[i],LOW);
    }
}
  
int read(int indx, int color){
    off();
    delay(down_d);
    digitalWrite(color,HIGH);
    digitalWrite(leds[indx],HIGH);
    delay(up_d);
    int value = analogRead(inputs[indx]);
    return value;
}
  

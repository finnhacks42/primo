// This script should be loaded onto your board when you run the python script to learn the color responce for each tile.


int red = 9;
int green = 8;
int blue = 7;

int leds[] = {36,40,44,48,47,43,39,35,34,38,42,46,49,37,41,45};
int ldrs[] = {A8,A2,A9,A1,A3,A11,A0,A10,A12,A4,A13,A5,A14,A6,A15,A7};
int redLeds[] =  {30,26,22,18,19,23,27,31,32,28,24,20,33,29,25,21};

int colors[] = {red,green,blue};
int up_d = 30; // time to leave led on before reading value
int down_d = 300; //time to turn off before reading next value
  
void setup(){
    Serial.begin(9600);
    pinMode(red, OUTPUT);
    pinMode(blue, OUTPUT);
    pinMode(green, OUTPUT);
    for (int i = 0; i < 16; i++){
      pinMode(leds[i],OUTPUT);
      pinMode(redLeds[i],OUTPUT);
      digitalWrite(redLeds[i],HIGH);
      pinMode(ldrs[i],INPUT);
    }
    off();  
}
  // each time round, outputs a comma seperated row of 16*4 values.
  // first 16 are ldrs with rgb led off
  // 2nd 16 are ldrs with rgb=red
  // 3rd 16 are ldrs with rgb=green
  // final 16 are ldrs with rbg=blue  
void loop() {
  
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
      int input = ldrs[i];
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
    int value = analogRead(ldrs[indx]);
    return value;
}
  

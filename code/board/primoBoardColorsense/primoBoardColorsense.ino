// This script should be loaded onto your board when you run the python script to learn the color responce for each tile.
// Writes out results in rows 64 ints
// Each row is 4 blocks of 16 ints. OFF, RED, GREEN, BLUE

#define red 9
#define green 8
#define blue 7
#define off 0

#define MAX_COVERED_VALUE 15


int leds[] = {36,40,44,48,47,43,39,35,34,38,42,46,49,37,41,45};
int ldrs[] = {A8,A2,A9,A1,A3,A11,A0,A10,A12,A4,A13,A5,A14,A6,A15,A7};
int redLeds[] =  {30,26,22,18,19,23,27,31,32,28,24,20,33,29,25,21};

// tile, value_off, value_blue, value_green, value_blue
int output[16][5];

int output_indx(int color){
  switch (color) {
       case off:
         return 1;
       case red:
         return 2;
        case green:
         return 3;
        case blue:
          return 4;
  }
}

int colors[] = {red,green,blue};
int up_d = 30; // time to leave led on before reading value
int down_d = 100; //time to turn off before reading next value
  
void setup(){
    Serial.begin(9600);
    pinMode(red, OUTPUT);
    pinMode(blue, OUTPUT);
    pinMode(green, OUTPUT);
    for (int i = 0; i < 16; i++){
      pinMode(leds[i],OUTPUT);
      pinMode(redLeds[i],OUTPUT);
      digitalWrite(redLeds[i],LOW);
      pinMode(ldrs[i],INPUT);
    }
    ledsOff();  
}

  // each time round, outputs a comma seperated row of 16*4 values.
  // first 16 are ldrs with rgb led off
  // 2nd 16 are ldrs with rgb=red
  // 3rd 16 are ldrs with rgb=green
  // final 16 are ldrs with rbg=blue  
void loop() {
  readLDRs(off); 
  for (int c = 0; c < 3; c ++) { 
      int color = colors[c];
      colorOn(color);     
      delay(up_d);
      readLDRs(color);
      ledsOff();
      delay(down_d);
      write_output();
  }
  
}


void readLDRs(int color) {  
  for (int i = 0; i < 16; i++) {
      int input = ldrs[i];
      int value = analogRead(input);
      //ldr_vals[i] = value;
      output[i][0] = i;
      output[i][output_indx(color)] = value;
    }
}

void write_output() {
  for (int row=0; row < 16; row++){
     for (int column=0; column < 5; column ++){
         Serial.print(output[row][column]);
         if (column < 4) {
            Serial.print(","); 
         }
     }
     if (row < 15) {
        Serial.print("|");
     }
  }
  Serial.println("");
}

void colorOn(int color){
    digitalWrite(red,LOW);
    digitalWrite(green,LOW);
    digitalWrite(blue,LOW);
    digitalWrite(color,HIGH);
    for (int i = 0; i < 16; i++) {
      if (output[i][output_indx(off)] < MAX_COVERED_VALUE) {
        digitalWrite(leds[i],HIGH);
      }
    }
}

void ledsOff(){
    digitalWrite(red,LOW);
    digitalWrite(green,LOW);
    digitalWrite(blue,LOW);
    for (int i = 0; i < 16; i++) {
      digitalWrite(leds[i],LOW);
    }
}
  


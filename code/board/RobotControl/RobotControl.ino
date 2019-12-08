#include <SoftwareSerial.h>
#include <limits.h>
#include<math.h>
#define inputs 16
#define repeats 10 // the number of times to measure each analog input - to reduce variance

// Define which pin controls the each color of the RGB leds
#define red 9
#define green 8
#define blue 7
#define FUNCTION 'C'
#define NOOP 'N'


SoftwareSerial mySerial(10, 11); // RX - connect to orange TX line from HC05, TX - connect to green recive line on HCO5

int centers[4][3] = {
   {-75,14,114},
   {-91,-31,-40},
   {72,-62,-43},
   {94,78,-31},
};

int average_brightness[16][3] = {
   {507,588,255},
   {684,686,365},
   {709,643,295},
   {652,670,342},
   {694,410,420},
   {625,439,460},
   {696,508,496},
   {590,361,339},
   {710,606,282},
   {651,661,333},
   {627,663,366},
   {660,632,309},
   {615,520,90},
   {650,661,304},
   {715,699,371},
   {681,559,277},
};





int leds[] = {36,40,44,48,47,43,39,35,34,38,42,46,49,37,41,45};
int ldrs[] = {A8,A2,A9,A1,A3,A11,A0,A10,A12,A4,A13,A5,A14,A6,A15,A7};
int redLeds[] =  {30,26,22,18,19,23,27,31,32,28,24,20,33,29,25,21};


int colors[3] = {red,green,blue};
int light[16][4]; // stores ldr values - background,red,green,blue
int program[16];

int up_d = 30; // time to leave led on before reading value
int down_d = 300; //time to turn off before reading next value
int dark = 20; // max value of ldr with no tile on top - this needs to be tile dependent ...


int buttonPin = 12;
int buttonState = 0;
int currentInstruction = -1;
int dispatchPoint = -1; // if we are in the function, used to store where we came from so we can return back there


void setup()  
{
  Serial.begin(9600);
  mySerial.begin(9600);
  pinMode(buttonPin,INPUT);
  pinMode(red,OUTPUT);
  pinMode(blue,OUTPUT);
  pinMode(green,OUTPUT);
  for (int i = 0; i < inputs; i ++) {
    pinMode(leds[i],OUTPUT); 
    pinMode(redLeds[i],OUTPUT);
  }
}
char instruction(int indx){
   if (indx == 0){return 'L';}
   if (indx == 1){return 'C';}
   if (indx == 2){return 'F';}
   if (indx == 3){return 'R';}
   return 'N';
}

void loop() // run over and over
{
  if (currentInstruction < 0) {
      // read the background light
      readLDRs(0); 
      
      // turn on indicator leds where there is a tile
      for (int i = 0; i < inputs; i++){
        if (light[i][0] < dark){
          digitalWrite(redLeds[i],HIGH);
        } else {
          digitalWrite(redLeds[i],LOW);
        }
      }
       
      // read state of button
      int b = digitalRead(buttonPin);
      if (b != buttonState){
        buttonState = b;
        if (buttonState == 1){
          Serial.println("BUTTON PRESSED!");
          currentInstruction = 0;
           // set all LEDs to low
          for (int i = 0; i < inputs; i ++) {
             digitalWrite(redLeds[i],LOW); 
          }
          // read current program
          detectColors();
        }
      }
    
  } else { 
    int iSet = program[currentInstruction]; // what was in the place of the current instruction when the button was pushed
    int iNow = detectTileColor(currentInstruction);// read what is currently happing at the specified tile
       
    if (iSet != iNow) {
      currentInstruction = -1; //terminating as program changed
    } else {
        char action = instruction(iSet);
        if (action != NOOP) {
          digitalWrite(redLeds[currentInstruction],HIGH);
          if (action != FUNCTION){
              Serial.println(action);
              mySerial.println(action);
              delay(1000);
          } else {
            delay(200);
          }
          digitalWrite(redLeds[currentInstruction],LOW);
        }
        currentInstruction ++;
                
        if (currentInstruction == 12) { // end of program - terminate
          currentInstruction = -1;
        } else if (action == FUNCTION) {
           Serial.println("FUNCTION CALL");
           dispatchPoint = currentInstruction;
           currentInstruction = 12; 
        }   else if (currentInstruction > 15) { //end of function
          currentInstruction = dispatchPoint;
          dispatchPoint = -1;
        }      
     }
  }
}






void readLDRs(int colorIndx){ //color indx 0 = background, 1=red, 2 = green, 3 = blue
    for (int i = 0; i < 16; i++) {
      int input = ldrs[i];
      light[i][colorIndx] = analogRead(input);
    }  
}

  
void readTileColors(){
  // reads the intensity of light reflected for each spot where a tile is placed and stashes values in light array
  readLDRs(0);
  for (int c = 0; c < 3; c ++) { 
      int color = colors[c];
      colorOn(color);     
      delay(up_d);
      readLDRs(c+1);
      off();
      delay(down_d);
  }
}


void detectColors(){
   readTileColors();
   int rgb[16][3]; // stores normalised rgb values for each tile slot
   for (int tile=0; tile < 16; tile ++){
     int matchColor = -1;
     if (light[tile][0] < dark){ 
       for (int i = 0; i < 3; i++){
          rgb[tile][i] = light[tile][i+1] - average_brightness[tile][i]; // +1 as first is background
       }     
       int closest = INT_MAX;
       for (int tileColor = 0; tileColor < 4; tileColor ++) {
          unsigned int d = pow(rgb[tile][0] - centers[tileColor][0],2) 
                     + pow(rgb[tile][1] - centers[tileColor][1],2)
                     + pow(rgb[tile][2] - centers[tileColor][2],2);
          if (d < closest){
             closest = d;
             matchColor = tileColor; 
          }
       }
     }
     program[tile] = matchColor; 
   } 
}

int detectTileColor(int tile){
  // reads into global rgb variable  
  int background = analogRead(ldrs[tile]);
  if (background > dark){
    return -1; 
  } 
  int rgb[3];
  for (int c = 0; c< 3; c++) {
     int color = colors[c];
     colorOnOne(color,tile);
     delay(up_d);
     rgb[c] = analogRead(ldrs[tile])- average_brightness[tile][c];
     off();
     delay(down_d); 
  }
  // now find the closest match for the reflection response
  int matchColor = -1;     
  int closest = INT_MAX;
  for (int tileColor = 0; tileColor < 4; tileColor ++) {
    unsigned int d = pow(rgb[0] - centers[tileColor][0],2) 
                     + pow(rgb[1] - centers[tileColor][1],2)
                     + pow(rgb[2] - centers[tileColor][2],2);    
    if (d < closest){
       closest = d;
       matchColor = tileColor; 
    }
  }
  return matchColor;  
}

void colorOnOne(int color, int tile){
    digitalWrite(red,LOW);
    digitalWrite(green,LOW);
    digitalWrite(blue,LOW);
    digitalWrite(color,HIGH);
    digitalWrite(leds[tile],HIGH);
}

void colorOn(int color){
    digitalWrite(red,LOW);
    digitalWrite(green,LOW);
    digitalWrite(blue,LOW);
    digitalWrite(color,HIGH);
    for (int i = 0; i < 16; i++) {
      if (light[i][0] < dark){ //if it is dark ie, there is a tile there...
        digitalWrite(leds[i],HIGH);
      }
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

#include <SoftwareSerial.h>
#include <limits.h>
#include<math.h>
#define inputs 16
#define repeats 10 // the number of times to measure each analog input - to reduce variance

SoftwareSerial mySerial(10, 11); // RX - connect to orange TX line from HC05, TX - connect to green recive line on HCO5

int centers[16][4][4] = {{{412,142,362},{615,90,368},{458,95,446},{647,100,533}},{{471,247,543},{651,182,541},{510,188,600},{673,191,662}},{{474,153,465},{647,116,462},{515,120,528},{676,120,598}},{{613,418,670},{734,329,665},{638,339,692},{751,339,723}},{{447,368,307},{519,342,296},{460,346,356},{536,346,409}},{{579,449,393},{663,421,388},{596,425,458},{683,426,526}},{{522,274,185},{714,224,176},{564,226,270},{737,233,365}},{{593,394,325},{768,358,323},{634,362,396},{789,369,479}},{{520,424,741},{724,302,736},{569,316,781},{749,317,822}},{{539,216,521},{706,160,519},{574,164,582},{728,167,652}},{{568,311,568},{678,266,564},{590,272,608},{700,272,657}},{{402,218,436},{517,166,431},{424,173,470},{542,173,517}},{{495,300,497},{674,216,490},{532,226,576},{696,231,653}},{{479,263,493},{661,182,486},{518,193,563},{684,195,635}},{{350,183,430},{515,111,424},{383,121,497},{542,122,576}},{{580,205,558},{655,186,552},{593,190,580},{671,189,602}}};


int red = 9;
int green = 8;
int blue = 7;
int leds[] = {36,40,44,48,47,43,39,35,34,38,42,46,49,37,41,45};
int ldrs[] = {A8,A2,A9,A1,A3,A11,A0,A10,A12,A4,A13,A5,A14,A6,A15,A7};
int redLeds[] =  {30,26,22,18,19,23,27,31,32,28,24,20,33,29,25,21};



int colors[3] = {red,green,blue};
int light[16][4]; // stores ldr values - background, red,green,blue
int program[16];
int rgb[3]; //temporary storage for the value of one tile

int up_d = 30; // time to leave led on before reading value
int down_d = 300; //time to turn off before reading next value
int dark = 20; // max value of ldr with no tile on top - this needs to be tile dependent ...


int buttonPin = 12;
int buttonState = 0;
int currentInstruction = -1;
int dispatchPoint = -1; // if we are in the function, used to store where we came from so we can return back there

char NOOP = 'N';
char FORWARD = 'F';
char LEFT = 'L';
char RIGHT = 'R';
char FUNCTION = 'C';

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


char instruction(int rawInput){
  if (rawInput < 0){
    return NOOP;
  } if (rawInput == 0) {
    return LEFT; 
  } if (rawInput == 2){
    return FUNCTION;
  } if (rawInput == 1) {
    return FORWARD;
  }
  return RIGHT;
}

void loop2() {
  mySerial.println(FORWARD);
  delay(500);
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



void detectColors(){
   readTileColors();
   for (int tile=0; tile < 16; tile ++){
     int matchColor = -1;
     if (light[tile][0] < dark){       
       int closest = INT_MAX;
       for (int tileColor = 0; tileColor < 4; tileColor ++) {
          unsigned int d = pow(light[tile][1]-centers[tile][tileColor][0],2)+ pow(light[tile][2]-centers[tile][tileColor][1],2) + pow(light[tile][3]-centers[tile][tileColor][2],2);
          if (d < closest){
             closest = d;
             matchColor = tileColor; 
          }
       }
     }
     program[tile] = matchColor; 
   } 
}


void readLDRs(int colorIndx){ //color indx 0 = background, 1=red, 2 = green, 3 = blue
    for (int i = 0; i < 16; i++) {
      int input = ldrs[i];
      light[i][colorIndx] = analogRead(input);
    }  
}

int readTileColor(int tile){
  int background = analogRead(ldrs[tile]);
  if (background > dark) {
    return -1;
  } else {
    
    
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
  // subtract background
  for (int j = 0; j < 16; j++){
    for (int c = 1; c < 4; c++){
      light[j][c] -= light[j][0]; 
    }
  }
}

int detectTileColor(int tile){
  // reads into global rgb variable  
  int background = analogRead(ldrs[tile]);
  if (background > dark){
    return -1; 
  } else {
    for (int c = 0; c< 3; c++) {
       int color = colors[c];
       colorOnOne(color,tile);
       delay(up_d);
       rgb[c] = analogRead(ldrs[tile])-background;
       off();
       delay(down_d); 
    }
    // now find the closest match for the reflection response
       int matchColor = -1;     
       int closest = INT_MAX;
       for (int tileColor = 0; tileColor < 4; tileColor ++) {
          unsigned int d = pow(light[tile][1]-centers[tile][tileColor][0],2)+ pow(light[tile][2]-centers[tile][tileColor][1],2) + pow(light[tile][3]-centers[tile][tileColor][2],2);
          if (d < closest){
             closest = d;
             matchColor = tileColor; 
          }
       }
     return matchColor; 
   }
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





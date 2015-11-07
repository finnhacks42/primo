#include <SoftwareSerial.h>
#include <limits.h>
#include<math.h>
#define inputs 16
#define repeats 10 // the number of times to measure each analog input - to reduce variance

SoftwareSerial mySerial(10, 11); // RX - connect to orange TX line from HC05, TX - connect to green recive line on HCO5

//int centers[16][4][3] = {{{705,687,451},{830,689,396},{709,706,399},{838,764,405}},{{547,557,368},{679,539,301},{553,594,316},{714,663,308}},{{618,591,353},{736,577,296},{620,604,297},{754,685,305}},{{507,614,372},{678,613,335},{507,645,334},{695,730,344}},{{643,535,235},{765,534,200},{646,557,200},{773,639,205}},{{640,578,319},{772,575,267},{648,614,273},{803,720,278}},{{530,351,87},{719,341,79},{530,359,74},{750,527,81}},{{594,435,189},{736,445,173},{593,451,171},{751,546,178}},{{692,343,407},{782,339,380},{692,367,375},{787,442,384}},{{604,256,304},{792,249,237},{614,280,241},{824,434,253}},{{686,460,622},{809,441,556},{688,515,561},{826,638,572}},{{648,437,568},{773,434,539},{652,487,539},{785,605,547}},{{752,568,243},{860,567,183},{750,588,188},{865,691,191}},{{530,544,312},{712,540,238},{539,586,241},{751,715,250}},{{568,489,219},{752,478,187},{574,520,186},{782,655,193}},{{551,533,260},{719,543,219},{560,572,222},{737,671,229}}};
int centers[16][4][3] = {{{383,326,123},{611,338,80},{389,358,78},{640,520,87}},{{403,474,192},{588,466,147},{415,523,155},{637,612,156}},{{451,452,139},{708,461,109},{457,513,114},{727,650,113}},{{643,690,461},{821,680,352},{647,714,373},{825,785,359}},{{566,389,498},{699,384,460},{568,439,457},{707,560,471}},{{592,372,459},{716,374,435},{594,430,432},{725,549,433}},{{470,160,261},{705,155,197},{478,189,205},{742,298,214}},{{578,314,383},{759,313,356},{577,327,354},{781,439,368}},{{519,737,356},{728,728,290},{522,757,296},{752,831,302}},{{522,508,198},{707,507,150},{531,553,152},{739,661,154}},{{567,563,307},{717,569,271},{568,599,278},{731,688,280}},{{455,485,271},{642,486,195},{461,516,206},{653,620,201}},{{480,460,241},{650,467,201},{481,486,200},{671,624,215}},{{486,465,234},{630,465,177},{491,514,181},{650,590,180}},{{357,419,165},{573,427,105},{360,489,124},{594,617,120}},{{615,597,200},{737,597,172},{616,609,172},{743,679,174}}};

int red = 9;
int green = 8;
int blue = 7;
//int leds[] = {22,24,26,34,38,40,36,47,42,44,48,49,46,33,29,31}; 
//int redLeds[] = {18,19,30,45,37,35,28,43,21,20,32,41,23,25,27,39};
//int ldrs[] = {A3,A4,A5,A6,A2,A1,A0,A7,A14,A15,A8,A9,A13,A12,A11,A10};

int leds[] = {36,40,44,48,47,43,39,35,34,38,42,46,49,37,41,45};
int ldrs[] = {A8,A2,A9,A1,A3,A11,A0,A10,A12,A4,A13,A5,A14,A6,A15,A7};
int redLeds[] =  {30,26,22,18,19,23,27,31,32,28,24,20,33,29,25,21};



int colors[3] = {red,green,blue};
int light[16][4]; // stores ldr values - background, red,green,blue
int program[16];
int rgb[3]; //temporary storage for the value of one tile

int up_d = 20; // time to leave led on before reading value
int down_d = 300; //time to turn off before reading next value
int dark = 20; // max value of ldr with no tile on top


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





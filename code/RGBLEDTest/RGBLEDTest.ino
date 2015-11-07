// This script tests the RGB leds. 
// If all is going well, they should come on in order for half a second each. The first time round, all in red, then all in green, then all in blue.
// If a single/few LEDs do not work, check their connectivity on the PCB with a multimeter.
// If any color does not work at all, check the correponding transistor. 

int indx=0;
int red = 9;
int green = 8;
int blue = 7;

int timeOn = 500;
int timeOff = 300;


int leds[] = {36,40,44,48,47,43,39,35,34,38,42,46,49,37,41,45};

int colors[] = {red,green,blue};
int cIndx=0;

void setup() {
  pinMode(red,OUTPUT);
  pinMode(blue,OUTPUT);
  pinMode(green,OUTPUT);
  
  for (int i = 0; i < 16; i++){
    pinMode(leds[i],OUTPUT);
  }
  
  digitalWrite(red,LOW);
  digitalWrite(blue,LOW);
  digitalWrite(green,HIGH);
}

void loop() {
  if (indx == 0){
    for (int c=0; c < 3; c++){
       digitalWrite(colors[c],LOW); 
    }
    digitalWrite(colors[cIndx],HIGH);
    cIndx ++;
    cIndx = cIndx%3;
  }
    
  int led = leds[indx];
  indx++;
  indx=indx % 16;
  digitalWrite(led,HIGH);
  delay(timeOn);
  digitalWrite(led,LOW);
  delay(timeOff);  
}


int indx=0;
int red = 2;
int green = 3;
int blue = 4;

int timeOn = 500;
int timeOff = 300;


int leds[] = {46,33,29,31,49,48,44,42,38,40,36,47,22,24,26,34};

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

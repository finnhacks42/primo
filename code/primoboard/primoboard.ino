#include <SoftwareSerial.h>

SoftwareSerial mySerial(10, 11); // RX - connect to orange TX line from HC05, TX - connect to green recive line on HCO5
#define inputs 16
#define repeats 10 // the number of times to measure each analog input - to reduce variance

int buttonPin = 28;
int medians[inputs];
int dist[repeats];
int leds[] = {30,32,34,36,38,40,42,44,31,37,33,35,39,41,43,45};
int buttonState = 0;
int currentInstruction = -1;
int dispatchPoint = -1; // if we are in the function, used to store where we came from so we can return back there

String NOOP = "N";
String FORWARD = "A";
String LEFT = "L";
String RIGHT = "R";
String FUNCTION = "F";

//31 33 35 37
//33,35,31,37

void setup()  
{
  Serial.begin(9600);
  pinMode(buttonPin,INPUT);
  for (int i = 0; i < inputs; i ++) {
    pinMode(leds[i],OUTPUT); 
  }
  delay(1000);
  mySerial.begin(9600);
}

void sort(int a[], int size) {
    for(int i=0; i<(size-1); i++) {
        for(int o=0; o<(size-(i+1)); o++) {
                if(a[o] > a[o+1]) {
                    int t = a[o];
                    a[o] = a[o+1];
                    a[o+1] = t;
                }
        }
    }
}

// reads in the analogValues
void readValues(int a[],int r[]) { 
  for (int i = 0; i < inputs; i++){
    for (int j = 0; j < repeats; j++) {
      int value = analogRead(i);
      r[j] = value;
    }
    sort(r,repeats);
    a[i] = r[repeats/2];
  }
}

// read a single analog value
int readValue(int i, int r[]){
   for (int j = 0; j < repeats; j++){
     int value = analogRead(i);
     r[j] = value;
   }
   sort(r,repeats);
   return r[repeats/2];
}

void transmitProgram(int medians[]){
  for (int i = 0; i < inputs; i ++){
    Serial.println(medians[i]);
  }
  Serial.println("---------------------------------------------------");
}

String instruction(int rawInput){
  if (rawInput > 1015){
    return NOOP;
  } if (rawInput > 725) {
    return LEFT; 
  } if (rawInput > 425){
    return FUNCTION;
  } if (rawInput > 180) {
    return FORWARD;
  }
  return RIGHT;
}

void loop() // run over and over
{
  if (currentInstruction < 0) {
    // read in from analog input ports and see what the resistance is, tells us which command has been placed
    readValues(medians,dist);
  
    for (int i = 0; i < inputs; i++){
      if (medians[i] < 1020) { // if a command has been placed light up the corresponding LED
       digitalWrite(leds[i],HIGH); 
      } else {
       digitalWrite(leds[i],LOW); 
      }
    }
  
    // read state of button
    int b = digitalRead(buttonPin);
    if (b != buttonState){
      buttonState = b;
      if (buttonState == 1){
        currentInstruction = 0;
         // set all LEDs to low
        for (int i = 0; i < inputs; i ++) {
           digitalWrite(leds[i],LOW); 
        }
      }
    }
  } else {
    // read the instruction at the current point 
    String action = instruction(readValue(currentInstruction,dist));
    String setAction = instruction(medians[currentInstruction]);
    if (action != setAction) {
      currentInstruction = -1; //terminating as program changed
    } else {
        if (action != NOOP) {
          digitalWrite(leds[currentInstruction],HIGH);
          if (action != FUNCTION){
              Serial.println(action);
              mySerial.println(action);
          }
          delay(1000);
          digitalWrite(leds[currentInstruction],LOW);
        }
        currentInstruction ++;
                
        if (currentInstruction == 12) { // end of program - terminate
          currentInstruction = -1;
        } else if (action == FUNCTION) {
           dispatchPoint = currentInstruction;
           currentInstruction = 12; 
        }   else if (currentInstruction > 15) { //end of function
          currentInstruction = dispatchPoint;
          dispatchPoint = -1;
        }      
     }
  }
}





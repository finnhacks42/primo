#include <SoftwareSerial.h>

SoftwareSerial mySerial(10, 11); // RX - connect to orange TX line from HC05, TX - connect to green recive line on HCO5
#define inputs 10
#define repeats 10 // the number of times to measure each analog input - to reduce variance

int data[inputs][repeats];
int leds[] = {30,32,34,36,38,40,42,44,31,33,35,37,39,41,43,45};


void setup()  
{
  for (int i = 0; i < inputs; i ++) {
   pinMode(leds[i],OUTPUT); 
  }
  Serial.begin(9600);
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
void readValues(int a[10][10]) { // [inputs][repeats]
  for (int i = 0; i < inputs; i++){
    for (int j = 0; j < repeats; j++) {
      int value = analogRead(i);
      a[i][j] = value;
      delay(2);
    }
  }
  for (int i = 0; i < inputs; i++) {
   sort(a[i],repeats); 
  }
}


void loop() // run over and over
{
  Serial.println("READING");
  readValues(data);
  // read in from analog input ports and see what the resistance is - ... this should tell us what command has been placed.
  for (int i = 0; i < inputs; i++){
    int median = data[i][repeats/2];// 4 is in the median of 0-9 (where 10 is the number of repeats)
    mySerial.print(median);
    Serial.println(median);
    if (median < 1020) {
     digitalWrite(leds[i],HIGH); 
    } else {
     digitalWrite(leds[i],LOW); 
    }
  }
  delay(10);
  mySerial.print("a");
  Serial.println("-----------------------");
 
  
  //mySerial.print("0");
  //mySerial.write((byte)0);
  //delay(1000);
  //mySerial.print("1");
  //mySerial.write((byte)1);
  //delay(1000);

  //mySerial.print("AAAAAAAAAAAAAA");

  //delay(1000);
  //mySerial.print("BB");
}



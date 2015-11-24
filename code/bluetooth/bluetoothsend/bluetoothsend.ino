#include <SoftwareSerial.h>

SoftwareSerial mySerial(10, 11); // RX, TX

int counter=0;

void setup()  
{
  Serial.begin(9600);
  delay(1000);
  mySerial.begin(9600);
}

void loop() // run over and over
{
  counter ++;
  //mySerial.print("0");
  //mySerial.write((byte)0);
  //delay(1000);
  //mySerial.print("1");
  //mySerial.write((byte)1);
  delay(1000);

  mySerial.print("AAAAAAAAAAAAAA");

  delay(1000);
  mySerial.print("BB");



}



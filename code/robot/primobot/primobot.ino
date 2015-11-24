#include <SoftwareSerial.h>

// The circuit: 
// * RX is digital pin 10 (connect to TX (orange) on bluetooth)
// * TX is digital pin 11 (connect to RX (green) on bluetooth)

SoftwareSerial mySerial(10, 11); // RX, TX

int counter=0;
char INBYTE;

void setup()  
{
  // Open serial communications and wait for port to open:
  Serial.begin(9600);
  delay(1000);
  mySerial.begin(9600);
}

void loop() // run over and over
{
  while(!mySerial.available()){}
  Serial.write(mySerial.read());
  //if (mySerial.available()){
  //  Serial.write(mySerial.read());
  //}
  
  //while (!mySerial.available());   // stay here so long as COM port is empty   
  //INBYTE = mySerial.read();        // read next available byte
  //if( INBYTE == '0' ) {
  //  Serial.println("OFF");
  //  digitalWrite(LED, LOW);  // if it's a 0 (zero) tun LED off
  //}
  //if( INBYTE == '1' ) {
  //  Serial.println("ON");
  //  digitalWrite(LED, HIGH); // if it's a 1 (one) turn LED on
  //} else {
  //  Serial.print("UKN:");
  //  Serial.print(INBYTE,DEC);
  //}

  //Serial.println(INBYTE,DEC);

}



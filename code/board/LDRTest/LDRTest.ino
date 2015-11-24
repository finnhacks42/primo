//Tests all the light dependent resistors
//Begin by filling the board with tiles (no particular order)
//The script will read each light dependent resistor in turn, first with the corresponding rgb led off,
//then with it on, and output the results to the console.
//After all 16 have been read, it prints a line of dashes and then restarts,
//You should see a pair of readings for each ldr. The first number should be < 10, the second > 150.



int red = 9;


int leds[] = {36,40,44,48,47,43,39,35,34,38,42,46,49,37,41,45};
int ldrs[] = {A8,A2,A9,A1,A3,A11,A0,A10,A12,A4,A13,A5,A14,A6,A15,A7};
int redLeds[] =  {30,26,22,18,19,23,27,31,32,28,24,20,33,29,25,21};

int up_d = 20; //time to leave rgb led on before reading value of ldr.
int down_d = 300;
int wait_time = 500;

void setup(){
   Serial.begin(9600); 
   pinMode(red, OUTPUT);
   digitalWrite(red,HIGH);
   for (int i = 0; i < 16; i++){
      pinMode(leds[i],OUTPUT);
      digitalWrite(leds[i],LOW);
      pinMode(ldrs[i],INPUT);
   }
   delay(down_d); 
}

int i = 0;
void loop(){
  // first turn on the LED for the LDR we are about to read
  
      digitalWrite(redLeds[i],HIGH);
      delay(wait_time);
      int off_value = analogRead(ldrs[i]); // read the ldr with the rgb led off.
      digitalWrite(leds[i],HIGH); //turn on the rgb led in red
      delay(up_d);
      int on_value = analogRead(ldrs[i]);
      Serial.print(off_value);
      Serial.print(",");
      Serial.println(on_value);
      digitalWrite(leds[i],LOW); //turn off the rgb led
      digitalWrite(redLeds[i],LOW); //turn off the indicator led
      delay(wait_time);
      
      i++;
      i = i%16;
      if (i ==0) {
        Serial.println("--------------------------------------------------------");
        delay(1000);
      }
  }


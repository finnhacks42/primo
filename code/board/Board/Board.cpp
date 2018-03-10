#include "Arduino.h"
#include "Board.h"

Board::Board()
{
	red = 9;
	green = 8;
	blue = 7; 
	pinMode(red,OUTPUT);
	pinMode(green,OUTPUT);
	pinMode(blue,OUTPUT);	
}

void Board::off()
{
    digitalWrite(red,LOW);
    digitalWrite(green,LOW);
    digitalWrite(blue,LOW);	

}

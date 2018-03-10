/* Board.h - Library for reading data from optical programming board 
   Created by Finn Lattimore, Nov 2015
   Released into the public domain
*/
#ifndef Board_h
#define Board_h
#include "Arduino.h"
class Board {
	public:
		Board();
		void off();	
		int red;
		int green;
		int blue;
}

#endif

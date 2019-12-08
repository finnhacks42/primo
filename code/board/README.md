# Board 

## Assembly

## Test & Configure

   1. Run `indicatorLEDTest_PCB` and check each red indicator LED flashes in turn. 

   2. Place a tile in each slot in the board (doesn't matter what colour). Run `LDRTest` and open up the serial monitor (at 9600 baud). You should see something like:

      ```  python
      0,610
      0,758
      0,637
      0,554
      0,580
      0,536
      0,786
      0,708
      0,609
      0,549
      0,510
      0,556
      0,722
      1,731
      0,771
      0,735
      ```
      These numbers are the values of each LDR with the corresponding RGB-LED on/off respectively. The first number should be < 10 and the second > 150.

   3. TODO Install python package 

   4. Load the `CalibrateColours` sketch onto the board and run `python readdata.py` in the terminal and follow the instructions printed to the terminal. 

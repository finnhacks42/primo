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

   3. Load the `CalibrateColours` sketch onto the board and run `python read.py` in the terminal. You should see `PLEASE SET BOARD TO CONFIGURATION: Red|Green|Blue|Yellow. Press any key to continue` in the terminal. This means set the board up like this:

   4. When the terminal reports `Shuffle. Press any key to continue`, move the tiles around so that each tile is in a new spot - but within the same column (ie which colour is in which column is unchanged). 

   5. Continue for each board configuration. This generates the data from which to compute what LDR responses correspond to each colour of tile.  When all is complete, you should get an output like this;

      ```
      int average_brightness[16][3] = {
         {507,588,256},
         {684,687,365},
         {709,643,295},
         {652,670,342},
         {694,410,420},
         {625,439,460},
         {696,508,496},
         {590,361,339},
         {710,606,282},
         {651,661,333},
         {627,663,366},
         {660,632,309},
         {615,520,90},
         {650,661,304},
         {715,699,371},
         {681,559,277},
      };
      
      int centers[4][3] = {
         {-75,14,114},
         {-91,-31,-40},
         {72,-62,-43},
         {94,78,-31},
      };
      ```

   6. Copy and past these values into the `RobotControl` Arduino script


KinderBot
=====

Contains design files and code for my version of the Primo robotic platform for young kids www.primo.io licenced under CC by-nc-sa 4.0 International
## Components

### Board
   - laser cut xxx and yyy in 3mm ply or bamboo
   - laser cut zzz in 1.5mm clear acrylic
   - 3d print 4 off zzz
   - 1 colorsense PCB
   - Arduino mega
   - HC05 serial bluetooth chip
   - 16 rgb leds
   - 16 red leds

### Robot
   - Kinder surprise egg
   - laser cut zzz of 3mm ply
   - Arduino pro micro 
   - 2 gear motors 3-6V with tyres
   - HC05 or HC06 serial bluetooth chip
   - H-Bridge IC (sn75441one)
   - 1.1K resistor
   - 1.2K resistor
   - 2 12pin female header
   - 2 2pin male header
   - Battery holder for 4xAA
   
## Assembly

### Board

### Robot

## Programming & Calibration

### Board

### Robot

## Setting up Bluetooth

We now need to set up the two bluetooth chips to automatically pair with one another when they are turned on. We will configure the chip in the board as the master and the chip in the robot as the slave. To set up the chips we first need to put them in AT mode. Using the pro-micro for your bot, construct the circuit below on a breadboard. 

Note that different manufacturers of the HC-05 breakout boards have used slightly different variants of pin orderings. On my board they `key` pin is not broken out. To temporally pull it high while programming the chip in AT mode I have just wedged a jumper wire under the protective plastic coating (see photo). Alternatively you can solder a wire directly to the pad on the chip. 



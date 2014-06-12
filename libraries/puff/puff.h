/*
  Puff.h - Library for arduino 8-wire manifold air ride systems.
  Script also includes functions for leveling with rotary position sensors.
  Created by Aaron R. Meeuwsen, November 28, 2013.
  Released into the public domain.
*/

/*
 "Bag" Array:
 [x][0] = valveUp
 [x][1] = valveDown
 [x][2] = leveler
 [x][3] = set level
 [x][4] = manual buton
 
 Manual Buttons
 bag[0][4] = X
 bag[1][4] = Y
 bag[2][4] = Select
*/

#include "Arduino.h"
#include "EEPROM.h"

#ifndef Puff_h
#define Puff_h

//Multidimensional array for all bag variables
extern int bag[4][5];
extern byte printSerial;

class Puff {
public:
    Puff(byte _button,
         byte _buttonLed,
         byte _valveUp1, 
		 byte _valveDown1,
		 byte _valveUp2,
         byte _valveDown2,
         byte _valveUp3,
         byte _valveDown3,
         byte _valveUp4,
         byte _valveDown4,
         byte _leveler1,
         byte _leveler2,
         byte _leveler3,
         byte _leveler4,
         byte _x,
         byte _y,
         byte _sel);
    byte button;
    byte buttonLed;
    
    //Initial setup function
    void setup();
    
    //Main function to call puff program
    void go();
    
    //Master button functions and variables
    byte activate; //Has the button been pushed?
    byte pushes; //How many times has the button been pushed?
    byte mbs;
	byte mbs2;
	byte mbs3;
    int adjustTimeout;
    int adjustTimer;
    int timeoutAmount;
	unsigned long time;
	unsigned long nextUpdate;
	void readyBlink(byte led, byte blinks, byte pause);
	void updateTimer();
    void activated();
    void getPushes();
    void executeHeight();
    
    //Valves and functions
    void killAll(); //Kills all valves
    void adjustBags(); //Function to keep valves cycling until all levels are met
   	void adjust(); //Raise or lower depending on current height

    
    //Leveler functions and variables
    void updateLevels(); //Get value of all levelers
    byte levelers[4];
    
    //Height functions and variables
    byte current; //Variable to store height on new selection
	void updateHeights(); //Grabs heights from EEPROM memory
    
    //Manual buttons
    void joystickAdjust();
    byte x; //Joystick X axis
    byte y; //Joystick Y axis
    byte sel; //Joystick select pin
	byte upDown; //Manual adjustment direction up or down
	byte dAmount; //Delay amount between each valve cycle
    
    //Helper functions for puff
    void cleanUp();
    
private:
};
#endif
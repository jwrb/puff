/*
  easyAir.h - Library for arduino 8-wire manifold air ride systems.
  Script also includes functions for leveling with rotary position sensors.
  Created by Aaron R. Meeuwsen, November 28, 2013.
  Released into the public domain.
 
 
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

#ifndef easyAir_h
#define easyAir_h

//Multidimensional array for all bag variables
extern int bag[4][5];

class easyAir {
public:
	virtual byte test();
};

class Levels : public easyAir {
public:
	void updateLevels();
	Levels(int _leveler1,
		   int _leveler2,
		   int _leveler3,
		   int _leveler4);
private:
	byte levelers[4];
};

class Buttons : public easyAir {
public:
	Buttons(byte _x, byte _y, byte select);
	void manualAdjust();
	byte upDown;
	byte dAmount;
};

class Heights : public easyAir {
public:
	byte current;
	void updateHeights();
};

class Helper : public easyAir {
public:
	byte activate;
	byte mbs;
	byte mbs2;
	byte mbs3;
	int timeoutAmount;
	unsigned long time;
	unsigned long nextUpdate;
	void readyBlink(byte led, byte blinks, byte pause);
	
	void updateTimer();
};

class Valves : public easyAir {
public:
	void killAll();
	void adjust();
	Valves(byte _valveUp1, 
		   byte _valveDown1, 
		   byte _valveUp2, 
		   byte _valveDown2, 
		   byte _valveUp3, 
		   byte _valveDown3, 
		   byte _valveUp4, 
		   byte _valveDown4);
};

#endif
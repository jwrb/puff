/*
 easyAir.cpp - Library for outputting to 8-wire manifolds.
 Created by Aaron R. Meeuwsen, November 28, 2013.
 Released into the public domain.
 */

#include "Arduino.h"
#include "easyAir.h"

//Multidimensional array for all bag variables
int bag[4][5];

byte easyAir::test()
{
	byte dummy = digitalRead(1);
}

Levels::Levels(int _leveler1,
			   int _leveler2,
			   int _leveler3,
			   int _leveler4){
	pinMode(_leveler1, INPUT);
	pinMode(_leveler2, INPUT);
	pinMode(_leveler3, INPUT);
	pinMode(_leveler4, INPUT);
	levelers[0] = _leveler1;
	levelers[1] = _leveler2;
	levelers[2] = _leveler3;
	levelers[3] = _leveler4;
}

void Levels::updateLevels()
{
	//Variables to do level adjustment calculations
	int levelerRead[4];
	
	for (int i = 0; i < 4; i++) { 
		levelerRead[i] = analogRead(levelers[i]);
		levelerRead[i] = map(levelerRead[i], 0, 1023, 0, 60);
		bag[i][2] = levelerRead[i];
	}
}

Buttons::Buttons(byte _x, 
				   byte _y, 
				   byte _select){
	pinMode(_x, INPUT);
	pinMode(_y, INPUT);
	pinMode(_select, INPUT);
	bag[0][4] = _x;
	bag[1][4] = _y;
	bag[2][4] = _select;
}

void Buttons::manualAdjust()
{
	int xState = analogRead(bag[0][4]);
	int yState = analogRead(bag[1][4]);
	byte selectState = digitalRead(bag[2][4]);
	
	dAmount = 50;
	
	if (selectState == LOW) {
		if (upDown == 0) {
			upDown = 1;
		}
		else if (upDown == 1) {
			upDown = 0;
		}
		delay(200);
	}
	
	if (upDown == 0) {
		if (yState > 1022 && xState < 600 && xState > 400) {
			digitalWrite(bag[0][0], HIGH);
			digitalWrite(bag[1][0], HIGH);
			delay(dAmount);
			digitalWrite(bag[0][0], LOW);
			digitalWrite(bag[1][0], LOW);
			delay(dAmount);
		}
		else if (yState > 1022 && xState < 1) {
			digitalWrite(bag[0][0], HIGH);
			delay(dAmount);
			digitalWrite(bag[0][0], LOW);
			delay(dAmount);
		}
		else if (yState > 1022 && xState > 1022) {
			digitalWrite(bag[1][0], HIGH);
			delay(dAmount);
			digitalWrite(bag[1][0], LOW);
			delay(dAmount);
		}
		else {
			digitalWrite(bag[0][0], LOW);
			digitalWrite(bag[1][0], LOW);
		}
	}
	if (upDown == 1) {
		if (yState > 1022 && xState < 600 && xState > 400) {
			digitalWrite(bag[0][1], HIGH);
			digitalWrite(bag[1][1], HIGH);
			delay(dAmount);
			digitalWrite(bag[0][1], LOW);
			digitalWrite(bag[1][1], LOW);
			delay(dAmount);
		}
		else if (yState > 1022 && xState < 1) {
			digitalWrite(bag[0][1], HIGH);
			delay(dAmount);
			digitalWrite(bag[0][1], LOW);
			delay(dAmount);
		}
		else if (yState > 1022 && xState > 1022) {
			digitalWrite(bag[1][1], HIGH);
			delay(dAmount);
			digitalWrite(bag[1][1], LOW);
			delay(dAmount);
		}
		else {
			digitalWrite(bag[0][1], LOW);
			digitalWrite(bag[1][1], LOW);
		}
	}
}

void Heights::updateHeights()
{	
	if (current == 1) {
		bag[0][3] = EEPROM.read(101);
		bag[1][3] = EEPROM.read(102);
		bag[2][3] = EEPROM.read(103);
		bag[3][3] = EEPROM.read(104);
	}
	
	if (current == 2) {
		bag[0][3] = EEPROM.read(201);
		bag[1][3] = EEPROM.read(202);
		bag[2][3] = EEPROM.read(203);
		bag[3][3] = EEPROM.read(204);
	}
	
	if (current == 3)
	{
		bag[0][3] = EEPROM.read(301);
		bag[1][3] = EEPROM.read(302);
		bag[2][3] = EEPROM.read(303);
		bag[3][3] = EEPROM.read(304);
	}
	
	if (current >= 8 && current <= 20)
	{
		bag[0][3] = 0;
		bag[1][3] = 0;
		bag[2][3] = 0;
		bag[3][3] = 0;
	}
}

void Helper::readyBlink(byte led, byte blinks, byte pause)
{
	if (blinks < 4) {
		for (byte i = 1; i <= blinks; i++) {
			digitalWrite(led, LOW);
			delay(pause);
			digitalWrite(led, HIGH);
			delay(pause);
		}
	}
}

void Helper::updateTimer()
{
	timeoutAmount = 1250;
	time = millis();
	nextUpdate = time + timeoutAmount;
}

Valves::Valves(byte _valveUp1, 
			   byte _valveDown1, 
			   byte _valveUp2, 
			   byte _valveDown2, 
			   byte _valveUp3, 
			   byte _valveDown3, 
			   byte _valveUp4, 
			   byte _valveDown4){
	pinMode(_valveUp1, OUTPUT);
	pinMode(_valveDown1, OUTPUT);
	pinMode(_valveUp2, OUTPUT);
	pinMode(_valveDown2, OUTPUT);
	pinMode(_valveUp3, OUTPUT);
	pinMode(_valveDown3, OUTPUT);
	pinMode(_valveUp4, OUTPUT);
	pinMode(_valveDown4, OUTPUT);
	bag[0][0] = _valveUp1;
	bag[1][0] = _valveUp2;
	bag[2][0] = _valveUp3;
	bag[3][0] = _valveUp4;
	bag[0][1] = _valveDown1;
	bag[1][1] = _valveDown2;
	bag[2][1] = _valveDown3;
	bag[3][1] = _valveDown4;
}

void Valves::killAll()
{
	for (int i = 0; i < 4; i++) {
		digitalWrite(bag[i][0], LOW);
		digitalWrite(bag[i][1], LOW);
	}
}

void Valves::adjust()
{
		for (byte i; i < 2; i++) {
			if (bag[i][2] < bag[i][3]) {
				digitalWrite(bag[i][0], HIGH);
				digitalWrite(bag[i][1], LOW);
			}
			else if (bag [i][2] > bag[i][3]) {
				digitalWrite(bag[i][1], HIGH);
				digitalWrite(bag[i][0], LOW);
			}
			else {
				digitalWrite(bag[i][0], LOW);
				digitalWrite(bag[i][1], LOW);
			}
		}
}













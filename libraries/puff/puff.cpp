/*
 Puff.cpp - Library for outputting to 8-wire manifolds.
 Created by Aaron R. Meeuwsen, November 28, 2013.
 Released into the public domain.
 */

#include "Arduino.h"
#include "puff.h"

//Multidimensional array for all bag variables
int bag[4][5];
byte printSerial;

///////////////////////////////////////////////////
//Function to put all pins into their proper arrays
///////////////////////////////////////////////////
Puff::Puff(byte _button,
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
           byte _sel) {
    pinMode(_button, INPUT);
    pinMode(_buttonLed, OUTPUT);
    pinMode(_valveUp1, OUTPUT);
	pinMode(_valveDown1, OUTPUT);
	pinMode(_valveUp2, OUTPUT);
	pinMode(_valveDown2, OUTPUT);
	pinMode(_valveUp3, OUTPUT);
	pinMode(_valveDown3, OUTPUT);
	pinMode(_valveUp4, OUTPUT);
	pinMode(_valveDown4, OUTPUT);
    pinMode(_x, INPUT);
	pinMode(_y, INPUT);
	pinMode(_sel, INPUT);
    //Set pins in proper array variables
    button = _button;
    buttonLed = _buttonLed;
    bag[0][0] = _valveUp1;
	bag[1][0] = _valveUp2;
	bag[2][0] = _valveUp3;
	bag[3][0] = _valveUp4;
	bag[0][1] = _valveDown1;
	bag[1][1] = _valveDown2;
	bag[2][1] = _valveDown3;
	bag[3][1] = _valveDown4;
    levelers[0] = _leveler1;
	levelers[1] = _leveler2;
	levelers[2] = _leveler3;
	levelers[3] = _leveler4;
	x = _x;
	y = _y;
	sel = _sel;
}

void Puff::setup()
{
    if (printSerial == 1) {
        Serial.begin(9600);
    }

    //Kill all valves on startup and read current levels
    Puff::killAll();
    Puff::updateLevels();
  
    //Sets the manual adjustment direction to up. 1 for down.
    upDown = 0;

    //Turns on LED indicating system ready
    digitalWrite(buttonLed, HIGH);
}

void Puff::go()
{
    Puff::activated();
    Puff::getPushes();
    Puff::executeHeight();
    Puff::joystickAdjust();
    if (activate == 1 && pushes == 0)
    {
      if (printSerial == 1) {
        Serial.println("No height selected");
      }
      Puff::cleanUp();
    }
  
    if (activate == 2)
    {
      if (printSerial == 1) {
        Serial.println("Programming mode entered...");
      }
      Puff::cleanUp();
    }
}

//Check if button is pushed. If yes activate system
void Puff::activated()
{
    mbs = digitalRead(button);
    if (mbs == HIGH)
    {
        activate = 1;
        readyBlink(buttonLed, 1, 100);
        delay(100);
    }
}

//Get number of pushes after initial activation
void Puff::getPushes()
{
  if (activate == 1)
  {
    if (printSerial == 1) {
        Serial.println("Please select a height");
    }
    mbs = LOW;
    updateTimer();
    while(time < nextUpdate)
    {
      time = millis();
      mbs2 = digitalRead(button);
      if (mbs2 == HIGH)
      {
        pushes = pushes++;
        if (pushes < 5)
        {
          nextUpdate = nextUpdate + 100;
        }
        digitalWrite(mbs2, LOW);
        delay(150);
      }
    }
  }
}

void Puff::executeHeight()
{
  if (activate == 1 && pushes > 0)
  {
    readyBlink(buttonLed, pushes, 150);
    current = pushes;
    updateHeights();
    adjustBags();
    cleanUp();
  }
}

/////////////////////////////
//Function to kill all valves
/////////////////////////////
void Puff::killAll()
{
	for (int i = 0; i < 4; i++) {
		digitalWrite(bag[i][0], LOW);
		digitalWrite(bag[i][1], LOW);
	}
}

/////////////////////////////////////
//Keeps adjusting bags until all are
//at correct level or until timeout
/////////////////////////////////////
void Puff::adjustBags()
{
  mbs2 = LOW;
  digitalWrite(buttonLed, LOW);
  updateLevels();
  adjustTimeout = millis() + 7000;
  adjustTimer = millis();
  while (mbs3 != HIGH && adjustTimer < adjustTimeout)
  {
    mbs3 = digitalRead(button);
    adjust();
    updateLevels();
    adjustTimer = millis();
    if (bag[0][2] == bag[0][3] && bag[1][2] == bag[0][3]) {
        mbs3 = HIGH;
    }
  }
}

///////////////////////////////////////
//Function to adjust bags against level
///////////////////////////////////////
void Puff::adjust()
{
/*
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
*/

        if (bag[0][2] < bag[0][3]) {
            digitalWrite(bag[0][0], HIGH);
            digitalWrite(bag[0][1], LOW);
        }
        else if (bag[0][2] > bag[0][3]) {
            digitalWrite(bag[0][1], HIGH);
            digitalWrite(bag[0][0], LOW);
        }
        else {
            digitalWrite(bag[0][0], LOW);
            digitalWrite(bag[0][1], LOW);
        }
    
        if (bag[1][2] < bag[1][3]) {
            digitalWrite(bag[1][0], HIGH);
            digitalWrite(bag[1][1], LOW);
        }
        else if (bag[1][2] > bag[1][3]) {
            digitalWrite(bag[1][1], HIGH);
            digitalWrite(bag[1][0], LOW);
        }
        else {
            digitalWrite(bag[1][0], LOW);
            digitalWrite(bag[1][1], LOW);
        }

}

//////////////////////////////////////////////
//Function to udpate all of the current levels
//////////////////////////////////////////////
void Puff::updateLevels()
{
	//Define temporary variables to do level adjustment calculations
	int levelerRead[4];
	
	for (int i = 0; i < 2; i++) {
		levelerRead[i] = analogRead(levelers[i]);
		levelerRead[i] = map(levelerRead[i], 0, 1023, 0, 60);
		bag[i][2] = levelerRead[i];
	}
}

////////////////////////////////////////////
//Function to pull saved heights from EEPROM
////////////////////////////////////////////
void Puff::updateHeights()
{
    //Saved memory levels for height 1
	if (current == 1) {
		bag[0][3] = EEPROM.read(101);
		bag[1][3] = EEPROM.read(102);
		bag[2][3] = EEPROM.read(103);
		bag[3][3] = EEPROM.read(104);
	}
	
    //Saved memory levels for height 2
	if (current == 2) {
		bag[0][3] = EEPROM.read(201);
		bag[1][3] = EEPROM.read(202);
		bag[2][3] = EEPROM.read(203);
		bag[3][3] = EEPROM.read(204);
	}
	
    //Saved memory levels for height 3
	if (current == 3)
	{
		bag[0][3] = EEPROM.read(301);
		bag[1][3] = EEPROM.read(302);
		bag[2][3] = EEPROM.read(303);
		bag[3][3] = EEPROM.read(304);
	}
	
    //Saved memory levels for all down
	if (current >= 8 && current <= 20)
	{
		bag[0][3] = 0;
		bag[1][3] = 0;
		bag[2][3] = 0;
		bag[3][3] = 0;
	}
}

/////////////////////////////////////////////////////////////
//Function to read joystick analog pins for manual adjustment
/////////////////////////////////////////////////////////////
void Puff::joystickAdjust()
{
	int xState = analogRead(x);
	int yState = analogRead(y);
	byte selectState = digitalRead(sel);
	
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

/////////////////////////////////////////////////////
//Function to clean up and reset variables after puff
//height adjustment
/////////////////////////////////////////////////////
void Puff::cleanUp()
{
    Puff::killAll();
    activate = 0;
    pushes = 0;
    mbs2 = LOW;
    mbs3 = LOW;
    digitalWrite(buttonLed, HIGH);
    delay(300);
}

void Puff::readyBlink(byte led, byte blinks, byte pause)
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

void Puff::updateTimer()
{
	timeoutAmount = 1250;
	time = millis();
	nextUpdate = time + timeoutAmount;
}













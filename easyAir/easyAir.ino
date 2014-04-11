#include "easyAir.h"
#include <EEPROM.h>

//Define master button input pin and led if applicable
const byte masterButton = 7;
const byte masterButtonLed = 6;

//Define valve pins (valveUp1, valveDown1, valveUp2, valveDown2.....)
Valves valves(13,11,12,10,0,0,0,0);

//Define leveler pins (leveler1, leveler2.....)
Levels levels(A0,A1,0,0);

//Define manual button pins (X axis, Y axis, Select Button)
Buttons buttons(A4,A3,6);

//Other library initializations and variables (no need to edit)
easyAir easyAir;
Heights height;
Helper helper;

byte activate = 0;
byte pushes = 0;

void setup() {
  Serial.begin(9600);
  pinMode(masterButton, INPUT);
  pinMode(masterButtonLed, OUTPUT);
  digitalWrite(masterButtonLed, HIGH);
  valves.killAll();
  levels.updateLevels();
  digitalWrite(bag[2][4], HIGH);
  //Sets the manual adjustment direction to up. 1 for down.
  buttons.upDown = 0;  
  Serial.print("Ready for command...");
}

void loop() {
  helper.mbs = digitalRead(masterButton);
  if (helper.mbs == HIGH)
  {
    helper.activate = 1;
    delay(150);
    helper.readyBlink(masterButtonLed, 1, 100);
  }
  
  if (helper.activate == 1)
  {
    Serial.println("Please select height");
    helper.mbs = LOW;
    helper.updateTimer();
    while(helper.time < helper.nextUpdate)
    {
      helper.time = millis();
      helper.mbs2 = digitalRead(masterButton);
      if (helper.mbs2 == HIGH)
      {
        pushes = pushes++;
        if (pushes < 5)
        {
          helper.nextUpdate = helper.nextUpdate + 500;
        }
        digitalWrite(helper.mbs2, LOW);
        delay(150);
      }
    }
  }
  if (helper.activate == 1 && pushes > 0)
  {
    Serial.println("Adjusting bags");
    helper.readyBlink(masterButtonLed, pushes, 150);
    height.current = pushes;
    height.updateHeights();
    levels.updateLevels();
    printInfo();
    adjustBags();
    cleanUp();
  }
  
  if (helper.activate == 1 && pushes == 0)
  {
    Serial.println("No height selected");
    cleanUp();
  }
  
  if (helper.activate ==2)
  {
    Serial.println("Programming mode entered...");
    cleanUp();
  }
  buttons.manualAdjust();
}

void adjustBags()
{
  helper.mbs2 = LOW;
  digitalWrite(masterButtonLed, LOW);
  levels.updateLevels();
  while (helper.mbs3 != HIGH)
  {
    helper.mbs3 = digitalRead(masterButton);
    valves.adjust();
    levels.updateLevels();
    
    if (bag[0][2] == bag[0][3] && bag[1][2] == bag[1][3])
    {
    Serial.println("Bags adjusted");
    Serial.println("");
    helper.mbs3 = HIGH;
    }
  }
}

void cleanUp()
{
  valves.killAll();
  helper.activate = 0;
  pushes = 0;
  helper.mbs2 = LOW;
  helper.mbs3 = LOW;
  Serial.print("Ready for command...");
  digitalWrite(masterButtonLed, HIGH);
  delay(300);
}

void printInfo()
{
  if (pushes > 8 && pushes < 20)
  {
    Serial.println("All down selected");
  } else {
    Serial.print("Height ");
    Serial.print(pushes);
    Serial.println(" selected");
  }
  Serial.println("----------------");
  Serial.print("Bag 1 set level: ");
  Serial.println(bag[0][3]);
  Serial.print("Bag 2 set level: ");
  Serial.println(bag[1][3]);
  Serial.print("Bag 3 set level: ");
  Serial.println(bag[2][3]);
  Serial.print("Bag 4 set level: ");
  Serial.println(bag[3][3]);
  Serial.println("----------------");
}




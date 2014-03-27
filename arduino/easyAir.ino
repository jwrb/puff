#include <EEPROM.h>

byte dummyLowPin = 4;

//Declare constant EEPROM's for saving ride heights
const int EEPROMcurrentRideHeight = 512; //EEPROM byte 512 stores current ride height
const int EEPROMhighRideHeight = 511;
const int EEPROMnormRideHeight = 510;
const int EEPROMlowRideHeight = 509;

//Declare airbag variables
byte bag1 = 7;
byte bag2 = 8;
byte bag3 = 9;
byte bag4 = 10;

//Declare manual buttons
const byte bag1Button = 13;
const byte bag2Button = 12;
const byte bag3Button = 11;
const byte bag4Button = 3;
const byte manualSwitch = 2;

//Declare manual button states
int bag1ButtonState;
int bag2ButtonState;
int bag3ButtonState;
int bag4ButtonState;
int manualSwitchState;

//Declare ride heights
int currentRideHeight;
int highRideHeight = EEPROM.read(EEPROMhighRideHeight);
int normRideHeight = EEPROM.read(EEPROMnormRideHeight);
int lowRideHeight = EEPROM.read(EEPROMlowRideHeight);
int allDownHeight = 0;

//Declare button variables
byte ringLed = 6;
byte rideHeightBtn = 5;

byte activated = 0;
byte pushes = 0;

//Declare leveler variables
int leveler = A0;
int levelerRead;
int level;
int levelRaiseHigh;
int levelRaiseMed;
int levelRaiseLow;
int levelLowerHigh;
int levelLowerMed;
int levelLowerLow;


int rideHeightBtnState = 0;
int rideHeightBtnState2 = 0;
int rideHeightBtnState3 = 0;
int cancelSelection = 0;

//Random variables without a good home
int timeoutAmount = 1000;

//Create nextUpdate value for the MasterButton
unsigned long time;
long nextUpdate;

void setup()
{
  Serial.begin(9600);
  pinMode(leveler, INPUT);
  pinMode(5, INPUT);
  pinMode(6, OUTPUT);
  pinMode(7, OUTPUT);
  pinMode(8, OUTPUT);
  pinMode(9, OUTPUT);
  pinMode(10, OUTPUT);
  pinMode(11, INPUT);
  pinMode(12, INPUT);
  pinMode(13, INPUT);
  pinMode(3, INPUT);
  pinMode(2, INPUT);
}

void loop()
{
  digitalWrite(ringLed, HIGH);
  updateLeveler();
  Serial.println(level);
  rideHeightBtnState = digitalRead(rideHeightBtn);
  if (rideHeightBtnState == HIGH)
  {
    activated = activated++;
    Serial.print("Select a height... ");
    delay(100);
    readyBlink();
  }
  
  manualButtons();
  
  if (activated > 0)
  {
    rideHeightBtnState = digitalRead(dummyLowPin);
    updateTimer();
    while(time < nextUpdate && pushes < 14)
    {
      time = millis();
      rideHeightBtnState2 = digitalRead(rideHeightBtn);
      if (rideHeightBtnState2 == HIGH)
      {
        pushes = pushes++;
        if (pushes < 4)
        {
          nextUpdate = nextUpdate + 500;
        }
        digitalWrite(rideHeightBtnState2, LOW);
        delay(150);
      }
    }
    if (pushes == 3)
    {
      buttonLedBlink3();
      Serial.print("High ride height selected!");
      currentRideHeight = highRideHeight;
      executeHeight();
    }
    if (pushes == 2)
    {
      buttonLedBlink2();
      Serial.print("Normal ride height selected!");
      currentRideHeight = normRideHeight;
      executeHeight();
    }
    if (pushes == 1)
    {
      buttonLedBlink1();
      Serial.print("Low ride height selected!");
      currentRideHeight = lowRideHeight;
      executeHeight();
    }
    if (pushes > 6)
    {
      Serial.print("All down, air it out!");
      currentRideHeight = allDownHeight;
      EEPROMheight();
      updateLeveler();
      allDown();
    }
    if (pushes <= 0)
    {
      Serial.print("No height selected");
      buttonLedError();
    }

    //Clean up of the second stage of the button
    rideHeightBtnState = digitalRead(rideHeightBtn);
    rideHeightBtnState2 = digitalRead(dummyLowPin);
    rideHeightBtnState3 = digitalRead(dummyLowPin);
    cancelSelection = 0;
    pushes = 0;
    activated = 0;
    delay(250);
  }
}

void readyBlink()
{
  digitalWrite(ringLed, LOW);
  delay(150);
  digitalWrite(ringLed, HIGH);
}

void buttonLedBlink1()
{
  digitalWrite(ringLed, LOW);
  delay(100);
  digitalWrite(ringLed, HIGH);
  delay(250);
  digitalWrite(ringLed, LOW);
}

void buttonLedBlink2()
{
  digitalWrite(ringLed, LOW);
  delay(100);
  digitalWrite(ringLed, HIGH);
  delay(250);
  digitalWrite(ringLed, LOW);
  delay(100);
  digitalWrite(ringLed, HIGH);
  delay(250);
  digitalWrite(ringLed, LOW);
}

void buttonLedBlink3()
{
  digitalWrite(ringLed, LOW);
  delay(100);
  digitalWrite(ringLed, HIGH);
  delay(250);
  digitalWrite(ringLed, LOW);
  delay(100);
  digitalWrite(ringLed, HIGH);
  delay(250);
  digitalWrite(ringLed, LOW);
  delay(100);
  digitalWrite(ringLed, HIGH);
  delay(250);
  digitalWrite(ringLed, LOW);
}

void buttonLedError()
{
  digitalWrite(ringLed, LOW);
  delay(1300);
  digitalWrite(ringLed, HIGH);
}

void updateTimer()
{
  time = millis();
  nextUpdate = time + timeoutAmount;
}

void readButton3()
{
  rideHeightBtnState3 = digitalRead(rideHeightBtn);
  if (rideHeightBtnState3 == HIGH)
  {
    cancelSelection = 1;
  }
}

void updateLeveler()
{
  levelerRead = analogRead(leveler);
  levelerRead = map(levelerRead, 0, 1023, 0, 60);
  level = levelerRead;
  levelRaiseHigh = level - 7;
  levelLowerMed = currentRideHeight + 5;
  levelLowerLow = currentRideHeight + 2;
  delay(10);
}

void lowerCarFull()
{
  digitalWrite(bag4, HIGH);
  digitalWrite(bag3, HIGH);
  digitalWrite(bag2, HIGH);
  digitalWrite(bag1, HIGH);
  delay(100);
  readButton3();
  digitalWrite(bag4, LOW);
  digitalWrite(bag3, LOW);
  digitalWrite(bag2, LOW);
  digitalWrite(bag1, LOW);
  delay(100);
}

void lowerCarSlow()
{
  digitalWrite(bag4, HIGH);
  digitalWrite(bag3, HIGH);
  digitalWrite(bag2, HIGH);
  digitalWrite(bag1, HIGH);
  delay(400);
  readButton3();
  digitalWrite(bag4, LOW);
  digitalWrite(bag3, LOW);
  digitalWrite(bag2, LOW);
  digitalWrite(bag1, LOW);
  delay(400);
}

void raiseCar()
{
  digitalWrite(bag4, HIGH);
  digitalWrite(bag3, HIGH);
  digitalWrite(bag2, HIGH);
  digitalWrite(bag1, HIGH);
}

void killAllValves()
{
  digitalWrite(bag1, LOW);
  digitalWrite(bag2, LOW);
  digitalWrite(bag3, LOW);
  digitalWrite(bag4, LOW);
}

void adjustBags()
{
  while(levelRaiseHigh < currentRideHeight && cancelSelection == 0)
  {
    raiseCar();
    updateLeveler();
    readButton3();
  }
  while(level > levelLowerMed && cancelSelection == 0)
  {
    lowerCarFull();
    updateLeveler();
    readButton3();
  }
  while(level > currentRideHeight && cancelSelection == 0)
  {
    lowerCarSlow();
    updateLeveler();
    readButton3();
  }
  killAllValves();
}

void allDown()
{
  rideHeightBtnState2 = digitalRead(dummyLowPin);
  EEPROMheight();
  updateLeveler();
  while(level > currentRideHeight && cancelSelection == 0)
  {
    lowerCarFull();
    updateLeveler();
    readButton3();
  }
  killAllValves();
}

void EEPROMheight()
{
  EEPROM.write(EEPROMcurrentRideHeight, currentRideHeight);
}

void executeHeight()
{ 
  rideHeightBtnState2 = digitalRead(dummyLowPin);
  EEPROMheight();
  updateLeveler();
  adjustBags();
}

void readManualButtons()
{
  bag1ButtonState = digitalRead(bag1Button);
  bag2ButtonState = digitalRead(bag2Button);
  bag3ButtonState = digitalRead(bag3Button);
  bag4ButtonState = digitalRead(bag4Button);
  manualSwitchState = digitalRead(manualSwitch);
}

void manualButtons()
{
  readManualButtons();
  if (bag1ButtonState == HIGH && manualSwitchState == LOW)
  {
    digitalWrite(bag1, HIGH);
    delay(100);
    digitalWrite(bag1, LOW);
    delay(100);
    readManualButtons();
  }
  
  if (bag2ButtonState == HIGH && manualSwitchState == LOW)
  {
    digitalWrite(bag2, HIGH);
    delay(100);
    digitalWrite(bag2, LOW);
    delay(100);
    readManualButtons();
  }
  
  if (bag3ButtonState == HIGH && manualSwitchState == LOW)
  {
    digitalWrite(bag3, HIGH);
    delay(100);
    digitalWrite(bag3, LOW);
    delay(100);
    readManualButtons();
  }
  
  if (bag4ButtonState == HIGH && manualSwitchState == LOW)
  {
    digitalWrite(bag4, HIGH);
    delay(100);
    digitalWrite(bag4, LOW);
    delay(100);
    readManualButtons();
  }
  
  if (bag1ButtonState == HIGH && manualSwitchState == HIGH)
  {
    digitalWrite(bag1, HIGH);
    readManualButtons();
  }
  
  if (bag2ButtonState == HIGH && manualSwitchState == HIGH)
  {
    digitalWrite(bag2, HIGH);
    readManualButtons();
  }
  
  if (bag3ButtonState == HIGH && manualSwitchState == HIGH)
  {
    digitalWrite(bag3, HIGH);
    readManualButtons();
  }
  
  if (bag4ButtonState == HIGH && manualSwitchState == HIGH)
  {
    digitalWrite(bag4, HIGH);
    readManualButtons();
  }
  
  if (bag1ButtonState == LOW)
  {
    digitalWrite(bag1, LOW);
    readManualButtons();
  }
  
  if (bag2ButtonState == LOW)
  {
    digitalWrite(bag2, LOW);
    readManualButtons();
  }
  
  if (bag3ButtonState == LOW)
  {
    digitalWrite(bag3, LOW);
    readManualButtons();
  }
  
  if (bag4ButtonState == LOW)
  {
    digitalWrite(bag4, LOW);
    readManualButtons();
  }
}
  
  

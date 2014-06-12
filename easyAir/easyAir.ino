#include "puff.h"
#include <EEPROM.h>

//Define pins here according to comments
Puff puff(12, // button    = Main command input pin               [0] -  Digital/INPUT
          13, // buttonLed = LED for main command input telemetry [1] -  Digital/OUTPUT
          2, // valveUp1        = Up valve for front driver side       [2] -  Digital/OUTPUT
          0, // valveDown1      = Down valve for front driver side     [6] -  Digital/OUTPUT
          0, // valveUp2        = Up valve for front passenger side    [3] -  Digital/OUTPUT
          0, // valveDown2      = Down valve for front passenger side  [7] -  Digital/OUTPUT
          3, // valveUp3        = Up valve for rear driver side        [4] -  Digital/OUTPUT
          0, // valveDown3      = Down valve for rear driver side      [8] -  Digital/OUTPUT
          0, // valveUp4        = Up valve for rear passenger side     [5] -  Digital/OUTPUT
          0, // valveDown4      = Down valve for rear passenger side   [9] -  Digital/OUTPUT
          A0, // leveler1        = Leveler for front driver side        [10] - Analog/INPUT
          A1, // leveler2        = Leveler for front passenger side     [11] - Analog/INPUT
          0, // leveler3        = Leveler for rear driver side         [12] - Analog/INPUT
          0, // leveler4        = Leveler for rear passenger side      [13] - Analog/INPUT
          0,// X               = X axis for manual adjust joystick    [14] - Analog/INPUT
          0,// Y               = Y axis for manual adjust joystick    [15] - Analog/INPUT
          0 // Select          = Joystick button for up/down direction[16] - Analog/INPUT
);

void setup() { 
  //Run setup function for puff
  puff.setup();
  Serial.begin(9600);
}


void loop() {
  puff.go(); //Run main code for puff
}

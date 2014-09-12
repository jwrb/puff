Arduino Air Ride Control
=========

This system is based off the Arudino Programming Platform with supporting C libraries for reusable functions that take variables in an effort to shield the beginning user as much as possible from the heavy lifting. For height selection the entire system is controlled via one input. This input is usually a simple push button. For fedback on the system a second circuit can be connected over pin x for led telemetry. There are three programmable heights with a the 4th command being the "all down" setting. Standard convention for height programming is as follows:

- 3 - High Ride Height
- 2 - Normal Ride Height
- 1 - Low Ride Height
- Hold - All Down

###**OPERATION:**###
Send HIGH input once to wake up system (LED circuit will blink once when it registers the input). Once LED has blinked the system is ready to receive inputs based on the programmed height you desire. For example:
- Press button once to wake up system.
- Wait for blink
- Press button twice to select height 2

To send the "all down" command keep a high input (press and hold button) for 2-3 seconds. WARNING: due to limited input devices the "set to programming mode" command is executed by holding a high input for 4 seconds. Be careful not to hold button for too long while selecting the all down command or programming mode will be initialized.

###**MANUAL ADJUSTMENT:**###

Manual adustment is controlled via joystick. The variable axis control from the nature of a joystick at design level allows different corners of the car to be grouped and manipulated on the fly. Pushing the joystick directly forward  will control both front corners together. Pushing forward and to either side in a diagonal fashion will control only that corner the joystick is pointing at. This same grouping scheme applies for all corners and directions. To select between "up" and "down" simply press down on the joystick in its center position as if pushing a button and the system will change between options (when the chip is reset the default direction is up when booted again).

###**PROGRAMMING:**###
To enter programming mode hold a high input for 4 seconds. LED circuit will fade from high to low and repeat when successful. Once in programming mode push the button a number of times correspoinding to the height you would like to select. The LED will now be blinking in a short to long pattern correspoinding to the hieght selected. For example if you wanted to edit height 2:

- Enter programming mode and push button twice
- LED blinks twice then pauses in the off position and then repeats
- The system is now waiting as you set your desired heights with the joystick control
- Once you have adjusted to your desired height simply click the button once and your height is saved
- The system will exit programming mode and be ready to adjust to a specified height after each saved level

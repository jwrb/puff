Arduino Air Ride Control
=========

This system is based of the Arudino Framework with supporting C libraries for reusable functions that take variables in a effort to shied the beginning user as much as possible from the heavy lifting.

For auto leveling the entire selection process is controlled via one input. This input is usually a simple push button but for feedback on the system a second circuit can be connected to pins x and x for led telemetry. There are three programmable heights with a the 4th command being the "all down" setting. Standard convention for height programming is as follows:

3 - High Ride Height
2 - Normal Ride Height
1 - Low Ride Height
Hold - All Down

##**OPERATION:**##
Send HIGH input once to wake up system (LED circuit will blink once when it registers the input). Once LED has blinked the system is ready to receive inputs based on the programmed height you desire. For example:
- Press button once to wake up system.
- Wait for blink
- Press button twice to select height 2

To send the "all down" command keep a high input (press and hold button) for 2-3 seconds. WARNING: due to limited inputs the "set to programming mode" command is executed by holding a high input for 4 seconds. Be careful not to hold button for too long or programming mode will be initialized.

##**PROGRAMMING:**##
To enter programming mode hold a high input for 4 seconds. LED circuit will fade from high to low when successful.

As of the most recent commit the ride heights are hard and not changeable on the fly. They are declared before the voids in the Arduino sketch. Also all corners are reading off of one potenitometer.

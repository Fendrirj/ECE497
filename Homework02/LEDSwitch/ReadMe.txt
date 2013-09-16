ledswitch is a program that tracks button inputs and toggles LEDs accordingly.
To use, simply run ledswitch. 

Connect your buttons to ports 14, 16, 21, and 22. For the buttons
connected to ports 14 and 16, tie one end to 3.3V and the other to
the port. For ports 21 and 22, tie one end to ground and the other
to the port. Due to some peculiarities with how Beaglebone has setup
its GPIOs, ports 14 and 16 have pull down resistors and ports 21 and 22
have pull up resistors.

Connect your LEDs to ports 11, 13, 15, and 12.

Once everything is connected properly, run the program. If you have
each LED connected to the GPIO and then to ground (with a resistor of
course), you'll note that two LEDs and off and two are lit. This is normal.
Each button when pressed will toggle the state of its given LED.

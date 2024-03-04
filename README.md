Practical Scenario: 
Temperature Monitoring in a Room with Overheating Indicator
In a context where we want to monitor the temperature in a room, we design a system
simply with the help of an Arduino board and a temperature sensor. The main purpose of this
project is to provide an efficient way to check the ambient temperature and warn
the user if it reaches critical levels.

Usage Scenario:
The user installs the system in a room. The Arduino board constantly reads the temperature
ambient with the help of the sensor and displays it on the digital screen. At the same time, the LED
warning light remains off as long as the temperature is below the critical threshold.
If the temperature rises and reaches or exceeds 25 degrees Celsius, the LED lights up
to warn the user that a high temperature has been reached in the room. At the same time, the information
detailed temperature information is displayed in the Serial Monitor, allowing the user to analyze
temperature evolution.
This simple system provides an efficient solution for temperature monitoring in rooms and
assures users that they can react quickly in overheating situations.
Operation and Mode of Work:
The temperature sensor constantly reads the ambient temperature. The Arduino program compares
this value with the set threshold. If the temperature exceeds the threshold, the LED lights up and
displays the temperature on the 4-digit 7-segment display. The user can thus monitor
ambient temperature in real time and react to any significant changes.

List of Required Modules (Bill of Materials):
Arduino Nano R3,
Breadboard 830p,
74HC595 IC,
4 Digit 7-Segment Display,
8 x 220 ohm resistors,
1 x 330 ohm resistors (for LED),
1 x 10k ohm resistor (for Thermistor),
Thermistor,
LED,
M-M wires (Male to Male jumper wires).

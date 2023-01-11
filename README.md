# Python file for Arduino Wifi Rev2 Motion Recorder 

Connect the Arduino and run to initiate the program 
There can be error if Arduino Serial port is not COM5 
This can simply solve by edit the 8th line of the Source code 

arduino = serial.Serial(port='COM5', baudrate=9600, timeout=.5)

Simply open Arduino IDE or any Serial Monitor program to check which port arduino is connected to 

If connection is established 

Send Enable : 

This will appear just type "enable"

to start UART data transfer 

Ctrl + C to stop

Send Enable : 

This will appear just type "enable" again to continue 

or Ctrl + C once more to quit 

The live data will contain 6 values in a param variable array

6 Variables 

x_acceleration_in(g) , y_acceleration_in(g) , z_acceleration_in(g)
x_omega,y_omega and z_omega is in an array named "param " You can put you code here to manage Serial data

Use param array data stream to your desire 

Viel Glück

Created by Panitan Kwankaew 

School of Engineering , Bangkok University 


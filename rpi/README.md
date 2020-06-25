**Aercus-Microdot.c** 

This program was written to run on a Raspberry Pi zero W. It listens for pushed data from an *Aercus weathersleuth* weather station and outputs selected values to a micro dot pHAT connected via I2c.

See [here](http://www.aercusinstruments.com/aercus-instruments-weathersleuth-professional-ip-weather-station-with-direct-real-time-internet-monitoring/ "In the UK you can buy them from a company called Greenfrog Scientific") for details about the Aercus product I'm using.

The display can be bought from [Pimoroni](https://shop.pimoroni.com "Micro Dot pHAT"). It comes as a kit you have to assemble, but it's simple enough to build.

To build an executable all you have to do is compile Aercus-Microdot.c and copy it somewhere sensible (/usr/local/bin).
If you want it to be run from boot, I've included weather.service for use with systemctl. When the program is running, any
errors or debug messages are written to syslog. If you want to see the debug messages, uncomment the DEBUG define.

I've got nothing to do with Aercus. I just bought one of these and after a few years of running I've had no problems with it. 
I wanted to see what the weather is doing without having to go on the Internet to look at Weather Underground..

  ![](https://github.com/wicked-rainman/wicked-rainman.github.io/blob/master/pictures/mdot.png "Yes, it looks a bit pants, but a: I've got a cheap 3d printer, b: I didn't clean it before I took the picture, c: The camera on my phone is too good at close-ups and lastly, d: I'm not sure I actually care")

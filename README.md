
# The Aercus weather station

A few years ago, my wife bought me an Aercus weather station. It's sat there for a few years, sending it's readings to Weather Underground without fault.

After a while I simply got fed up of having to fire up a computer each time I wanted to see the readings and decided to make some changes.

The station itself is designed to send a GET request (populated with the weather values) to an Internet server (often Weather Underground) via its Ethernet interface.
There is a port 80 socket open on the station (HTTP) that allows you to reconfigure where it sends this request. I changed it from weather underground to a local IP
address so I could get the data and process it for myself.
I have a few Raspberry Pis that are on my network all the time, so I wrote a simple socket service that listens to the GET requests
from the station, extracts out all the weather values and then re-sends this sanitised data as a broadcast udp packet. This allows me to 
view the data on as many  devices as I like, provided they areconnected to my local network.

The code to do this (written for a Raspberry pi) is held in file UdpBroadcaster.c. 
I've written some client code for an M5Stack (M5StackUdpClient.ino), a simple Unix terminal client (simpleUdpClient.c) and a Unix curses version (udpCursesClient.c).

![](https://github.com/wicked-rainman/wicked-rainman.github.io/blob/master/pictures/wstack.png "Just look at those lovely colours!")

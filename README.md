
# The Aercus weather station

A few years ago, my wife bought me an Aercus weather station. It's sat there for a few years, sending it's readings to Weather Underground without fault.

After a while I simply got fed up of having to fire up a computer each time I wanted to see the readings and decided to make some changes.

The station itself is designed to send a GET request (populated with the weather values) to an Internet server (often Weather Underground) via its Ethernet interface.
There is a port 80 socket open on the station (HTTP) that allows you to reconfigure where it sends this request. I changed it from weather underground to a local IP
address so I could get the data and process it for myself.
I have a few Raspberry Pis that are on my network all the time, so I wrote a simple socket service that listens to the GET requests
from the station, extracts out all the weather values and then re-sends this sanitised data as a broadcast udp packet. This allows me to 
view the data on as many  devices as I like, provided they areconnected to my local network.

The code to do this (written for a Raspberry pi) is held in file UdpBroadcaster.c. I run it as a demon service using systemctl.

I've written some client code for an M5Stack (M5StackUdpClient.ino), a simple Unix terminal client (simpleUdpClient.c) and a Unix curses version (udpCursesClient.c).

In the rpi directory you'll find a sockets based client for a Raspberry Pi that reads and translates the get request directly from the weather station and displays it on a dot-matrix display. It works well, but I've abandoned the idea because it only allows for one client display at any one time.

The picture below shows the client running on an M5Stack Fire. The bargrap at the top shows the humidity and the yellow block right at the bottom moves each time an update is received.... Obviously, with a small screen only some of the weather values are being displayed.

I've seen other folk write a network traffic sniffer to achieve the same results - but I think this is a more simple approach. 

The UdpBroadcaster code could still forward the received packet to Weather Undergroun, but I've simply not writtend the code to do that yet.

![](https://github.com/wicked-rainman/wicked-rainman.github.io/blob/master/pictures/wstack.png "Just look at those lovely colours!")

Here's a picture of the udpCursesClient.c runnning under Ubuntu. The progress bar along the bottom makes it easy to see if it's still receiving packets from the UdpBroadcaster.

![](https://github.com/wicked-rainman/wicked-rainman.github.io/blob/master/pictures/udplisten.png "Just look at those lovely colours!")

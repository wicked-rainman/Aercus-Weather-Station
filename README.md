
# The Aercus weather station

A few years ago, my wife bought me an Aercus weather station. It's sat there for a few years, sending it's readings to Weather Underground without fault.

After a while I simply got fed up of having to fire up a computer each time I wanted to see the readings and decided to make some changes.

The station itself is designed to send a GET request (populated with the weather values) to an Internet server via its Ethernet interface.
There is a port 80 interface (http) open on the station that allows you to reconfigure where it sends this request. I changed it to a local IP
address so I could process this data myself.
I have a few Raspberry Pis that are on my network all the time, so I wrote a simple socket service that listens to the GET requests
from the station, extracts out all the weather values and then re-sends this sanitised data as broadcast udp packet. This allows me to 
view the data on any device connected to my network.

![](https://github.com/wicked-rainman/wicked-rainman.github.io/blob/master/pictures/wstack.png "Just look at those lovely colours!")

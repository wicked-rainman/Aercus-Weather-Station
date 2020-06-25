
# The Aercus weather station

A few years ago, my wife bought me an Aercus weather station. It's sat there for a few years, sending it's readings to Weather Underground without fault.

After a while I simply got fed up of having to fire up a computer each time I wanted to see the readings and decided to make some changes.

The station itself is designed to send a GET request (populated with the weater values) to an Internet server via its Ethernet interface.
There is a port 80 interface (http) open on the station that allows you to reconfigure where it sends this request. I changed it to a local IP
address so I could process this data myself.

![](https://github.com/wicked-rainman/wicked-rainman.github.io/blob/master/pictures/wstack.png "Just look at those lovely colours!")

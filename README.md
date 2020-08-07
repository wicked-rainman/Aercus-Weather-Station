
# Aercus weather station related code

In this repo you'll find various bits of code that I've created as a spin-off to owning an Aercus Weather station.

The station is designed to update a site such as weather underground. I've changed the configuration so it sends the HTTP requests to a local host on my network. I then take this data, strip out the values I want and send it out as a UDP broadcast on my local network. As it's now a broadcast, any number of hosts on my network can view the weather output...

In here you will find the HTML to UDP converter and code for various UDP broadcast clients.

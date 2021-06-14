# sensus_protocol_esp8266_lib

This is arduino (ESP8266) library for reading Sensus Protocol.

Was developed mainly to be able to read Sensus SRD 2 water meter, but could be usable for others. Depedning on meter, the string returned may be different, but the protocol to read the meter is the same.

Water Meter Pinout
------------------
most Sensus compatible meters (including the elster/AMCO meter) there are 3 wires (or three labeled terminals):

RED:    This is the clock signal and also power. Depeding on 
        meter type, it may need 5V to run, I managed to read
        my meter using 3.3V. When meter is without any power
        for about 1 second, it resets itself. Then one needs
        to keep power up and to fetch next bit, just briefly
        remove power and restore it back. After that data
        line will indicate next bit.

GREEN:  This is the data signal and will be used to read the
        meter data. Ther line is open collector, i.e. it
        shorts to the ground, but nothing pulls is up. When
        line pulls low (i.e. ground) it signals 0 otherwise
        1.

BLACK:  Ground.


This library was tested with ESP8266 (ESP-01) and meter connected to GPIO2 (clock) and GPIO0 (data) lines. The connections are as below:

RED:   connect to 3.3V
BLACK: connect to GPIO2, there also needs to be 10k resistor
       between GPIO2 and 3.3V.
GREEN: connect to GPIO0, there also needs to be 10k resistor
       between GPIO0 and 3.3V

If your board already have external pull up resistors on GPIO0 & GPIO2, you may not need extra ones. I had issues with pull up resistors less than 10k, reading was not reliable.

The connection may not be conventional, but it ensures that both GPIO pins are pulled up on startup, so the ESP8266 starts up fine. Also it makes sure that there is no voltage on the meter, to reset it cleanly. When GPIO2 is pulled down, it takes about 0.1V away, so the meter operates on 3.2V.

The data format from the meter I have is:

start bit (0), 7 data bits (LSB), parity bit, stop bit (1)

My meter seems to send:

R226107789670\r

i.e. Rxxxxyyyyyyyy\r

xxxx - is reading in m3
yyyyyyyy - not completely sure, does not seem to change.
\r - carriage return, after that the meter read repeats


Some of this code was inspired by https://github.com/rszimm/kmeter by Rich Zimmerman, which gave me an idea how to read the meter. Thanks for sharing the code.

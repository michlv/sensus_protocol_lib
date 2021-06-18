# sensus_protocol_lib

This is arduino library for reading Sensus Protocol (UI-1203). It was developed on ESP8266 (ESP-01), but should also work with Arduino, if the meter is connected the same way.

Developed to read Sensus SR D II (SR D 2) water meter, but could be usable for others. Depedning on meter, the string returned may be different, but the protocol to read the meter should be the same.

Water Meter Pinout
------------------
Most Sensus compatible meters (including the elster/AMCO meter) there are 3 wires (or three labeled terminals):

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
BLACK: connect to GPIO2
GREEN: connect to GPIO0

The data read pin is configured by default with integrated pull-up resistor.

The connection may not be conventional, but it ensures that both GPIO pins are pulled up on startup, so the ESP8266 starts up fine. Also it makes sure that there is no voltage on the meter, so it resets cleanly. When GPIO2 is pulled down, it takes about 0.1V away, so the meter operates on 3.2V.

If your ESP board does not boot up, one may need to add 10k or higher pull up resitors between GPIO2 and 3.3V or GPIO0 and 3.3V.

For Adruino, I expect, no pull up resistors are needed and in the above where 3.3V is used, just replace with 5V instead.

The data format from the meter I have is:

start bit (0), 7 data bits (LSB first), parity bit, stop bit (1)

My meter seems to send:

R226107229550\r

i.e. Rxxxxyyyyyyyy\r

xxxx - is reading in m3
yyyyyyyy - not completely sure, does not seem to change.
\r - carriage return, after that the meter read repeats


Some of this code was inspired by https://github.com/rszimm/kmeter by Rich Zimmerman, which gave me an idea how to read the meter. Thanks for sharing the code.

Some other info I also got from http://www.plctalk.net/qanda/showthread.php?t=79284

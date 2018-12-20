QlockToo
========

PIC-based QlockTwo clone


Connections
===========

Expansion port  
pin | function  
----+---------
 1  | VDD  
 2  | SDA (i2c)  
 3  | SCL (i2c)  
 4  | RE2/AN7/OESPP  
 5  | RE1/AN6/CK2SPP  
 6  | nc  
 7  | GND  


Version history
===============

0.5
---
define MODE_CLOCK              0  
define MODE_SECONDS            1  
define MODE_BRIGHTNESS         2 -> does not save correctly  
define MODE_TIME_SET_HOURS     3  
define MODE_TIME_SET_MINS      4  
define MODE_BRIGHTNESS_MODE    5  
define MODE_SHOW_MINUTES       6  
define MODE_LEDTEST            7  
define MODE_LEDTEST_MANUAL     8  
define MODE_ANIMATION_1        9  
define MODE_OFF                10  
define MODE_ALL                11  


# VL53L5CX 

Sensor: VL53L5CX
Board: Raspberyy Pi3 B+

NOTE: Since I am using four sensors, Lpn pins are connected to GPIO 17, GPIO 27, GPIO 22, GPIO 23 on the raspberry.

NOTE: AVDD and IOVDD must be supplied with voltage.

**NOTE: A maximum voltage of 3.3V should be applied to the IOWA input.**
**NOTE: When assigning an address, it should be drawn to the 3.3v and GND lines with a 47k resistor.**

NOTE: AVDD and IOVDD can be connected to 3.3v by short circuit.

NOTE: The code I wrote to change the vl53l5cx i2c address should be run with the `sudo` command.


#### Library and Basic Settings: [Linux driver for VL53L5CX multi-zone sensor ](https://www.st.com/en/embedded-software/stsw-img025.html) 

* vl53l5cx, i2c address change
* simultaneous use of multiple vl53l5cx





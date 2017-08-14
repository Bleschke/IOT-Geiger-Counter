# IoT-Geiger-Counter

Brian Leschke 2017
 
- Update 8/9/2017:  Formed project idea
- Update 8/11/2017: Geiger Counter built
- Update 8/12/2017: Merge and custom code completed! Adafruit Parts built. Working Release!

NOTE: Personal webpage not yet implemented. This may be done in the future. 

Future Ideas
- User prompt on startup on whether or not to connect to wifi and upload data to Adafruit.io
- WiFi signal strength meter
- Personal webpage displaying graphs and charts
- Time display
- GPS logging

## **Overview**

This project utilizes an Adafruit Feather Huzzah esp8266, neopixels, and a Feather OLED to display radiation in uS/hr, CPS, and CPM. Information will display on an oled and a webpage. 

An Adafruit Feather Huzzah programmed with the Arduino Software using an microusb cable will attempt to connect to the specified wifi network and geiger counter. If successful, information will populate on the webpage and the oled screen. 

Project Breakdown:
* Neopixels
    * Warning for radiation levels (Blinks with detection)
      * Blinks Indigo (1-49 CPM)
      * Blinks Blue   (50-99 CPM)
      * Blinks Green  (100-149 CPM)
      * Blinks Yellow (150-299 CPM)
      * Blinks Orange (300-499 CPM)
      * Blinks Red    (500-749 CPM)
      * Blinks White  (>750 CPM)
* OLED
    * Displays radiation levels
      * CPM - counts per minute
      * CPS - counts per second
      * µS/hr - microSievert
* Webpage
    * Displays radiation information
        * Graphs
        * Charts
        * Downloadable data

### **Prerequisities**

You will need:

1. MightyOhm Geiger Counter Kit (http://mightyohm.com/blog/products/geiger-counter/)
2. Adafruit Feather Huzzah ESP8266 (http://adafru.it/2821)
3. Neopixels (http://adafru.it/1312)
4. Feather OLED (http://adafru.it/2900)
5. Arduino programming software
6. Python 2.7.x installed on your operating system (used for OTA updates).
7. FTDI cable (https://www.sparkfun.com/products/9717)
8. Lithium Ion Battery Pack - optional (http://adafru.it/2011)


### **Usage**

In order to use successfully implement this project and its code, you will need to install the Arduino Programming Software and know how to solder.
    
### **Libraries**

The following libraries are required for this project:
    
  * [Arduino Framework (core library)](https://github.com/esp8266/Arduino)
  * ArduinoOTA
  * ArduinoJson
  * Adafruit_Neopixel
  * ESP8266WiFi
  * [EspSoftSerialRx](https://github.com/scottwday/EspSoftSerial)
  * Adafruit_SSD1306
  * NTPClient
  * [Adafuit LED Backpack](https://github.com/adafruit/Adafruit-LED-Backpack-Library)
  * [Adafuit GFX](https://github.com/adafruit/Adafruit-GFX-Library)
        
## **Uploading**

The code can be uploaded to the ESP8266 in two ways, either by OTA (over-the-air) updates or a microusb cable (hardwire) connection.
Users will need to initially upload the code using a microusb cable.

### **MicroUSB Upload**

This method must be used in the initial flashing of the ESP8266.  

### **OTA Upload**

Over-the-air updates is a method to update the ESP8266 without physically being with the device. To update the device you will need to
know the IP address and be connected to the same network.

If for some reason, the network ESP8266 network configuration is incorrect, the device will restart.

## **WARNING!**
This kit converts electricity into high voltage for the Geiger Tube (SBM-20). I am not responsible and liable for any harm or damage done to you, others around you, or property of any kind. 
        
## **Recognition and Credit**
This project will use the code from Dan Watson [SyncChannel] (https://gist.github.com/SyncChannel/ffe320953fa26d3ab9b5).
Most of my credit goes to him! 


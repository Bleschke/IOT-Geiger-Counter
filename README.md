# IOT-Geiger-Counter
An Adafruit Feather Huzzah with a Feather OLED expansion will be connected to a MightyOhm Geiger Counter (kit). The Feather Huzzah is programmed to display uS/hr, CPS, and CPM on both a webpage and the oled screen. 

Brian Leschke 2017
 
- Update 8/10/2017: Formed project idea
- 

## **Overview**

This project utilizes an Adafruit Feather Huzzah esp8266, neopixels, and a Feather OLED to display radiation in uS/hr, CPS, and CPM. Information will display on an oled and a webpage. 

An Adafruit Feather Huzzah programmed with the Arduino Software using an FTDI cable will attempt to connect to the specified wifi network and geiger counter. If successful, information will populate on the webpage and the oled screen. 

Project Breakdown:
* Neopixels
    * Warning for radiation levels over 500 CPM (Blinks with detection)
      * Blinks Blue   (1-99 CPM)
      * Blinks Green  (100-199 CPM)
      * Blinks Yellow (200-499 CPM)
      * Blinks Orange (500-749 CPM)
      * Blinks Red    (>750 CPM)
* OLED
    * Displays radiation levels
    * CPM - counts per minute
    * CPS - counts per second
    * uS/hr - microSievert
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


### **Usage**

In order to use successfully implement this project and its code, you will need to install the Arduino Programming Software and know how to solder.
    
### **Libraries**

The following libraries are required for this project:
    
  * [Arduino Framework (core library)](https://github.com/esp8266/Arduino)
  * ArduinoOTA
  * ArduinoJson
  * Adafruit_Neopixel
  * ESP8266WiFi
  * EspSoftSerialRx
  * Adafruit_SSD1306
  * NTPClient
  * [Adafuit LED Backpack](https://github.com/adafruit/Adafruit-LED-Backpack-Library)
  * [Adafuit GFX](https://github.com/adafruit/Adafruit-GFX-Library)
        
## **Uploading**

The code can be uploaded to the ESP8266 in two ways, either by OTA (over-the-air) updates or by a serial (FTDI cable) connection.
Users will need to initially upload the code using an FTDI cable.

### **FTDI Upload**

This method must be used in the initial flashing of the ESP8266. An FTDI or UART cable has 6 pins.
Every time the you want to flash the ESP8266, you will need to do the following to enter the bootloader mode:

  1. Press and hold the RESET button.
  2. Press and hold the GPIO0 button.
  3. Release the RESET button and continue to hold the GPIO0 button.
  4. Release the GPIO0 button (you should see a faint red glow from the GPIO0 LED).
  

### **OTA Upload**

Over-the-air updates is a method to update the ESP8266 without physically being with the device. To update the device you will need to
know the IP address and be connected to the same network.

If for some reason, the network ESP8266 network configuration is incorrect, the device will restart.

## **WARNING!**
This kit converts electricity into high voltage for the Geiger Tube (SBM-20). I am not responsible and liable for any harm or damage done to you, others around you, or property of any kind. 
        
## **Recognition and Credit**
This project will combine the code from users [SynchChannel] (https://gist.github.com/SyncChannel/ffe320953fa26d3ab9b5) and [dmainmon] (https://github.com/dmainmon/Geiger-Counter-IoT).
Most of the credit goes to them. 


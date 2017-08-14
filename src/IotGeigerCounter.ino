/*  
* Brian Leschke 
* August 13, 2017 
* Adafruit Feather Huzzah IoT Geiger Counter 
* An Adafruit Feather Huzzah with a Feather OLED expansion will be connected to a MightyOhm Geiger Counter (kit). 
* The Feather Huzzah is programmed to display uS/hr, CPS, and CPM on both a webpage and the oled screen.
* This is a modified version. Initial version created by Dan Watson.
* Version 1.0 
*  
* 
* -- Credit and Recognition: -- 
* Initial program by: Dan Watson (syncchannel.blogspot.com)
* I give him most of the credit. 
*   https://gist.github.com/SyncChannel/ffe320953fa26d3ab9b5
*  
*  
* -- Changelog: --  
*  
* 8/9/2017  - Formed idea, sketched project on paper. Supplies purchased 
* 8/11/2017 - Geiger Counter built 
* 8/12/2017 - Merge and custom code completed! Adafruit Parts built. Working Release!
* 8/13/2017 - Further modified and simplified code. 
*  
*  
**/


#include <SPI.h>
#include <Wire.h>
#include <ArduinoOTA.h>
#include <Adafruit_GFX.h>
#include <gfxfont.h>
#include <Adafruit_SSD1306.h>
#include <CircularBuffer.h>
#include <EspSoftSerialRx.h>
#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#include "Adafruit_MQTT.h"
#include "Adafruit_MQTT_Client.h"

#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
  #include <avr/power.h>
#endif

#define WLAN_SSID       "your ssid here"
#define WLAN_PASS       "your password here"
#define AIO_SERVER      "io.adafruit.com"
#define AIO_SERVERPORT  1883
#define AIO_USERNAME    "your Adafruit IO user name here"
#define AIO_KEY         "your Adafruit IO private key here"

/************ Global State (you don't need to change this!) ******************/

// Create an ESP8266 WiFiClient class to connect to the MQTT server.
WiFiClient client;

// Setup the MQTT client class by passing in the WiFi client and MQTT server and login details.
Adafruit_MQTT_Client mqtt(&client, AIO_SERVER, AIO_SERVERPORT, AIO_USERNAME, AIO_KEY);

/****************************** Feeds ***************************************/

// Modify these feed names to match what you created on Adafruit IO

Adafruit_MQTT_Publish cpsfeed = Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME "/feeds/GeigerCPS");

Adafruit_MQTT_Publish cpmfeed = Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME "/feeds/GeigerCPM");

Adafruit_MQTT_Publish sievertsfeed = Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME "/feeds/GeigerSieverts");

/****************************** NeoPixel Setup ******************************/

// ** Neopixel Setup **
#define PIN            15            // Pin used for Neopixel communication
#define NUMPIXELS      1           // Number of Neopixels connected to Arduino
Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

/************************ NTP Server Information ****************************/

// ** NTP SERVER INFORMATION **
// const char* timeHost = "time-c.nist.gov";
const char* timeHost    = "129.6.15.30";
const int timePort      = 13;

int ln = 0;
String TimeDate = "";

/****************************************************************************/

#define buttonA 0
#define buttonB 16
#define buttonC 2
#define rxPin 13

Adafruit_SSD1306 display;
EspSoftSerialRx SerialGeiger;

#if (SSD1306_LCDHEIGHT != 32)
#error("Height incorrect, please fix Adafruit_SSD1306.h!");
#endif

// Used to store strings arriving from geiger counter
String geigerSerial;

// Storage for stats
int cps = 0;
int cpm = 0;
float sieverts = 0;

// Misc variables for mode and monitoring
uint8_t mode = 1;
unsigned long geigerTimeout = 0;
bool geigerConnected = false;
uint8_t publishCounter = 0;
bool publishStatus = false;

// ---------- CONFIGURATION - DO NOT MODIFY ----------
// Bug workaround for Arduino 1.6.x, it seems to need a function declaration
// for some reason (only affects ESP8266, likely an arduino-builder bug).
//void MQTT_connect();

void setup()
{
  // String from geiger counter will be echoed on USB Serial
  // Software serial Rx-only is used to receive the string from the geiger counter
  Serial.begin(9600);
  SerialGeiger.begin(9600, rxPin);
  
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);

  // All three buttons on the OLED FeatherWing are used in this example
  pinMode(buttonA, INPUT_PULLUP);
  pinMode(buttonC, INPUT_PULLUP);
  pinMode(buttonC, INPUT_PULLUP);
 
 
  display.display();
  delay(2000);
  display.clearDisplay();
  display.setTextColor(WHITE);
  display.setTextSize(1);
  display.setCursor(0,8);
  display.println("IoT Geiger Counter");
  display.println("rev 1.0");
  display.display();

  // Connect to WiFi access point.
  Serial.println(); Serial.println();
  Serial.print("Connecting to ");
  Serial.println(WLAN_SSID);

  WiFi.begin(WLAN_SSID, WLAN_PASS);
  int waitCount = 0;
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
    
    waitCount++;
    if (waitCount > 20)
    {
      Serial.println("Connection Failed! Rebooting...");
      delay(2000);
      ESP.restart();
    }
  }
  Serial.println();

  Serial.println("WiFi connected");
  Serial.println("IP address: "); Serial.println(WiFi.localIP());
  
// ---------- CONFIGURATION - DO NOT MODIFY ----------

// ---------- OTA CONFIGURATION - DO NOT MODIFY ----------

    // Port defaults to 8266
  //ArduinoOTA.setPort(8266);

  // Hostname defaults to esp8266-[ChipID]
  // ArduinoOTA.setHostname("myesp8266");

  // No authentication by default
  ArduinoOTA.setPassword((const char *)"OTA_GEIGERPASS");    // OTA auth password

  ArduinoOTA.onStart([]() {
    Serial.println("OTA Update Initiated");
    display.clearDisplay();
    display.setTextSize(1);
    display.setCursor(0,8);
    display.println("OTA Update Initiated");
    display.display();
  });
  ArduinoOTA.onEnd([]() {
    Serial.println("OTA Update Complete");
    display.clearDisplay();
    display.setTextSize(1);
    display.setCursor(0,8);
    display.println("OTA Update Complete");
    display.display();
  });
  ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
    Serial.printf("Progress: %u%%\r", (progress / (total / 100)));
    int totalProgress  = (progress / (total/100));
    display.clearDisplay();
    display.setTextSize(1);
    display.setCursor(0,8);
    display.println("OTA Progress: ");
    display.print(totalProgress);
    display.print("%");
    display.display();
  });
  ArduinoOTA.onError([](ota_error_t error) {
    Serial.printf("Error[%u]: ", error);
    if (error == OTA_AUTH_ERROR)
    {
      Serial.println("OTA Auth Failed");
      display.clearDisplay();
      display.setTextSize(1);
      display.setCursor(0,8);
      display.println("OTA Auth Failed");
      display.display();
    }
    else if (error == OTA_BEGIN_ERROR)
    {
      Serial.println("OTA Begin Failed");
      display.clearDisplay();
      display.setTextSize(1);
      display.setCursor(0,8);
      display.println("OTA Begin Failed");
      display.display();
    }
    else if (error == OTA_CONNECT_ERROR)
    {
      Serial.println("OTA Connect Failed");
      display.clearDisplay();
      display.setTextSize(1);
      display.setCursor(0,8);
      display.println("OTA Connect Failed");
      display.display();
    }
    else if (error == OTA_RECEIVE_ERROR)
    {
      Serial.println("OTA Receive Failed");
      display.clearDisplay();
      display.setTextSize(1);
      display.setCursor(0,8);
      display.println("OTA Receive Failed");
      display.display();
    }
    else if (error == OTA_END_ERROR)
    {
      Serial.println("OTA End Failed");
      display.clearDisplay();
      display.setTextSize(1);
      display.setCursor(0,8);
      display.println("OTA End Failed");
      display.display();
    }
  });
  ArduinoOTA.begin();
  Serial.println("OTA Ready");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
}
// ---------- OTA CONFIGURATION - DO NOT MODIFY ----------

// ---------- ESP 8266 FUNCTIONS - SOME CAN BE REMOVED ----------

void GeigerCheck() {
  // Check if a button is pressed and change mode accordingly
  yield();
  
  bool gotButton = false;
  
  if (!digitalRead(buttonA))
  {
    mode = 1;
    gotButton = true;
  }

  else if (!digitalRead(buttonB))
  {
    mode = 2;
    gotButton = true;
  }

  else if (!digitalRead(buttonC))
  {
    mode = 3;
    gotButton = true;
  }

  if (gotButton)
    delay(50); // Debounce

  // End of button checking

  // Monitor the serial connection from the geiger counter.
  // If it is absent for > 3 seconds, display a warning to the user
  
  /*
  if (millis() > (geigerTimeout + 3000) && geigerConnected == true)
  {
    geigerConnected = false;
    display.setTextColor(WHITE);
    display.clearDisplay();
    display.setTextSize(1);
    display.setCursor(0,8);
    display.println("GEIGER COUNTER");
    display.println("NOT CONNECTED!");
    display.display();
  }

*/

  // End of serial connection monitoring

  // Attempt to receive a serial character from the geiger counter
  // If we got one, append it to the string
  // If we got a new line character, parse the string and update the display
  // The strings arrive about once a second, so this loop is also used for timing of publish events
    
  unsigned char b;
  if(SerialGeiger.read(b))
  {
    geigerSerial += char(b);

    if (b == 0x0A) // New line, end of string. We need to parse.
    {
      Serial.print(geigerSerial); // Echo the string to USB serial for potential PC monitoring
      geigerConnected = true; // Record that serial connection health is ok
      geigerTimeout = millis();
      publishCounter++; // When this gets to five (five seconds passed) we will publish
      SerialGeiger.service(); // Periodic service routine required for this library

      // Parse CPS, CPM, and uS/hr from the string
      int comma1Index = geigerSerial.indexOf(',');
      int comma2Index = geigerSerial.indexOf(',', comma1Index + 1);
      int comma3Index = geigerSerial.indexOf(',', comma2Index + 1);
      int comma4Index = geigerSerial.indexOf(',', comma3Index + 1);
      int comma5Index = geigerSerial.indexOf(',', comma4Index + 1);
      int comma6Index = geigerSerial.indexOf(',', comma5Index + 1);

      String cpsString = geigerSerial.substring(comma1Index+1,comma2Index);
      String cpmString = geigerSerial.substring(comma3Index+1,comma4Index);
      String sievertsString = geigerSerial.substring(comma5Index+1,comma6Index);

      cps = cpsString.toInt();
      cpm = cpmString.toInt();
      sieverts = sievertsString.toFloat();

      geigerSerial = ""; // Clear the string for next time

      if (cpm >= 1 && cpm <= 49)
      {
        colorWipe(pixels.Color(75, 0, 130), 0); // set color to Indigo
      }
      else if (cpm >= 50 && cpm <= 99)
      {
        colorWipe(pixels.Color(0, 0, 255), 0); // set neopixels to Blue
      }
      else if (cpm >= 100 && cpm <= 149)
      {
        colorWipe(pixels.Color(0 ,128, 0), 0); // set neopixels to Green
      }
      else if (cpm >= 150 && cpm <= 299)
      {
        colorWipe(pixels.Color(255 ,225, 0), 0); // set neopixels to Yellow
      }
      else if (cpm >= 300 && cpm <= 499)
      {
        colorWipe(pixels.Color(255, 165, 0), 0); // set neopixels to Orange
      }
      else if (cpm >= 500 && cpm <= 749)
      {
        colorWipe(pixels.Color(255, 0, 0), 0); // set neopixels to Red
      }
      else if (cpm >=750)
      {
        colorWipe(pixels.Color(255, 255, 255), 0); // set neoPixels to White
      } 
      else
      {
        Serial.println("Error: Outside range. value <1");
      }

      // Update the display depending on what mode we are in
      // Lots of potential to modify/improve this. You could do cool stuff like graphing!
      switch (mode) {
        case 1: // Stats
          display.setTextColor(WHITE);
          display.setTextSize(1);
          display.clearDisplay();
          display.setCursor(4,8);
          display.print("CPS:");
          display.print(cps);
          display.print(" | CPM:");
          display.print(cpm);
          display.setCursor(4,20);
          display.print("uSv/hr: ");
          display.println(sieverts);
          display.display();
          break;
        case 2: // CPM only in large text for greater visibility
          display.setTextColor(WHITE);
          display.clearDisplay();
          display.setTextSize(1);
          display.setCursor(2,19);
          display.print("CPM:");
          display.setTextSize(3);
          display.setCursor(30,4);
          display.print(cpm);
          display.display();
          break;
        case 3: // Status of Wi-Fi, publishing, and IP address
          display.setTextSize(1);
          display.setTextColor(WHITE);
          display.clearDisplay();
          display.setCursor(2,8);
          display.print("Wi-FI: ");
          if (WiFi.status() != WL_CONNECTED)
            display.println("Not Connected");
          else if (!publishStatus)
            display.println("Can't Publish");
          else
            display.println("Connected");
          display.setCursor(2,20);
          display.print("IP: ");
          display.println(WiFi.localIP());
          display.display();
          break;
      }
    }
  }

  MQTT_connect();

  // We publish every five seconds to Adafruit IO
  // If any publish attempt fails, the user will be notified on the Mode 3 screen
  if (publishCounter >= 5)
  {
    publishCounter = 0;
    bool publishResults = true;
    if (!cpsfeed.publish(cps))
      publishResults = false;
    delay(10);
    if (!cpmfeed.publish(cpm))
      publishResults = false;
    delay(10);
    if (!sievertsfeed.publish(sieverts))
      publishResults = false;
    publishStatus = publishResults;
  } 
}

// Function to connect and reconnect as necessary to the MQTT server.
// Should be called in the loop function and it will take care if connecting.
void MQTT_connect() {
  yield();
  int8_t ret;

  // Stop if already connected.
  if (mqtt.connected()) {
    return;
  }

  Serial.print("Connecting to MQTT... ");

  while ((ret = mqtt.connect()) != 0) { // connect will return 0 for connected
       Serial.println(mqtt.connectErrorString(ret));
       Serial.println("Retrying MQTT connection in 5 seconds...");
       mqtt.disconnect();
       delay(5000);  // wait 5 seconds
  }
  Serial.println("MQTT Connected!");
}

// Fill the dots one after the other with a color
void colorWipe(uint32_t c, uint8_t wait) {
  for(uint16_t i=0; i<pixels.numPixels(); i++) {
      pixels.setPixelColor(i, c);
      pixels.setBrightness(50);
      pixels.show();
      delay(wait);
  }
}

// ---------- ESP 8266 FUNCTIONS - SOME CAN BE REMOVED ----------

void loop()
{
  // ---------- USER CODE GOES HERE ----------
  
  // ** Handle OTA server. **
  ArduinoOTA.handle();

  // ** Handle Geiger Counter Check **
  GeigerCheck();

  // ** Receive Time (NTP) **
  //GetTime();

  // ---------- USER CODE GOES HERE ----------
  yield();
}

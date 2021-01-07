//-------------------------------------------------------------------------.-----------.
//                                                                         | Libraries |
//-------------------------------------------------------------------------'-----------'
#include "NTPClient.h"
#include "ESP8266WiFi.h"
#include "WiFiUdp.h"
#include "ESP8266mDNS.h"
#include "ESP8266WebServer.h"
#include "Adafruit_NeoPixel.h"
#include "elapsedMillis.h"
//-------------------------------------------------------------------------.-----------.
//                                                                         | Variables |
//-------------------------------------------------------------------------'-----------'

const int interval = 1200000; //20 minutes
const int timersun = 6;    //6 seconds

int i= 1;

int colorindex = 0; //index number for the color lists
int hourly = 0;

bool alarm = false;

int redList[]   = {  0,8,17,26,35,43,52,61,70,79,87,96,105,114,123,131,140,149,158,167,175,184,193,202,211,219,228,237,246,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,254,253,253,252,252,251,250,250,249,249,248,247,247,246,246,245,245,244,243,243,242,242,241,240,240,239,239,238,237,237,236,236,235,235,234,233,233,232,232,231,230,230,229,229,228,228,227,226,226,225,225,224,223,223,222,222,221,220,220,219,219,218,218,217,216,216,215,215,214,213,213,212,212,211,211,210,209,209,208,208,207,206,206,205,205,204,203,203,202,202,201,201,200,199,199,198,198,197,196,196,195,195,194,193,193,192,192,191,191,190,189,189,188,188,187,186,186,185,185,184,184,183,182,182,181,181,180,179,179,178,178,177,176,176,175,175,174,174,173,172,172,171,171,170,169,169,168,168,167,167,166,165,165,164,164,163,162,162,161,161,160,159,159,158,158,157,157,156,155,155,154,154,
                     153,152,152,151,151,150,150,149,148,147,146,145,144,142,141,140,139,138,137,136,135,134,133,132,131,130,129,128 //300 values
               };
int greenList[] = {  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2,4,6,8,10,13,15,17,19,21,23,26,28,30,32,34,36,39,41,43,45,47,49,52,54,56,58,60,62,65,67,69,71,73,75,78,80,82,84,86,88,91,93,95,97,99,101,104,106,108,110,112,114,117,119,121,123,125,128,128,130,132,135,137,140,142,145,147,150,150,150,150,150,150,150,150,150,150,150,150,150,150,150,150,150,150,150,150,150,150,150,150,150,150,150,150,150,150,150,150,150,150,150,150,150,150,150,150,150,150,150,150,150,150,150,150,150,150,150,150,150,150,150,150,150,150,150,150,150,150,150,150,150,150,150,150,150,150,150,150,150,150,150,150,150,150,150,150,150,150,150,150,150,150,150,150,150,150,150,150,150,150,150,150,150,150,150,150,150,150,150,150,150,150,150,150,150,150,150,150,150,150,150,150,150,150,150,150,150,150,150,150,150,150,150,150,150,150,150,150,150,150,150,150,150,150,150,150,150,150,150,150,150,150,150,150,150,150,150,150,150,150,150,150,150,150,150,150,150,
150,150,150,150,150,150,150,150,150,150,150,150,150,150,150,150,150,150,150,149,148,147,146,145,144,142,141,140,139,138,137,136,135,134,133,132,131,130,129,128 //300 values
               };
int blueList[]  = {  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,2,3,5,6,7,9,10,11,12,14,15,16,18,19,20,22,23,24,25,27,28,29,31,32,33,34,36,37,38,40,41,42,44,45,46,47,49,50,51,53,54,55,57,58,59,60,62,63,64,66,67,68,69,71,72,73,75,76,77,79,80,81,82,84,85,86,88,89,90,92,93,94,95,97,98,99,101,102,103,104,106,107,108,110,111,112,114,115,116,117,119,120,121,123,124,125,127,128,129,130,132,133,134,136,137,138,139,141,142,143,145,146,147,149,150,151,152,154,155,156,158,159,160,162,163,164,165,167,168,169,171,172,173,174,176,177,178,180,181,182,184,185,186,187,189,190,191,193,194,195,197,198,199,200,202,203,204,206,207,208,209,211,
212,213,215,216,217,219,220,221,222,224,225,226,228,229,230,232,233,234,235,236,237,238,240,241,242,243,244,245,246,247,248,249,250,251,252,253,255 //300 values
               };

//-------------------------------------------------------------------------.-------------------------.
//                                                                         | WiFi connection && time |
//-------------------------------------------------------------------------'-------------------------'
ESP8266WebServer server(80);

#ifndef STASSID
#define STASSID ""              //Change this to the name of your home wifi                
#define STAPSK  ""              //Change this to the password of your home wifi
#endif

const char * SSID = STASSID;
const char * PSK = STAPSK;

const long utcOffsetInSeconds = 3600;

char daysOfTheWeek[7][12] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};

// Define NTP Client to get time
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org", utcOffsetInSeconds);

//-------------------------------------------------------------------------.-----------.
//                                                                         | LED Setup |
//-------------------------------------------------------------------------'-----------'

#define LED_PIN     4               //Conect data pin to D2 on ESP8266
#define LED_COUNT  60               //number of LEDs on the strip

Adafruit_NeoPixel strip(LED_COUNT, LED_PIN, NEO_GRB + NEO_KHZ800);

int red   = 0;
int green = 0;
int blue  = 0;

elapsedMillis timeElapsed;
elapsedSeconds LEDBrighter;
//-------------------------------------------------------------------------.------------.
//                                                                         | WiFi Start |
//-------------------------------------------------------------------------'------------'

void setup() {
  Serial.begin(115200);                     // Start the Serial communication to send messages to the computer
  delay(10);
  Serial.println('\n');

  WiFi.mode(WIFI_STA);
  WiFi.begin(SSID, PSK);

  Serial.println("Connecting ...");
  int i = 0;
  while (WiFi.status() != WL_CONNECTED) {   // Wait for the Wi-Fi to connect: scan for Wi-Fi networks, and connect to the strongest of the networks above
  delay(250);
  Serial.print('.');
  }

  Serial.println('\n');
  Serial.print("Connected to ");
  Serial.println(WiFi.SSID());              // Tell us what network we're connected to
  Serial.print("IP address:\t");
  Serial.println(WiFi.localIP());           // Send the IP address of the ESP8266 to the computer

  if (MDNS.begin("esp8266")) {              // Start the mDNS responder for esp8266.local
    Serial.println("mDNS responder started");
  } else {
    Serial.println("Error setting up MDNS responder!");
  }

  MDNS.addService("http", "tcp", 80);

//-------------------------------------------------------------------------.-----------.
//                                                                         | functions |
//-------------------------------------------------------------------------'-----------'

  void handle_Sensor();                   
  void LEDOn();
  void LEDOff();
  void LEDCustom();
  void LEDPreset();
  void Colors();  
  void Alarm();
  void handleNotFound();

//-------------------------------------------------------------------------.-------------------.
//                                                                         | Starting websites |
//-------------------------------------------------------------------------'-------------------'

  server.on("/", handle_main);              //starts the server or websites
  server.on("/LEDOn", LEDOn);
  server.on("/LEDOff", LEDOff);
  server.on("/LEDCustom", LEDCustom);
  server.on("/LEDPreset", LEDPreset);
  server.on("/Colors", Colors);  
  server.on("/Alarm", Alarm);
  server.onNotFound(handleNotFound);        // When a client requests an unknown URI (i.e. something other than "/"), call function "handleNotFound"

  server.begin();                           // Actually start the server
  timeClient.begin();
  
  Serial.println("HTTP server started");

  strip.begin();           // INITIALIZE NeoPixel strip object (REQUIRED)
  strip.show();            // Turn OFF all pixels ASAP

}

//-------------------------------------------------------------------------.------.
//                                                                         | Loop |
//-------------------------------------------------------------------------'------'
void loop() {
//-------------------------------------------------------------------------.---------------.
//                                                                         | Checking time |
//-------------------------------------------------------------------------'---------------'
  if (timeElapsed > interval) {        //Subtracts the current time passed minus the last time this function was activated. If it's longer than 20
                                       //minutes it will get bigger than the interval and trigger the loop
  timeClient.update();

  Serial.print(daysOfTheWeek[timeClient.getDay()]);
  Serial.print(", ");
  Serial.print(timeClient.getHours());
  Serial.print(":");
  
  hourly = timeClient.getHours();
  
  Serial.print(timeClient.getMinutes());
  Serial.print(":");
  Serial.println(timeClient.getSeconds());
  Serial.println(timeClient.getFormattedTime());
  timeElapsed = 0;                      
  }

  if (LEDBrighter > timersun && hourly == 6 && alarm == true){   
    colorindex++;                             //increases index by one so the function takes the next value from the lists above
      if (colorindex < 300){                  //stops at 300 because there are only 300 values in a list
      red    = redList[colorindex];           //takes the colorindex number and applies the RGB value that is indexed by this number
      green  = greenList[colorindex];
      blue   = blueList[colorindex];   
        for (i=0; i<LED_COUNT; i++){   
        strip.setPixelColor(i, strip.Color(red, green, blue));
        strip.show();
        LEDBrighter = 0;
      }
    }
  }
  

  MDNS.update();
  server.handleClient();                    // Listen for HTTP requests from clients

}

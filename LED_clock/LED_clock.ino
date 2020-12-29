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
//                                                                         | Variablen |
//-------------------------------------------------------------------------'-----------'

const int interval = 1200000; //20 minutes
const int timersun = 6000;    //6 seconds

int i= 1;

bool alarm = false;

//-------------------------------------------------------------------------.-------------------------.
//                                                                         | Allgemeine HTML Befehle |
//-------------------------------------------------------------------------'-------------------------'
String AnfangA  =   "<!DOCTYPE html> <html>\n"
                    "<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0, user-scalable=no\">\n"                  //CSS for cosmetics
                    "<style>"     ".header1  { font-size: 40pt; text-align: center;}"
                    ".button1  { background-color: #00FF00; text-align: center; border: 2px solid black; color: black; padding: 25px 40px;}"
                    ".button2  { background-color: #DF0101; text-align: center; border: 2px solid black; color: black; padding: 25px 40px;}"
                    ".div1     { text-align:center;}"
                    "</style>";
                    
String AnfangB  =   "<body>\n";
                    
String Ende     =   "</body>\n"
                    "</html>\n";
//-------------------------------------------------------------------------.--------------------.
//                                                                         | Start Website HTML |
//-------------------------------------------------------------------------'--------------------'

String  StartWebsite() {

  String  ptr = AnfangA;

  ptr += "<title>Wlan LED Steuerung</title>\n";

  ptr += AnfangB;

  ptr += "<h1 class=\"header1\">Wlan LED Steuerung</h1>\n";

  ptr += "<div class = div1>" ;        
  ptr += "<p><a href=\"LEDCustom\"><button class=\"button button1\"> LED On </button>";                       //If you press the button "LED On" you'll be redirected to the void LEDCustom
  ptr += "<p><a href=\"LEDOff\"><button class=\"button button2\"> LED Off </button><br><br><br><br><br>";     //If you press the button "LED Off" you'll be redirected to the void LEDOff
  
  ptr += "<form action = \"Alarm\" method=\"get\">";                                  
  ptr += "<label class=\"toggle\">";
  ptr += "<input type=\"checkbox\" name = \"tickthebox\" value = \"ticked\">";
  ptr += "<input type=\"submit\" value= \"Apply\"></form>";
  ptr += "</div>";

  ptr += Ende;
  return ptr;
}

//-------------------------------------------------------------------------.----------------------.
//                                                                         | Start Website 2 HTML |
//-------------------------------------------------------------------------'----------------------'

String SecondWebsite(){
    
  String  ptr = AnfangA;

  ptr += "<title>Setting LED color</title>\n";

  ptr += AnfangB;

  ptr += "<h1 class=\"header1\">Setting LED color</h1>\n";

  ptr += "<div class = div1>" ;

  ptr += "<form action=\"LEDOn\" method = \"post\">";
  
  ptr += "<label for=\"RedValue\">Red Value:</label>";                                                           //Name next to the slider
  ptr += "<input type = \"range\" min = \"0\" max = \"255\" value = \"0\" name = \"RedValue\"><br><br>";         //initializes the sliders to set RGB values and naming it accordingly
  
  ptr += "<label for=\"GreenValue\">Green Value:</label>";
  ptr += "<input type = \"range\" min = \"0\" max = \"255\" value = \"0\" name = \"GreenValue\"><br><br>";
  
  ptr += "<label for=\"BlueValue\">BlueValue:</label>";
  ptr += "<input type = \"range\" min = \"0\" max = \"255\" value = \"0\" name = \"BlueValue\"><br><br>";

  ptr += "</label>";
  
  ptr += "<input type=\"submit\" value=\"Submit\">";                                      //submits the slider values to the void LEDOn
  ptr += "</form>";

  ptr += "</div>";

  ptr += Ende;
  return ptr;
}
//-------------------------------------------------------------------------.----------------------------.
//                                                                         | WiFi verbindung && Uhrzeit |
//-------------------------------------------------------------------------'----------------------------'
ESP8266WebServer server(80);

#ifndef STASSID
#define STASSID "WIFI name"              //Change this to the name of your home wifi                
#define STAPSK  "WIFI password"    //Change this to the password of your home wifi
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
#define LED_COUNT  60               //amount of LEDS in strip

Adafruit_NeoPixel strip(LED_COUNT, LED_PIN, NEO_GRB + NEO_KHZ800);

int red = 0;
int green = 0;
int blue = 0;

elapsedMillis timeElapsed;
elapsedSeconds LEDBrighter;
elapsedSeconds Checktimeagain;
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

//-------------------------------------------------------------------------.------------.
//                                                                         | Funktionen |
//-------------------------------------------------------------------------'------------'

  void handle_Sensor();                   
  void LEDOn();
  void LEDCustom();
  void LEDOff();
  void Alarm();
  void handleNotFound();

//-------------------------------------------------------------------------.----------------.
//                                                                         | Seiten starten |
//-------------------------------------------------------------------------'----------------'

  server.on("/", handle_main);              //starts the server or websites
  server.on("/LEDOn", LEDOn);
  server.on("/LEDCustom", LEDCustom);
  server.on("/LEDOff", LEDOff);
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

  int redList[]   =   {0, 4, 8, 13, 17, 21, 26, 30, 35, 39, 43, 48, 52, 57, 61, 65, 70, 74, 79, 83, 87, 92, 96, 101, 105, 109, 114, 118, 123, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 126, 126, 126, 126, 125, 125, 125, 124, 124, 124, 123, 123, 123, 123, 122, 122, 122, 121, 121, 121, 121, 120, 120, 120, 119, 119, 119, 118, 118, 118, 118, 117, 117, 117, 116, 116, 116, 116, 115, 115, 115, 114, 114, 114, 114, 113, 113, 113, 112, 112, 112, 111, 111, 111, 111, 110, 110, 110, 109, 109, 109, 109, 108, 108, 108, 107, 107, 107, 106, 106, 106, 106, 105, 105, 105, 104, 104, 104, 104, 103, 103, 103, 102, 102, 102, 101, 101, 101, 101, 100, 100, 100, 99, 99, 99, 99, 98, 98, 98, 97, 97, 97, 96, 96, 96, 96, 95, 95, 95, 94, 94, 94, 94, 93, 93, 93, 92, 92, 92, 92, 91, 91, 91, 90, 90, 90, 89, 89, 89, 89, 88, 88, 88, 87, 87, 87, 87, 86, 86, 86, 85, 85, 85, 84, 84, 84, 84, 83, 83, 83, 82, 82, 82, 82, 81, 81, 81, 80, 80, 80, 79, 79, 79, 79, 78, 78, 78, 77, 77, 77, 77, 76, 76, 76, 75, 75, 75, 75,
                    74, 74, 73, 73, 72, 72, 71, 70, 70, 69, 69, 68, 68, 67, 67, 66, 66, 65, 65, 64, 64};    //300 values

  int greenList[] =   {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 62, 64, 64, 65, 66, 67, 68, 70, 71, 72, 73, 75, 75, 75, 75, 75, 75, 75, 75, 75, 75, 75, 75, 75, 75, 75, 75, 75, 75, 75, 75, 75, 75, 75, 75, 75, 75, 75, 75, 75, 75, 75, 75, 75, 75, 75, 75, 75, 75, 75, 75, 75, 75, 75, 75, 75, 75, 75, 75, 75, 75, 75, 75, 75, 75, 75, 75, 75, 75, 75, 75, 75, 75, 75, 75, 75, 75, 75, 75, 75, 75, 75, 75, 75, 75, 75, 75, 75, 75, 75, 75, 75, 75, 75, 75, 75, 75, 75, 75, 75, 75, 75, 75, 75, 75, 75, 75, 75, 75, 75, 75, 75, 75, 75, 75, 75, 75, 75, 75, 75, 75, 75, 75, 75, 75, 75, 75, 75, 75, 75, 75, 75, 75, 75, 75, 75, 75, 75, 75, 75, 75, 75, 75, 75, 75, 75, 75, 75, 75, 75, 75, 75, 75, 75, 75, 75, 75, 75, 75, 75, 75, 75, 75, 75, 75, 75, 75, 75, 75, 75, 75, 75, 75, 75, 75,
                    75, 75, 75, 75, 75, 75, 75, 75, 75, 75, 75, 75, 75, 75, 75, 75, 74, 74, 73, 73, 72, 72, 71, 70, 70, 69, 69, 68, 68, 67, 67, 66, 66, 65, 65, 64, 64};    //300 values

  int blueList[]  =   {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 2, 3, 3, 4, 5, 5, 6, 7, 7, 8, 9, 9, 10, 11, 11, 12, 12, 13, 14, 14, 15, 16, 16, 17, 18, 18, 19, 20, 20, 21, 22, 22, 23, 23, 24, 25, 25, 26, 27, 27, 28, 29, 29, 30, 31, 31, 32, 33, 33, 34, 34, 35, 36, 36, 37, 38, 38, 39, 40, 40, 41, 42, 42, 43, 44, 44, 45, 46, 46, 47, 47, 48, 49, 49, 50, 51, 51, 52, 53, 53, 54, 55, 55, 56, 57, 57, 58, 58, 59, 60, 60, 61, 62, 62, 63, 64, 64, 65, 66, 66, 67, 68, 68, 69, 69, 70, 71, 71, 72, 73, 73, 74, 75, 75, 76, 77, 77, 78, 79, 79, 80, 81, 81, 82, 82, 83, 84, 84, 85, 86, 86, 87, 88, 88, 89, 90, 90, 91, 92, 92, 93, 93, 94, 95, 95, 96, 97, 97, 98, 99, 99, 100, 101, 101, 102, 103, 103, 104, 104, 105, 106, 106, 107, 108, 108, 109, 110, 110, 111, 112, 112, 113, 114, 114, 115, 116,
                    116, 117, 117, 118, 118, 119, 120, 120, 121, 121, 122, 122, 123, 123, 124, 124, 125, 125, 126, 126, 127};   //300 values

  int colorindex = 0; //index number for the color lists
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
  Serial.print(timeClient.getMinutes());
  Serial.print(":");
  Serial.println(timeClient.getSeconds());
  Serial.println(timeClient.getFormattedTime());
  timeElapsed = 0;                      
  }

  if (LEDBrighter > timersun && timeClient.getHours() == 6 && alarm == true){   
    colorindex++;                             //increases index by one so the function takes the next value from the lists above
      if (colorindex < 300 ){                 //stops at 300 because there are only 300 values in a list
      red    = redList[colorindex];           //takes the colorindex number and applies the RGB value that is indexed by this number
      green  = greenList[colorindex];
      blue   = blueList[colorindex];   
        for (i=0; i<LED_COUNT; i++){   
        strip.setPixelColor(3, strip.Color(red, green, blue));
        strip.show();
        LEDBrighter = 0;
      }
    }
  }
  

  MDNS.update();
  server.handleClient();                    // Listen for HTTP requests from clients

}

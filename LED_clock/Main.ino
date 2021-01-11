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
    
int color [] = {0x80000, 0x110000, 0x1a0000, 0x230000, 0x2b0000, 0x340000, 0x3d0000, 0x460000, 0x4f0000, 0x570000, 0x600000, 0x690000, 0x720000, 0x7b0000, 0x830000, 0x8c0000, 0x950000, 0x9e0000, 0xa70000, 0xaf0000, 0xb80000, 0xc10000, 0xca0000, 0xd30000, 0xdb0000, 0xe40000, 0xed0000, 0xf60000, 0xff0000, 0xff0000, 0xff0200, 0xff0400, 0xff0600, 0xff0800, 0xff0a00, 0xff0d00, 0xff0f00, 0xff1100, 0xff1300, 0xff1500, 0xff1700, 0xff1a00, 0xff1c00, 0xff1e00, 0xff2000, 0xff2200, 0xff2400, 0xff2700, 0xff2900, 0xff2b00, 0xff2d00, 0xff2f00, 0xff3100, 0xff3400, 0xff3600, 0xff3800, 0xff3a00, 0xff3c00, 0xff3e00, 0xff4100, 0xff4300, 0xff4500, 0xff4700, 0xff4900, 0xff4b00, 0xff4e00, 0xff5000, 0xff5200, 0xff5400, 0xff5600, 0xff5800, 0xff5b00, 0xff5d00, 0xff5f00, 0xff6100, 0xff6300, 0xff6500, 0xff6800, 0xff6a00, 0xff6c00, 0xff6e00, 0xff7000, 0xff7200, 0xff7500, 0xff7700, 0xff7900, 0xff7b00, 0xff7d00, 0xff8000, 0xff8000, 0xff8200, 0xff8400, 0xff8700, 0xff8900, 0xff8c00, 0xff8e00, 0xff9100, 0xff9300, 0xff9600, 0xfe9601, 0xfd9602, 0xfd9603, 0xfc9605, 0xfc9606, 0xfb9607, 0xfa9609, 0xfa960a, 0xf9960b, 0xf9960c, 0xf8960e, 0xf7960f, 0xf79610, 0xf69612, 0xf69613, 0xf59614, 0xf59616, 0xf49617, 0xf39618, 0xf39619, 0xf2961b, 0xf2961c, 0xf1961d, 0xf0961f, 0xf09620, 0xef9621, 0xef9622, 0xee9624, 0xed9625, 0xed9626, 0xec9628, 0xec9629, 0xeb962a, 0xeb962c, 0xea962d, 0xe9962e, 0xe9962f, 0xe89631, 0xe89632, 0xe79633, 0xe69635, 0xe69636, 0xe59637, 0xe59639, 0xe4963a, 0xe4963b, 0xe3963c, 0xe2963e, 0xe2963f, 0xe19640, 0xe19642, 0xe09643, 0xdf9644, 0xdf9645, 0xde9647, 0xde9648, 0xdd9649, 0xdc964b, 0xdc964c, 0xdb964d, 0xdb964f, 0xda9650, 0xda9651, 0xd99652, 0xd89654, 0xd89655, 0xd79656, 0xd79658, 0xd69659, 0xd5965a, 0xd5965c, 0xd4965d, 0xd4965e, 0xd3965f, 0xd39661, 0xd29662, 0xd19663, 0xd19665, 0xd09666, 0xd09667, 0xcf9668, 0xce966a, 0xce966b, 0xcd966c, 0xcd966e, 0xcc966f, 0xcb9670, 0xcb9672, 0xca9673, 0xca9674, 0xc99675, 0xc99677, 0xc89678, 0xc79679, 0xc7967b, 0xc6967c, 0xc6967d, 0xc5967f, 0xc49680, 0xc49681, 0xc39682, 0xc39684, 0xc29685, 0xc19686, 0xc19688, 0xc09689, 0xc0968a, 0xbf968b, 0xbf968d, 0xbe968e, 0xbd968f, 0xbd9691, 0xbc9692, 0xbc9693, 0xbb9695, 0xba9696, 0xba9697, 0xb99698, 0xb9969a, 0xb8969b, 0xb8969c, 0xb7969e, 0xb6969f, 0xb696a0, 0xb596a2, 0xb596a3, 0xb496a4, 0xb396a5, 0xb396a7, 0xb296a8, 0xb296a9, 0xb196ab, 0xb096ac, 0xb096ad, 0xaf96ae, 0xaf96b0, 0xae96b1, 0xae96b2, 0xad96b4, 0xac96b5, 0xac96b6, 0xab96b8, 0xab96b9, 0xaa96ba, 0xa996bb, 0xa996bd, 0xa896be, 0xa896bf, 0xa796c1, 0xa796c2, 0xa696c3, 0xa596c5, 0xa596c6, 0xa496c7, 0xa496c8, 0xa396ca, 0xa296cb, 0xa296cc, 0xa196ce, 0xa196cf, 0xa096d0, 0x9f96d1, 0x9f96d3, 0x9e96d4, 0x9e96d5, 0x9d96d7, 0x9d96d8, 0x9c96d9, 0x9b96db, 0x9b96dc, 0x9a96dd, 0x9a96de, 0x9996e0, 0x9896e1, 0x9896e2, 0x9796e4, 0x9796e5, 0x9696e6, 0x9696e8, 0x9595e9, 0x9494ea, 0x9393eb, 0x9292ec,0x9191ed,0x9090ee,0x8e8ef0,0x8d8df1,0x8c8cf2,0x8b8bf3,0x8a8af4,0x8989f5,0x8888f6,0x8787f7,
0x8686f8,0x8585f9,0x8484fa,0x8383fb,0x8282fc,0x8181fd,0x8080ff
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
#define LED_COUNT  10               //number of LEDs on the strip

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
      if (colorindex < 299){                  //stops at 299 because there are only 299 values in a list
        red   = (color[colorindex] >> 16) & 0xFF;
        green = (color[colorindex] >> 8) & 0xFF;
        blue  =  color[colorindex] & 0xFF;           //takes the colorindex number and applies the RGB value that is indexed by this number
        for (i=0; i<LED_COUNT; i++){   
        strip.setPixelColor(i, strip.Color(red,green,blue));
        strip.show();
        LEDBrighter = 0;
      }
    }
  }
  

  MDNS.update();
  server.handleClient();                    // Listen for HTTP requests from clients

}

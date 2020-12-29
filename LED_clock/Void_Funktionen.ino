//-------------------------------------------------------------------------.----------.
//                                                                         | Websites |
//-------------------------------------------------------------------------'----------'
//-------------------------------------------------------------------------.--------------.
//                                                                         | Main Website |
//-------------------------------------------------------------------------'--------------'

void handle_main() {
  server.send(200, "text/html", StartWebsite());
}
//-------------------------------------------------------------------------.-------------.
//                                                                         | LED Website |
//-------------------------------------------------------------------------'-------------'

void LEDOn() { 
 
  String RedValue    = server.arg("RedValue");            //takes the submitted values from the sliders and converts it into a string
  String GreenValue  = server.arg("GreenValue");
  String BlueValue   = server.arg("BlueValue");
  
  int  RedVal   = RedValue.toInt();                      //converts string to integer
  int  GreenVal = GreenValue.toInt();
  int  BlueVal  = BlueValue.toInt();
  for (i=0; i<LED_COUNT; i++){   
        strip.setPixelColor(i, strip.Color(RedVal, GreenVal, BlueVal));
        strip.show();
      }  
  handle_main();
}


void LEDOff() {
  
  for (i=0; i<LED_COUNT; i++){
    strip.setPixelColor(i, strip.Color(0, 0, 0)); 
    strip.show();   //Turns the LED off and returns that value to the void "handle_Sensor"  
  }
  
  alarm = false;    //also sets the alarm to false, will deactivate the sunrise
  handle_main();
}


void LEDCustom(){
  
  server.send(200, "text/html", SecondWebsite());
 
  }


void Alarm(){

    if(server.hasArg("tickthebox")){
      if (server.arg("tickthebox")=="ticked") {
        alarm = true;                             //checks if the checkbox is checked, if it is it sets the alarm to true
        server.send(200, "text/html", StartWebsite());
      }       
   }
}

//-------------------------------------------------------------------------.----------------.
//                                                                         | Fehler Meldung |
//-------------------------------------------------------------------------'----------------'
void handleNotFound() {
  
  server.send(404, "text/plain", "404: Not found");
}

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

void LEDPreset(){
  server.send(200, "text/html", ThirdWebsite());
  
}

void Colors(){

      if(server.hasArg("tickthebox")){
        if (server.arg("tickthebox")=="1") {
          for (i=0; i<LED_COUNT; i++){
            strip.setPixelColor(i, strip.Color(229, 83, 0)); 
            strip.show();   
            }
        } 
        
        if (server.arg("tickthebox")=="2") {
          for (i=0; i<LED_COUNT; i++){
            strip.setPixelColor(i, strip.Color(69,179,224)); 
            strip.show();     
            }
        }
        
        if (server.arg("tickthebox")=="3") {
          for (i=0; i<LED_COUNT; i++){
            strip.setPixelColor(i, strip.Color(255,255,0)); 
            strip.show();     
            }
        }

        if (server.arg("tickthebox")=="4") {
          for (i=0; i<LED_COUNT; i++){
            strip.setPixelColor(i, strip.Color(255,182,193)); 
            strip.show();     
            }
        }
        server.send(200, "text/html", StartWebsite());                             
 }  
}


void Alarm(){

    if(server.hasArg("tickthebox")){
      if (server.arg("tickthebox")=="ticked") {
        alarm = true;                         
        server.send(200, "text/html", StartWebsite());
      }       
   }
}

//-------------------------------------------------------------------------.-------.
//                                                                         | Error |
//-------------------------------------------------------------------------'-------'
void handleNotFound() {
  
  server.send(404, "text/plain", "404: Not found");
}

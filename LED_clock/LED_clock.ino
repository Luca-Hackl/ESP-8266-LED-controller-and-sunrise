String AnfangA  =   "<!DOCTYPE html> <html>\n"
                    "<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0, user-scalable=no\">\n"                  //CSS for cosmetics
                    "<style>"     

".myDiv {"
  "text-align: center;"
  "width: 100%;"
  "justify-content: center;"
  "align-items: center;"
"}"

"body {"

  "height: 93vh;" 
  "background: #222;"
  "color: #eee;"
  "font-family: \"Dosis\", sans-serif;"
"}"

".underlined-a {"
  "text-decoration: none;"
  "color: aqua;"
  "padding-bottom: 0.15em;"
  "box-sizing: border-box;"
  "box-shadow: inset 0 -0.2em 0 aqua;"
  "transition: 0.2s;"
"}"

".brk-btn {"
  "position: relative;"
  "background: none;"
  "color: aqua;"
  "width: 100px;"
  "text-align: center;"
  "text-transform: uppercase;"
  "text-decoration: none;"
  "border: 0.2em solid aqua;"
  "padding: 0.5em 1em;"
  "box-sizing: border-box;"
"}"


"input[type=submit] {"
    "position: relative;"
    "background: none;"
    "color: aqua;"
    "width: 100px;"
    "text-align: center;"
    "text-transform: uppercase;"
    "text-decoration: none;"
    "border: 0.2em solid aqua;"
    "padding: 0.5em 1em;"
    "cursor: pointer;"
    "padding-bottom: 0.15em;"
    "box-sizing: border-box;"
"}"

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

  ptr +=  "<div class=\"myDiv\">";
  ptr += "<h1 class=\"header1\">Wlan LED Steuerung</h1>\n";

  ptr +=  "<link href=\"https://fonts.googleapis.com/css?family=Dosis:700\" rel=\"stylesheet\">";
  ptr +=  "<br><br><br><br>";
  ptr +=  "<a href=\"LEDCustom\" class=\"brk-btn\">";
  ptr +=    "LED On";
  ptr +=  "</a><br><br><br>";
  ptr +=  "<a href=\"LEDOff\" class=\"brk-btn\">";
  ptr +=    "LED Off";
  ptr +=  "</a><br><br><br>";

  ptr += "<form action = \"Alarm\" method=\"get\">";                                  
  ptr += "<label class=\"toggle\">";
  ptr += "<input type=\"checkbox\" name = \"tickthebox\" value = \"ticked\">";
  ptr += "<input type=\"submit\"  value = \"Apply\"></form> ";

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

  ptr += "<div class = myDiv>" ;

  ptr += "<h1 class=\"header1\">Setting LED color</h1>\n";

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

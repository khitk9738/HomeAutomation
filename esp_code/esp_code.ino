#include <ESP8266WiFi.h>
#include"FS.h"
#include "Adafruit_MQTT.h"
#include "Adafruit_MQTT_Client.h"

const char* ssid = "ppiiyy";                                                         // SSID of the hotspot of the device which will be connected to the Wi-Fi Module
//const char* password = "Passwordiseasy";                                             // Password of the hotspot of the device which will be connected to the Wi-Fi Module
//fb
#define AIO_SERVER      "io.adafruit.com"
#define AIO_SERVERPORT  1883                   // use 8883 for SSL
#define AIO_USERNAME    "aks60507"
#define AIO_KEY         "f3643ce6b933402b86759ff40a915143"
const char* host = "maker.ifttt.com";
const int httpsPort = 443;
WiFiClient client;
// Setup the MQTT client class by passing in the WiFi client and MQTT server and login details.
Adafruit_MQTT_Client mqtt(&client, AIO_SERVER, AIO_SERVERPORT, AIO_USERNAME, AIO_KEY);

Adafruit_MQTT_Subscribe LAMPbutton = Adafruit_MQTT_Subscribe(&mqtt, AIO_USERNAME "/feeds/LAMP ON");
Adafruit_MQTT_Subscribe FANbutton = Adafruit_MQTT_Subscribe(&mqtt, AIO_USERNAME "/feeds/Fan ON");
Adafruit_MQTT_Subscribe SOCKETbutton = Adafruit_MQTT_Subscribe(&mqtt, AIO_USERNAME "/feeds/Socket ON");
void MQTT_connect();

 
int lamp = D4;   
int fan = D5;
int socket = D6;
int inter_pin=D7;
int fan_count=0;  //ts
int water_count=0;//check
int light_count=0;//tr
int person_count=0;//tr
int water_mode=0;
int door_mode=0;
int alarm_count=0;
int temp_val=30;//tr
int humid_val=60;//tr
int door_automatic=0;//ts

int g_0=0;//tr
int g_1=0;
int g_2=0;
int g_3=0;
int g_4=0;
int g_5=0;//tr
String send_string="";
String receive_string="";
WiFiServer server(80);
 String hr="";
 String mi="";
 String sec="";
void setup() {
  Serial.begin(9600);
  delay(10);
  Serial.println(F("Adafruit MQTT demo"));
  pinMode(lamp, OUTPUT);                                                         // defining the mode of the pin of the Arduino Uno Microcontroller
  digitalWrite(lamp, LOW);                                                       // providing the initial value to the pin i.e. logic LOW
  pinMode(fan, OUTPUT);
  analogWrite(fan, 0);
  pinMode(socket, OUTPUT);                                                      
  digitalWrite(socket , LOW);                                                                                             
  pinMode(inter_pin, OUTPUT);                                                      
  digitalWrite(inter_pin , LOW);
  //Serial.println();
  //Serial.println();
  //Serial.print("Connecting to ");                                                   // Connecting to WiFi network
  //Serial.println(ssid);                                                             // prints the SSID of the hotspot in the serial monitor 
  WiFi.begin(ssid);                                                       
 
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);                                                                      //check whether the client is connected or not,if not connected then print "." else wait for 500ms
    //Serial.print(".");
  }
  //Serial.println("");
  //Serial.println("WiFi connected");                                                  //print the status of the connection in the serial monitor
 
  server.begin();                                                                    
  //Serial.println("Server started");                                                  // Start the server when the connection is finally established
 
 // Serial.print("Use this URL to connect: ");
  //Serial.print("http://");                                                           // prints the IP address of the Wi-Fi Module(server)
  //Serial.print(WiFi.localIP());
  //Serial.println("/");

  //Serial.println(tim);

  // Setup MQTT subscription for onoff feed.
  mqtt.subscribe(&LAMPbutton);
  mqtt.subscribe(&FANbutton);
  mqtt.subscribe(&SOCKETbutton);
  //Serial.print("connecting to ");
  //Serial.println(host);
  if (!client.connect(host, httpsPort)) {
    //Serial.println("connection failed");
    return;
  }

  String url = "/trigger/esp8266/with/key/bcN4X98JEG7N0jMhqXIMVC";
//With an optional JSON body of:
 // Serial.print("requesting URL: ");
 // Serial.println(url);

  client.print(String("GET ") + url + " HTTP/1.1\r\n" +
               "Host: " + host + "\r\n" +
               "User-Agent: BuildFailureDetectorESP8266\r\n" +
               "Connection: close\r\n\r\n");

  //Serial.println("request sent");
  while (client.connected()) {
    String line = client.readStringUntil('\n');
    if (line == "\r") {
      //Serial.println("headers received");
      break;
    }
  }
  String line = client.readStringUntil('\n');

  
  //Serial.println("closing connection");
  
}

void loop()
{
  if(Serial.available())
  {
   
   char c=(char)Serial.read();
    
    if((int)c>=48 && (int)c<=57)
    {
       if(x==0)  water_level=(int)c-48;
       if(x==1)fire=(int)c-48;
       if(x==2) g_0=(int)c-48;
       if(x==3) g_1=(int)c-48;
       if(x==4) g_2=(int)c-48;
       if(x==5) g_3=(int)c-48;
       if(x==6) g_4=(int)c-48;
       if(x==7) g_5=(int)c-48;
       if(x==8) temp_val1=(int)c-48;
       if(x==9) temp_val2=(int)c-48;
       if(x==10) humid_val=(int)c-48;
       if(x==11) humid_val2=(int)c-48;
       if(x==12) person_count=(int)c-48;
       x++;
       
      }
      temp_val=temp_val1*10+temp_val2;
      humid_val=humid_val1*10+humid_val2;
      if(x==13) {x=0;temp_val=temp_val1*10+temp_val2;
      humid_val=humid_val1*10+humid_val2;}
      
  }
  
   fb();
   httml();
  }
void fb()
{
  
   MQTT_connect();
  Adafruit_MQTT_Subscribe *subscription;
  while ((subscription = mqtt.readSubscription(5000))) {
    if (subscription == &LAMPbutton) {
      Serial.print(F("LAMP status : "));
      Serial.println((char *)LAMPbutton.lastread);
      uint16_t state=atoi((char *)LAMPbutton.lastread);
      digitalWrite(lamp,state);
      digitalWrite(inter_pin,HIGH);
    delay(10);
    digitalWrite(inter_pin,LOW);
    }
    else if (subscription == &FANbutton) {
      Serial.print(F("FAN status: "));
      Serial.println((char *)FANbutton.lastread);
      uint16_t state=atoi((char *)FANbutton.lastread);
      digitalWrite(fan,state);
      digitalWrite(inter_pin,HIGH);
    delay(10);
    digitalWrite(inter_pin,LOW);
    }
    else if (subscription == &SOCKETbutton) {
      Serial.print(F("SOCKET status: "));
      Serial.println((char *)SOCKETbutton.lastread);
      uint16_t state=atoi((char *)SOCKETbutton.lastread);
      digitalWrite(socket,state);
      digitalWrite(inter_pin,HIGH);
    delay(10);
    digitalWrite(inter_pin,LOW);
    }
  }
}

void MQTT_connect() {
  int8_t ret;
  // Stop if already connected.
  if (mqtt.connected()) {
    return;
  }

  Serial.print("Connecting to MQTT... ");

  uint8_t retries = 3;
  while ((ret = mqtt.connect()) != 0) { // connect will return 0 for connected
       Serial.println(mqtt.connectErrorString(ret));
       Serial.println("Retrying MQTT connection in 5 seconds...");
       mqtt.disconnect();
       delay(5000);  // wait 5 seconds
       retries--;
       if (retries == 0) {
         // basically die and wait for WDT to reset me
         while (1);
       }
  }
  Serial.println("MQTT Connected!");
}
  
void httml()
{
  WiFiClient client = server.available();                                          // Check if a client has connected. If connected then return else continue searching.
  if (!client)
  {
    return;
  }
 
 // Serial.println("new client");
  while(!client.available())                                                        // function is used to Wait until the client sends some data
  {
    delay(1);
  }
  
  String request = client.readStringUntil('\r');                                    // Read the first line of the request
  //Serial.println(request);
  //String tim=getTime();  
  //hr=tim.substring(17,19);
  //mi=tim.substring(20,22);
  //sec=tim.substring(23,25);
  client.flush();
  
  int value1 = LOW;
  if (request.indexOf("/LAMP=ON") != -1)                                             // Match the request to on the device
  {
    digitalWrite(lamp, HIGH);                                                    // Set ledPin according to the request
    digitalWrite(inter_pin,HIGH);
    delay(10);
    digitalWrite(inter_pin,LOW);
    value1 = HIGH;
  }
  if (request.indexOf("/LAMP=OFF") != -1)                                             //Match the request to off the device
  {
    digitalWrite(lamp, LOW);
    digitalWrite(inter_pin,HIGH);
    delay(10);
    digitalWrite(inter_pin,LOW);// Set ledPin according to the request
    value1 = LOW;
  }


  int value2 = LOW;
  if (request.indexOf("/FAN=ON") != -1)                                             //Match the request to on the fan
  {
    switch(fan_count)
    {
      case 0: {analogWrite(fan,0);break;}
      case 1: {analogWrite(fan,100);break;}
      case 2: {analogWrite(fan,200);break;}
      case 3: {analogWrite(fan,255);break;}
      }                                                     // Set ledPin according to the request
    digitalWrite(inter_pin,HIGH);
    delay(10);
    digitalWrite(inter_pin,LOW);
    value2 = HIGH;
  }
  if (request.indexOf("/FAN=OFF") != -1)                                             //Match the request to off the fan
  {
    analogWrite(fan, 0);                                                       // Set ledPin according to the request
    digitalWrite(inter_pin,HIGH);
    delay(10);
    digitalWrite(inter_pin,LOW);
    value2 = LOW;
  }


  int value3 = LOW;
  if (request.indexOf("/SOCKET=ON") != -1)                                             //Match the request to on the socket 
  {
    digitalWrite(socket, HIGH);                                                       // Set ledPin according to the request
    digitalWrite(inter_pin,HIGH);
    delay(10);
    digitalWrite(inter_pin,LOW);
    value3 = HIGH;
  }
  if (request.indexOf("/SOCKET=OFF") != -1)                                            //Match the request to off the socket
  {
    digitalWrite(socket, LOW);                                                        // Set ledPin according to the request
    digitalWrite(inter_pin,HIGH);
    delay(10);
    digitalWrite(inter_pin,LOW);
    value3 = LOW;
  }

  if(request.indexOf("/SPEED=INCREASE")!=-1  && fan_count<3)
  {
     fan_count++;
    }
  if(request.indexOf("/SPEED=DECREASE")!=-1  && fan_count>0)
  {
     fan_count--;
    }
  if (request.indexOf("/AUTOMATIC") != -1)  
  {
     door_automatic=(++door_automatic)%2;
    }
    if (request.indexOf("/MANUAL") != -1)
    {
      water_mode=0;
      }

    if (request.indexOf("/RANGE_LIMIT") != -1)
    {
      water_mode=1;
      }

    if (request.indexOf("/AUTO") != -1)
    {
      water_mode=2;
      }
    
// Set ledPin according to the request
//digitalWrite(ledPin, value);
 
  client.println("HTTP/1.1 200 OK");                                                     // Return the response
  client.println("Content-Type: text/html");                                             // start printing the HTML code in the client web browser
  client.println(""); 
String html ="<!DOCTYPE html> <html> <title>Circuit Magicians</title> <meta charset=\"UTF-8\"> <meta name=\"viewport\" content=\"width=device-width, initial-scale=1\"> <link rel=\"stylesheet\" href=\"https://www.w3schools.com/w3css/4/w3.css\"> <link rel=\"stylesheet\" href=\"https://fonts.googleapis.com/css?family=Raleway\"> <script src=\"https://ajax.googleapis.com/ajax/libs/jquery/3.3.1/jquery.min.js\"></script> <style> body,h1,h2,h3,h4,h5 {font-family: \"Raleway\", sans-serif} .w3-quarter img{margin-bottom: -6px; cursor: pointer} .w3-quarter img:hover{opacity: 0.6; transition: 0.3s} .dropdown-container { display: none; font-style:italic; text-align:center; } .w3-row-padding img {margin-bottom: 12px} .w3-row-padding img {margin-bottom: 12px} .lefto{width:100%;height:1000px;background-color: black;position: fixed;z-index:150;left:0;border-left: 10px white dashed;border-top: 10px white dashed;border-down: 10px white dashed;border-right: 10px white dashed;transition:all 0.5s ease-out;} .leftoo{width:0%;height:1000px;background-color: black;position: fixed;z-index:-5;left:0;border-left: 0px white dashed;border-right: 0px white dashed;transition:all 0.5s ease-out;} .open{position: fixed;width:150px;height:150px;border-radius: 75px;margin-top: 260px;left: 50%;;margin-left:-75px;background: url('https://i.imgur.com/MgMQjY5.png');background-color:black;background-size:100% 100%;z-index: 160;box-shadow: 0px 0px 50px white;transition:all 0.5s ease-in-out;} .openo{position: fixed;width:75px;height:75px;border-radius: 75px;left:50%;margin-top: -200px;margin-left:-35px;background: url('https://i.imgur.com/MgMQjY5.png');background-color:black;background-size:100% 100%;transform: rotate(360deg);z-index: -30;box-shadow: 0px 0px 50px white;transition:all 0.5s ease-in-out 0.1s;} #jsalarmclock{ font-family: Tahoma; font-weight: bold; font-size: 12px; } #jsalarmclock div{ margin-bottom: 0.8em; } #jsalarmclock div.leftcolumn{ float: left; width: 20%; font-size: 13px; clear: left; } #jsalarmclock span{ margin-right: 5px; } .button_menu { background-color:#008CBA; border: none; color: white; padding: 20px; text-align: center; text-decoration: none; display: inline-block; font-size: 16px; margin: 4px 2px; cursor: pointer; border-radius: 50%; } .button_menu:hover { background-color: white; color: black; } .select_menu { width: 20%; height: 28px; background-color:white; border: 1px solid red; border-radius: 10px; color: blue; } .text_menu { width: 90%; height: 28px; background-color: white; border: 1px solid red; border-radius: 10px; color: blue; } </style>";
client.println(html); 
html ="<script > var jsalarm={ padfield:function(f){ return (f<10)? \"0\"+f : f }, showcurrenttime:function(){ var dateobj=new Date() var ct=this.padfield(dateobj.getHours())+\":\"+this.padfield(dateobj.getMinutes())+\":\"+this.padfield(dateobj.getSeconds()) this.ctref.innerHTML=ct this.ctref.setAttribute(\"title\", ct) if (typeof this.hourwake!=\"undefined\"){ if (this.ctref.title==(this.hourwake+\":\"+this.minutewake+\":\"+this.secondwake)){ clearInterval(jsalarm.timer) window.location=document.getElementById(\"musicloc\").value } } }, init:function(){ var dateobj=new Date() this.ctref=document.getElementById(\"alarm_alarm\") this.submitref=document.getElementById(\"submit_submit\") this.submitref.onclick=function(){ jsalarm.setalarm() this.value=\"Alarm Set\" this.disabled=true return false } this.resetref=document.getElementById(\"reset_reset\") this.resetref.onclick=function(){ jsalarm.submitref.disabled=false jsalarm.hourwake=undefined jsalarm.hourselect.disabled=false jsalarm.minuteselect.disabled=false jsalarm.secondselect.disabled=false return false } var selections=document.getElementsByTagName(\"select\") this.hourselect=selections[0] this.minuteselect=selections[1] this.secondselect=selections[2] for (var i=0; i<60; i++){ if (i<24) this.hourselect[i]=new Option(this.padfield(i), this.padfield(i), false, dateobj.getHours()==i) this.minuteselect[i]=new Option(this.padfield(i), this.padfield(i), false, dateobj.getMinutes()==i) this.secondselect[i]=new Option(this.padfield(i), this.padfield(i), false, dateobj.getSeconds()==i) } jsalarm.showcurrenttime() jsalarm.timer=setInterval(function(){jsalarm.showcurrenttime()}, 1000) }, setalarm:function(){ this.hourwake=this.hourselect.options[this.hourselect.selectedIndex].value this.minutewake=this.minuteselect.options[this.minuteselect.selectedIndex].value this.secondwake=this.secondselect.options[this.secondselect.selectedIndex].value this.hourselect.disabled=true this.minuteselect.disabled=true this.secondselect.disabled=true } } </script> <script> $(function(){ $('.open').on('click', function(){$('.lefto').toggleClass('leftoo');$('.open').toggleClass('openo');});}); </script>";
client.println(html); 
html ="<body class=\"w3-black\"> <div class=\"loader\"></div> <!-- Sidebar/menu --> <nav class=\"w3-sidebar w3-bar-block w3-black w3-animate-right w3-top w3-text-light-grey w3-large\" style=\"z-index:3;width:250px;font-weight:bold;display:none;right:0;\" id=\"sidenav\"> <a href=\"javascript:void()\" onclick=\"w3_close()\" class=\"w3-bar-item w3-button w3-center w3-padding-32\">CLOSE</a> <button class=\"w3-bar-item w3-button w3-center w3-padding-32\">TEAM MEMBERS </button> <div class=\"dropdown-container\"> <button class=\"w3-bar-item w3-button w3-center w3-padding-32\">Piyush Gupta </button> <div class=\"dropdown-container\"> <img src=\"https://i.imgur.com/bbCOEEI.jpg\" width=\"100%\";> </div> <button class=\"w3-bar-item w3-button w3-center w3-padding-32\">Aditya Singh </button> <div class=\"dropdown-container\"> <img src=\"https://i.imgur.com/eLKuzCi.jpg\" width=\"100%\"; > </div> <button class=\"w3-bar-item w3-button w3-center w3-padding-32\">Pragya Pande </button> <div class=\"dropdown-container\"> <img src=\"https://i.imgur.com/04MbEVF.jpg\" width=\"100%\"; > </div> <button class=\"w3-bar-item w3-button w3-center w3-padding-32\">Samita Verma </button> <div class=\"dropdown-container\"> <img src=\"https://i.imgur.com/2QhdOtk.jpg\" width=\"100%\";> </div> </div> <button class=\"w3-bar-item w3-button w3-center w3-padding-32\">ABOUT </button> <div class=\"dropdown-container\"> <p>he Project was built using Arduino and ESP microcontrollers to facilitate the need of the hour-Home Automation through Smartphones Website,Google Assistant & Messenger.</p> </div> <button class=\"w3-bar-item w3-button w3-center w3-padding-32\">APPLICATIONS </button> <div class=\"dropdown-container\"> <p>Person Count</p><p>Water Level Indicator</p><p>Fire Alarm</p><p>Automated Fan Regulation</p><p>Automatic Door Opening </p><p>Realtime Temperature Monitoring</p><p>Energy Efficient Lighting</p><p>Humidity Monitoring System</p> <p>Centralised Alarm System</p><p>Intensity Varying Lighting</p> </div> <button class=\"w3-bar-item w3-button w3-center w3-padding-32\">CONTACT </button> <div class=\"dropdown-container\"> <p>9455396226</p> <p>piyuskgupta1997";
client.println(html); 
html ="gmail.com</p> <p>https://www.facebook.com/Home_esp-137734820384279/inbox/?selected_item_id=100007944130317</p> </div> </nav> <script> /* Loop through all dropdown buttons to toggle between hiding and showing its dropdown content - This allows the user to have multiple dropdowns without any conflict */ var dropdown = document.getElementsByClassName(\"w3-button\"); var i; for (i = 0; i < dropdown.length; i++) { dropdown[i].addEventListener(\"click\", function() { this.classList.toggle(\"active\"); var dropdownContent = this.nextElementSibling; if (dropdownContent.style.display === \"block\") { dropdownContent.style.display = \"none\"; } else { dropdownContent.style.display = \"block\"; } }); } </script> <!-- Top menu on small screens --> <header class=\"w3-top w3-black w3-xlarge\"> <span class=\"w3-left\"> <div id=\"lefto\" class=\"lefto\"></div> <div id=\"open\" class=\"open\"></div> <img src=\"https://i.imgur.com/ROyPc4Q.png\" width=\"100%\" align=\"center\"></span> <a href=\"javascript:void(0)\" class=\"w3-right w3-button w3-black\" onclick=\"w3_open()\">☰</a> </header> <!-- Overlay effect when opening sidebar on small screens --> <div class=\"w3-overlay w3-animate-opacity\" onclick=\"w3_close()\" style=\"cursor:pointer\" title=\"close side menu\" id=\"myOverlay\"></div>";
client.println(html); 
html ="<!-- !PAGE CONTENT! --> <div class=\"w3-main w3-content\" style=\"max-width:1600px;margin-top:83px\"> <!-- Photo Grid --> <div class=\"w3-row-padding\" id=\"myGrid\" style=\"margin-bottom:28px\"> <div class=\"w3-quarter\">";
client.println(html);
html=" <img src=\"https://i.imgur.com/yTwpJuul.jpg\" style=\"width:100%\" onclick=\"document.getElementById('lights').style.display='block'\" alt=\"Lights\"> <div id=\"lights\" class=\"w3-modal\"> <div class=\"w3-modal-content w3-card-4 w3-animate-top\"> <header class=\"w3-container w3-yellow w3-center w3-wide\" style=\"font-family:allerta;color:white;\" > <span onclick=\"document.getElementById('lights').style.display='none'\" class=\"w3-btn w3-display-topright\">&times;</span> <h2><strong >Lights</strong></h2> </header> <div class=\"w3-container w3-center w3-yellow \" style=\"font-family:Cursive\"> <img src=\"https://i.imgur.com/VcFvUM4.png\" style=\"width:25%\"> <div > <a href=\"/LAMP=ON\"><img src=\"https://i.imgur.com/3O1udd6.png\" style=\"width:10%\"></a> <a href=\"/LAMP=OFF\"><img src=\"https://i.imgur.com/2C03Yrk.png\" style=\"width:10%\"></a> </div> </div><!--w3 container--> </div><!--end modal content--> </div><!--end modal--> ";
client.println(html);
html="<img src=\"https://i.imgur.com/fMa0e9V.png\" style=\"width:100%\" onclick=\"document.getElementById('ldr').style.display='block'\" alt=\"Lights Levels\"> <div id=\"ldr\" class=\"w3-modal\"> <div class=\"w3-modal-content w3-card-4 w3-animate-top\"> <header class=\"w3-container w3-orange w3-center w3-wide\" style=\"font-family:allerta;\" > <span onclick=\"document.getElementById('ldr').style.display='none'\" class=\"w3-btn w3-display-topright\">&times;</span> <h2><strong style=\"color:white;\">Light Levels</strong></h2> </header> <div class=\"w3-container w3-center w3-orange \" style=\"font-family:Cursive\">";
client.println(html);
switch(light_count)
  {
     case 0:{client.println("<img src=\"https://i.imgur.com/x9lb4v8.png\">");break;}
     case 1:{client.println("<img src=\"https://i.imgur.com/x9lb4v8.png\">");break;}
     case 2:{client.println("<img src=\"https://i.imgur.com/TjK22vB.png\">");break;}
     case 3:{client.println("<img src=\"https://i.imgur.com/fMa0e9V.png\">");break;}
     case 4:{client.println("<img src=\"https://i.imgur.com/7YgVBXq.png\">");break;}
    }
    
client.println(" </div><!--w3 container--> </div><!--end modal content--> </div><!--end modal-->");
html=" <img src=\"https://i.imgur.com/ITK5psF.jpg\" style=\"width:100%\" onclick=\"document.getElementById('waterl').style.display='block'\" alt=\"Water Levels\"> <div id=\"waterl\" class=\"w3-modal\"> <div class=\"w3-modal-content w3-card-4 w3-animate-top\"> <header class=\"w3-container w3-blue w3-center w3-wide\" style=\"font-family:allerta;color:white;\" > <span onclick=\"document.getElementById('waterl').style.display='none'\" class=\"w3-btn w3-display-topright\">&times;</span> <h2><strong style=\"color:white;\">Water Levels</strong></h2> </header> <div class=\"w3-container w3-center w3-blue \" style=\"font-family:Cursive\">";
client.println(html);
switch(water_count)
  {
     case 0:{client.println("<img src=\"https://i.imgur.com/Bgfbru0.png\">");break;}
     case 1:{client.println("<img src=\"https://i.imgur.com/6e8PFcH.png\">");break;}
     case 2:{client.println("<img src=\"https://i.imgur.com/bQT6zVp.png\">");break;}
     case 3:{client.println("<img src=\"https://i.imgur.com/vsQRe8y.png\">");break;}
     case 4:{client.println("<img src=\"https://i.imgur.com/zET0YjC.png\">");break;}
    }

client.println(" </div><!--w3 container--> </div><!--end modal content--> </div><!--end modal-->");


html=" <img src=\"https://i.imgur.com/HXvdSpy.jpg\" style=\"width:100%\" onclick=\"document.getElementById('socket').style.display='block'\" alt=\"Sockets\"> <div id=\"socket\" class=\"w3-modal\"> <div class=\"w3-modal-content w3-card-4 w3-animate-top\"> <header class=\"w3-container w3-deep-purple w3-center w3-wide\" style=\"font-family:allerta;color:white;\" > <span onclick=\"document.getElementById('socket').style.display='none'\" class=\"w3-btn w3-display-topright\">&times;</span> <h2><strong style=\"color:white;\">Sockets</strong></h2> </header> <div class=\"w3-container w3-center w3-deep-purple \" style=\"font-family:Cursive\"> <img src=\"https://i.imgur.com/Wrs3Iqg.png\" style=\"width:20%\"><br/> <a href=\"/SOCKET=ON\"><img src=\"https://i.imgur.com/3O1udd6.png\"style=\"width:10%\"></a><br/> <a href=\"/SOCKET=OFF\"><img src=\"https://i.imgur.com/2C03Yrk.png\"style=\"width:10%\"></a><br/> </div><!--w3 container--> </div><!--end modal content--> </div><!--end modal--> </div>";
client.println(html);
html=" <div class=\"w3-quarter\"> <img src=\"https://i.imgur.com/utMFkUrl.jpg\" style=\"width:100%\" onclick=\"document.getElementById('temp').style.display='block'\" alt=\"FAN\"> <div id=\"temp\" class=\"w3-modal\"> <div class=\"w3-modal-content w3-card-4 w3-animate-top\"> <header class=\"w3-container w3-cyan w3-center w3-wide\" style=\"font-family:allerta;color:white;\" > <span onclick=\"document.getElementById('temp').style.display='none'\" class=\"w3-btn w3-display-topright\">&times;</span> <h2><strong style=\"color:white;\">TEMP</strong></h2> </header> <div class=\"w3-container w3-center w3-cyan \" style=\"font-family:Cursive\"> <img src=\"https://i.imgur.com/34dclJy.png\" style=\"width:30%\"><br/> <font face=\"Century Gothic\" color=\"white\"><h2 align=\"center\">";
client.println(html);
client.println(temp_val);
client.println("deg</h2></font> </div><!--w3 container--> </div><!--end modal content--> </div><!--end modal--> ");
html="<img src=\"https://i.imgur.com/3dn1Rz6.jpg\" style=\"width:100%\" onclick=\"document.getElementById('water').style.display='block'\" alt=\"Water\"> <div id=\"water\" class=\"w3-modal\"> <div class=\"w3-modal-content w3-card-4 w3-animate-top\"> <header class=\"w3-container w3-blue w3-center w3-wide\" style=\"font-family:allerta;\" > <span onclick=\"document.getElementById('water').style.display='none'\" class=\"w3-btn w3-display-topright\">&times;</span> <h2><strong>Water</strong></h2> </header> <div class=\"w3-container w3-center w3-blue \" style=\"font-family:Cursive\"> <img src=\"https://i.imgur.com/Tc1fcwz.png\" style=\"width:20%\"><br/> <a href=\"/MANUAL\" onMouseOut=\"MM_swapImgRestore()\" onMouseOver=\"MM_swapImage('Image20','','https://i.imgur.com/rvCDCpD.png',1)\"><img src=\"https://i.imgur.com/Tr5qHtN.png\" alt=\"\" vspace=\"2%\" id=\"Image20\"></a> <a href=\"/RANGE_LIMIT\" onMouseOut=\"MM_swapImgRestore()\" onMouseOver=\"MM_swapImage('Image21','','https://i.imgur.com/YEM6dHy.png',1)\"><img src=\"https://i.imgur.com/iH4FoT2.png\" alt=\"\" vspace=\"2%\" id=\"Image21\"></a> <a href=\"/AUTO\" onMouseOut=\"MM_swapImgRestore()\" onMouseOver=\"MM_swapImage('Image22','','https://i.imgur.com/cNkOpsB.png',1)\"><img src=\"https://i.imgur.com/Lo6ao4l.png\" alt=\"\" vspace=\"2%\" id=\"Image22\"></a> </div><!--w3 container--> </div><!--end modal content--> </div><!--end modal-->";
client.println(html);
html=" <img src=\"https://i.imgur.com/RfZqsJMl.jpg\" style=\"width:100%\" onclick=\"document.getElementById('fire').style.display='block'\" alt=\"Fire-Alarm\"> <div id=\"fire\" class=\"w3-modal\"> <div class=\"w3-modal-content w3-card-4 w3-animate-top\"> <header class=\"w3-container w3-light-green w3-center w3-wide\" style=\"font-family:allerta;color:white;\" > <span onclick=\"document.getElementById('fire').style.display='none'\" class=\"w3-btn w3-display-topright\">&times;</span> <h2><strong style=\"color:white;\">Fire-Alarm</strong></h2> </header> <div class=\"w3-container w3-center w3-light-green \" style=\"font-family:Cursive\"> <img src=\"https://i.imgur.com/316mdjW.png\" style=\"width:40%\"> </div><!--w3 container--> </div><!--end modal content--> </div><!--end modal-->";
client.println(html); 
html=" <img src=\"https://i.imgur.com/InmTGdV.jpg\" style=\"width:100%\" onclick=\"document.getElementById('fan').style.display='block'\" alt=\"FAN\"> <div id=\"fan\" class=\"w3-modal\"> <div class=\"w3-modal-content w3-card-4 w3-animate-top\"> <header class=\"w3-container w3-cyan w3-center w3-wide\" style=\"font-family:allerta;color:white;\" > <span onclick=\"document.getElementById('fan').style.display='none'\" class=\"w3-btn w3-display-topright\">&times;</span> <h2><strong style=\"color:white;\">FANS</strong></h2> </header> <div class=\"w3-container w3-center w3-cyan \" style=\"font-family:Cursive\"> <img src=\"https://i.imgur.com/9nhgzvS.png\" style=\"width:20%\"><br/> <a href=\"/FAN=ON\"><img src=\"https://i.imgur.com/3O1udd6.png\"style=\"width:10%\"></a> <a href=\"/FAN=OFF\"><img src=\"https://i.imgur.com/2C03Yrk.png\"style=\"width:10%\"></a><br/> <a href=\"/SPEED=INCREASE\"><button class=\"w3-button\">UP</button></a> <a href=\"/SPEED=DECREASE\"><button class=\"w3-button\">DOWN</button></a> </div><!--w3 container--> </div><!--end modal content--> </div><!--end modal--> </div> ";
client.println(html);
html="<div class=\"w3-quarter\"> <img src=\"https://i.imgur.com/b0UrBOR.jpg\" style=\"width:100%\" onclick=\"document.getElementById('door').style.display='block'\" alt=\"Door\"> <div id=\"door\" class=\"w3-modal\"> <div class=\"w3-modal-content w3-card-4 w3-animate-top\"> <header class=\"w3-container w3-brown w3-center w3-wide\" style=\"font-family:allerta;color:white;\" > <span onclick=\"document.getElementById('door').style.display='none'\" class=\"w3-btn w3-display-topright\">&times;</span> <h2><strong style=\"color:white;\">Doors</strong></h2> </header> <div class=\"w3-container w3-center w3-brown \" style=\"font-family:Cursive\"> <img src=\"https://i.imgur.com/1Zr33rh.png\" style=\"width:30%\"><br/> <a href=\"/DOOR=OPEN\"><img src=\"https://i.imgur.com/3O1udd6.png\"style=\"width:10%\"></a> <a href=\"/DOOR=CLOSE\"><img src=\"https://i.imgur.com/2C03Yrk.png\"style=\"width:10%\"></a><br/> <a href=\"/AUTOMATIC\" onMouseOut=\"MM_swapImgRestore()\" onMouseOver=\"MM_swapImage('Image23','','https://i.imgur.com/WgB2IqM.png',1)\"><img src=\"https://i.imgur.com/hO6Ttsz.png\" alt=\"\" width=\"20%\" height=\"20%\" vspace=\"2%\" id=\"Image23\"></a> </div><!--w3 container--> </div><!--end modal content--> </div><!--end modal-->";
client.println(html);
html=" <img src=\"https://i.imgur.com/08zNDq3l.jpg\" style=\"width:100%\" onclick=\"document.getElementById('people').style.display='block'\" alt=\"Person Count\"> <div id=\"people\" class=\"w3-modal\"> <div class=\"w3-modal-content w3-card-4 w3-animate-top\"> <header class=\"w3-container w3-dark-gray w3-center w3-wide\" style=\"font-family:allerta;color:white;\" > <span onclick=\"document.getElementById('people').style.display='none'\" class=\"w3-btn w3-display-topright\">&times;</span> <h2><strong style=\"color:white;\">Person Count</strong></h2> </header> <div class=\"w3-container w3-center w3-dark-gray \" style=\"font-family:Cursive\"> <a href=\"#nogo\"><img src=\"https://i.imgur.com/XBzic9W.png\" style=\"width:20%\"></a> <font color=\"white\" size=\"6\"><h1 align=\"center\"> 6</h1></font> </div><!--w3 container--> </div><!--end modal content--> </div><!--end modal-->";
client.println(html);
html=" <img src=\"https://i.imgur.com/HozgONJ.jpg\" style=\"width:100%\" onclick=\"document.getElementById('gallery').style.display='block'\" alt=\"Gallery Lights\"> <div id=\"gallery\" class=\"w3-modal\"> <div class=\"w3-modal-content w3-card-4 w3-animate-top\"> <header class=\"w3-container w3-purple w3-center w3-wide\" style=\"font-family:allerta;color:white;\" > <span onclick=\"document.getElementById('gallery').style.display='none'\" class=\"w3-btn w3-display-topright\">&times;</span> <h2><strong style=\"color:white;\">G A L L E R Y-L I G H T S</strong></h2> </header> <div class=\"w3-container w3-center w3-purple\" style=\"font-family:Cursive\">";
client.println(html);


switch(g_0)
  {
     case 0:{client.println("<td><img src=\"https://i.imgur.com/ZD6UyS0.png\" width=\"50\" height=\"50\"></td>");break;}
     case 1:{client.println("<td><img src=\"https://i.imgur.com/KmEQr5k.png\" width=\"50\" height=\"50\"></td>");break;}
  }
  switch(g_1)
  {
     case 0:{client.println("<td><img src=\"https://i.imgur.com/ZD6UyS0.png\" width=\"50\" height=\"50\"></td>");break;}
     case 1:{client.println("<td><img src=\"https://i.imgur.com/KmEQr5k.png\" width=\"50\" height=\"50\"></td>");break;}
  }
  switch(g_2)
  {
     case 0:{client.println("<td><img src=\"https://i.imgur.com/ZD6UyS0.png\" width=\"50\" height=\"50\"></td>");break;}
     case 1:{client.println("<td><img src=\"https://i.imgur.com/KmEQr5k.png\" width=\"50\" height=\"50\"></td>");break;}
  }
  
  
  client.println("</tr>");
  client.println("<tr>");
  switch(g_3)
   {
      case 0:{client.println("<td><img src=\"https://i.imgur.com/ZD6UyS0.png\" width=\"50\" height=\"50\"></td>");break;}
      case 1:{client.println("<td><img src=\"https://i.imgur.com/KmEQr5k.png\" width=\"50\" height=\"50\"></td>");break;}
   }
   switch(g_4)
  {
     case 0:{client.println("<td><img src=\"https://i.imgur.com/ZD6UyS0.png\" width=\"50\" height=\"50\"></td>");break;}
     case 1:{client.println("<td><img src=\"https://i.imgur.com/KmEQr5k.png\" width=\"50\" height=\"50\"></td>");break;}
  }
  switch(g_5)
  {
     case 0:{client.println("<td><img src=\"https://i.imgur.com/ZD6UyS0.png\" width=\"50\" height=\"50\"></td>");break;}
     case 1:{client.println("<td><img src=\"https://i.imgur.com/KmEQr5k.png\" width=\"50\" height=\"50\"></td>");break;}
  }

 client.println(" </div><!--w3 container--> </div><!--end modal content--> </div><!--end modal--> </div> ");



html="<div class=\"w3-quarter\"> <img src=\"https://i.imgur.com/wBlpbdbl.jpg\" style=\"width:100%\" onclick=\"document.getElementById('fanl').style.display='block'\" alt=\"Speed\"> <div id=\"fanl\" class=\"w3-modal\"> <div class=\"w3-modal-content w3-card-4 w3-animate-top\"> <header class=\"w3-container w3-cyan w3-center w3-wide\" style=\"font-family:allerta;color:white;\" > <span onclick=\"document.getElementById('fanl').style.display='none'\" class=\"w3-btn w3-display-topright\">&times;</span> <h2><strong style=\"color:white;\">Speed</strong></h2> </header> <div class=\"w3-container w3-center w3-cyan \" style=\"font-family:Cursive\">";
client.println(html);

switch(fan_count)
  {
     case 0:{ client.println("<img src=\"https://i.imgur.com/ykrhfC3.png\">");break;}
     case 1:{ client.println("<img src=\"https://i.imgur.com/q4mDuGP.png\">");break;}
     case 2:{ client.println("<img src=\"https://i.imgur.com/MRxGPO8.png\">");break;}
     case 3:{ client.println("<img src=\"https://i.imgur.com/VMet7B5.png\">");break;}
    }

client.println("</div><!--w3 container--> </div><!--end modal content--> </div><!--end modal--> ");

html="<img src=\"https://i.imgur.com/Gee0bSLl.jpg\" style=\"width:100%\" onclick=\"document.getElementById('alarm').style.display='block'\" alt=\"Alarm\"> <div id=\"alarm\" class=\"w3-modal\"> <div class=\"w3-modal-content w3-card-4 w3-animate-top\"> <header class=\"w3-container w3-aqua w3-center w3-wide\" style=\"font-family:allerta;color:white;\" > <span onclick=\"document.getElementById('alarm').style.display='none'\" class=\"w3-btn w3-display-topright\">&times;</span> <h2><strong style=\"color:blue;\">Alarm</strong></h2> </header> <div class=\"w3-container w3-center w3-aqua \" style=\"font-family:Cursive\"> <form action=\"\" method=\"\"> <div id=\"jsalarmclock\"> <div> <div class=\"leftcolumn\">Current Time:</div> <span id=\"alarm_alarm\" style=\"letter-spacing: 2px\"></span> </div> <div> <div class=\"leftcolumn w3-container\">Set Alarm:</div> <span><select class=\"select_menu\"></select> Hour</span> <span><select class=\"select_menu\"></select> Minutes</span> <span><select class=\"select_menu\"></select> Seconds</span> </div> <div> <div class=\"leftcolumn\">Action:</div> <input type=\"text\" class=\"text_menu\" id=\"musicloc\" size=\"55\" value=\"https://www.youtube.com/watch?v=wU2BDZkv17k\" /> </div> <input type=\"submit\" class=\"button_menu\" value=\"Set Alarm!\" id=\"submit_submit\" /> <input type=\"reset\" class=\"button_menu \" value=\"reset\" id=\"reset_reset\" /><br/> </div> </form> <script type=\"text/javascript\"> jsalarm.init() </script> </div><!--w3 container--> </div><!--end modal content--> </div><!--end modal-->";
client.println(html);
html=" <img src=\"https://i.imgur.com/jl8pHrll.jpg\" style=\"width:100%\" onclick=\"document.getElementById('humid').style.display='block'\" alt=\"Humididty\"> <div id=\"humid\" class=\"w3-modal\"> <div class=\"w3-modal-content w3-card-4 w3-animate-top\"> <header class=\"w3-container w3-red w3-center w3-wide\" style=\"font-family:allerta;color:white;\" > <span onclick=\"document.getElementById('humid').style.display='none'\" class=\"w3-btn w3-display-topright\">&times;</span> <h2><strong style=\"color:white;\">Humidity</strong></h2> </header> <div class=\"w3-container w3-center w3-red\" style=\"font-family:Cursive\"> <img src=\"https://i.imgur.com/sBaLlGJ.png\" style=\"width:30%\"> <font face=\"Century Gothic\" color=\"white\"><h1 align=\"center\">";
client.println(html);
client.println(humid_val);
client.println("</h1></font> </div><!--w3 container--> </div><!--end modal content--> </div><!--end modal-->");
html="<a href=\"https://www.facebook.com/Home_esp-137734820384279/inbox/?selected_item_id=100007944130317\"><img src=\"https://i.imgur.com/4qssSbsl.png\" style=\"width:100%\" alt=\"FB\"></a> </div> </div> <div class=\"w3-black w3-center w3-padding-24\">Created By <a href=\"https://www.facebook.com/Home_esp-137734820384279/inbox/?selected_item_id=100007944130317\" title=\"Circuit Magicians\" target=\"_blank\" class=\"w3-hover-opacity\">Circuit Magicians</a></div> <!-- End page content --> </div>";
client.println(html); 

html ="<script> function w3_open() { document.getElementById(\"sidenav\").style.display = \"block\"; document.getElementById(\"myOverlay\").style.display = \"block\"; } function w3_close() { document.getElementById(\"sidenav\").style.display = \"none\"; document.getElementById(\"myOverlay\").style.display = \"none\"; } </script> <script src=\"https://ajax.googleapis.com/ajax/libs/jquery/3.3.1/jquery.min.js\"></script> <script type=\"text/javascript\"> function MM_swapImgRestore() { var i,x,a=document.MM_sr; for(i=0;a&&i<a.length&&(x=a[i])&&x.oSrc;i++) x.src=x.oSrc;} function MM_preloadImages() { var d=document; if(d.images){ if(!d.MM_p) d.MM_p=new Array();var i,j=d.MM_p.length,a=MM_preloadImages.arguments; for(i=0; i<a.length; i++)if (a[i].indexOf(\"#\")!=0){ d.MM_p[j]=new Image; d.MM_p[j++].src=a[i];}}} function MM_findObj(n, d) { var p,i,x; if(!d) d=document; if((p=n.indexOf(\"?\"))>0&&parent.frames.length) {d=parent.frames[n.substring(p+1)].document; n=n.substring(0,p);}if(!(x=d[n])&&d.all) x=d.all[n]; for (i=0;!x&&i<d.forms.length;i++) x=d.forms[i][n];for(i=0;!x&&d.layers&&i<d.layers.length;i++) x=MM_findObj(n,d.layers[i].document);if(!x && d.getElementById) x=d.getElementById(n); return x;} function MM_swapImage() { var i,j=0,x,a=MM_swapImage.arguments; document.MM_sr=new Array; for(i=0;i<(a.length-2);i+=3)if ((x=MM_findObj(a[i]))!=null){document.MM_sr[j++]=x; if(!x.oSrc) x.oSrc=x.src; x.src=a[i+2];}} </script> </body> </html>";
client.println(html); 

 
}

String getTime() {
  WiFiClient client;                                                                                        // creating an object of the type WiFi Client
  while (!!!client.connect("google.com", 80)) {                                                           
    //Serial.println("connection failed, retrying...");
  }

  client.print("HEAD / HTTP/1.1\r\n\r\n");
  while(!!!client.available()) {
     yield();
  }

  while(client.available()){
    if (client.read() == '\n') {    
      if (client.read() == 'D') {    
        if (client.read() == 'a') {    
          if (client.read() == 't') {    
            if (client.read() == 'e') {    
              if (client.read() == ':') {    
                client.read();
                String theDate = client.readStringUntil('\r');
                client.stop();
                return theDate;
              }
            }
          }
        }
      }
    }
  }
}void setup() {
  // put your setup code here, to run once:

}

void loop() {
  // put your main code here, to run repeatedly:

}

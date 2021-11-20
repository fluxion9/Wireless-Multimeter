#include <ArduinoJson.h>
#include <ESP8266WiFi.h>
#include <ESPAsyncTCP.h>
#include <ESPAsyncWebServer.h>
const char* ssid = "wireless meter";
const char* password = "";
AsyncWebServer server(80);
const char* html = "<html><body><h1>Hello there!</h1></body></html>";
 void setup()
 {
    WiFi.softAP(ssid);
    IPAddress local_IP(192,168,4,22);
    IPAddress gateway(192,168,4,9);
    IPAddress subnet(255,255,255,0);
    server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/html", html);
  });
  server.begin();
 }
 void loop() 
 {

 }
 void doSomething()
 {
   ;
 }
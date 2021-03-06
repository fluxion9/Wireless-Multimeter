//{acv:220, dcv:3.3, dca:2.0, res:1000}
#include <ArduinoJson.h>
#include <ESP8266WiFi.h>
#include <ESPAsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <ArduinoJson.h>
const char* ssid = "wireless meter";
const char* password = "";
AsyncWebServer server(80);
const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE HTML><html>
<head>
<title>Wireless Multimeter</title>
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <style>
    html {
     font-family: Arial;
     display: inline-block;
     margin: 0px auto;
     text-align: center;
    }
    h2 { font-size: 3.0rem; }
    p { font-size: 3.0rem; }
    .units { font-size: 1.2rem; }
    .labels{
      font-size: 1.5rem;
      vertical-align:middle;
      padding-bottom: 15px;
    }
  </style>
</head>
<body>
  <h2>Wireless Multimeter</h2>
  <p>
    <span class="labels">Voltage DC</span> 
    <span id="dcv"></span>
    <sup class="units">V</sup>
  </p>
  <p>
    <span class="labels">Current DC</span>
    <span id="dca"></span>
    <sup class="units">mA</sup>
  </p>
  <p>
    <span class="labels">Voltage ~AC</span>
    <span id="acv"></span>
    <sup class="units">V</sup>
  </p>
  <p>
    <span class="labels">Resistance</span>
    <span id="res"></span>
    <sup class="units">&#x3A9</sup>
  </p>
</body>
<script>
routines();
setInterval(routines, 3000 );
function routines()
{
  checkDCV();
  checkDCA();
  checkACV();
  checkRES();
}
function checkDCV() {
  var xhttp = new XMLHttpRequest();
  xhttp.onreadystatechange = function() {
    if (this.readyState == 4 && this.status == 200) {
      document.getElementById("dcv").innerHTML = this.responseText;
    }
  };
  xhttp.open("GET", "/dcv", true);
  xhttp.send();
}
function checkDCA() {
  var xhttp = new XMLHttpRequest();
  xhttp.onreadystatechange = function() {
    if (this.readyState == 4 && this.status == 200) {
      document.getElementById("dca").innerHTML = this.responseText;
    }
  };
  xhttp.open("GET", "/dca", true);
  xhttp.send();
}
function checkACV() {
  var xhttp = new XMLHttpRequest();
  xhttp.onreadystatechange = function() {
    if (this.readyState == 4 && this.status == 200) {
      document.getElementById("acv").innerHTML = this.responseText;
    }
  };
  xhttp.open("GET", "/acv", true);
  xhttp.send();
}
function checkRES() {
  var xhttp = new XMLHttpRequest();
  xhttp.onreadystatechange = function() {
    if (this.readyState == 4 && this.status == 200) {
      document.getElementById("res").innerHTML = this.responseText;
    }
  };
  xhttp.open("GET", "/res", true);
  xhttp.send();
}
</script>
</html>)rawliteral";
float dcv, dca, acv, res;
String data;
void setup()
{
  Serial.begin(9600);
  pinMode(LED_BUILTIN, 1);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.println(WiFi.localIP());
  while (WiFi.status() != WL_CONNECTED) {
    digitalWrite(LED_BUILTIN, 0);
    delay(150);
    digitalWrite(LED_BUILTIN, 1);
    delay(150);
  }
  digitalWrite(LED_BUILTIN, 0);
  
  server.on("/", HTTP_GET, [](AsyncWebServerRequest * request) {
    request->send_P(200, "text/html", index_html);
  });
  server.on("/dcv", HTTP_GET, [](AsyncWebServerRequest * request) {
    request->send_P(200, "text/plain", String(dcv).c_str());
  });
  server.on("/dca", HTTP_GET, [](AsyncWebServerRequest * request) {
    request->send_P(200, "text/plain", String(dca).c_str());
  });
  server.on("/acv", HTTP_GET, [](AsyncWebServerRequest * request) {
    request->send_P(200, "text/plain", String(acv).c_str());
  });
  server.on("/res", HTTP_GET, [](AsyncWebServerRequest * request) {
    request->send_P(200, "text/plain", String(res).c_str());
  });
  server.begin();
}
void loop()
{
  if(Serial.available())
  {
    while(Serial.available())
    {
      delay(3);
      char d = Serial.read();
      data += d;
    }
    deserializeAndExtractJSON(data);
    Serial.println(data);
    Serial.println("acv: " + String(acv));
    Serial.println("dcv: " + String(dcv));
    Serial.println("dca: " + String(dca));
    Serial.println("res: " + String(res));
    data = "";
    Serial.flush();
  }
}

void deserializeAndExtractJSON(String input)
{
  DynamicJsonDocument doc(1024);
  deserializeJson(doc, input);
  JsonObject obj = doc.as<JsonObject>();
  dcv = obj[String("dcv")];
  dca = obj[String("dca")];
  acv = obj[String("acv")];
  res = obj[String("res")];
}

// Main module of async_web_led PlatformIO/Arduino sketch
// Copyright: see notice in async_web_led.ino

#include <Arduino.h>
#include <WiFi.h>
#include <AsyncTCP.h>            // See: Note on Location of Async Libraries
#include <ESPAsyncWebServer.h>   // See: Note on Location of Async Libraries
#include "html.h"
#include "secrets.h"

/*********************************************************************************************************************************
Note on Location of Async libraries

In both Arduino and PlatformIO, private copies of the AsyncTCP and ESPAsyncWebServer must be used for two reasons.

1. The very latest version of the me-no-dev/ESP Async WebServer must be used. The latest stable version (1.2.3) obtained with
   the PIO Libraries manager is incompatible with the latest version of md5.h.  This can be fixed by getting the development branch
   https://github.com/me-no-dev/ESPAsyncWebServer.git
   Ref: https://github.com/me-no-dev/ESPAsyncWebServer/issues/1147

2. The IPAddress AsyncWebSocketClient::remoteIP() in .pio/libdeps/seeed_xiao_esp32c3/ESP Async WebServer/src/AsyncWebSocket.cpp
   must be edited. The 0 null address returned if a client is not defined must be typecaset to uint32_t.
      IPAddress AsyncWebSocketClient::remoteIP() {
        if(!_client) {
            return IPAddress((uint32_t) 0U);
        }
        return _client->remoteIP();
      }
   Reference: https://github.com/me-no-dev/ESPAsyncWebServer/issues/1164

Placing both AsyncTCP and ESPAsyncWebServer in the scr subdirectory did not work in Arduino. The many #include <AsyncTCP.h> in
EPSAsyncWebServer could not be resolved. The solution was to save both libraries in a directory named "libraries" as a sibling of
the 05_ASYNC_WEB_LED and then to make their common parent directory xiao_esp32c3_sketches the Sketchbook location in the
Arduino Preferences.
***********************************************************************************************************************************/

// Connecting an external LED:
//  The diode's cathode (-, usually the short lead on the flat side of the LED) is connected to GND.
//  The diode's anode (+, usually the long lead on the round side of the LED) is connected to a
//  current limiting 240 ohm resistor. The other lead of the resistor is connected to an I/O pin.
//
int led = D10;

int ledState = 0;
String ledStatus = "OFF";

void setLed(int value) {
  digitalWrite(led, value);
  ledState = value;
  ledStatus = (digitalRead(led) ? "ON" : "OFF");
  Serial.printf("LED now %s.\n", ledStatus);
  assert(ledState == digitalRead(led));
}

void toggleLed(void) {
  setLed(1-ledState);
}

// Webserver instance using default HTTP port 80
AsyncWebServer server(80);

// Template substitution function
String processor(const String& var){
  if (var == "LEDSTATUS") return String(ledStatus);
  if (var == "SERVERNAME") return String("XIAO ESP32C3 WEB SERVER");
  return String(); // empty string
}

// 404 error handler
void notFound(AsyncWebServerRequest *request) {
  request->send_P(404, "text/html", html_404, processor);
}

void setup() {
  // Set the digital pin connected to the LED as an output
  pinMode(led, OUTPUT);
  digitalWrite(led, 0);

  Serial.begin();
  delay(1000);      // 1 second delay should be sufficient

  // Connect to Wi-Fi network with SSID and password
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(100);
    Serial.print(".");
  }
  // Print local IP address and start web server
  Serial.println("");
  Serial.println("WiFi connected.");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

  // Setup and start Web server
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    Serial.println("Index page requested");
    request->send_P(200, "text/html", html_index, processor);
  });
  server.on("/led", HTTP_GET, [](AsyncWebServerRequest *request){
    Serial.println("Web button pressed");
    toggleLed();
    request->send_P(200, "text/html", html_index, processor); // updates the client making the request only
  });
  server.onNotFound(notFound);
  server.begin();

  setLed(0);
  Serial.println("setup completed.");
}

void loop() {
}

/*
 *  See async_web_led.ino for license and attribution.
 */

#include <Arduino.h>
#include <WiFi.h>
#include <ESPAsyncWebServer.h> 
#include "html.h"
#include "secrets.h"


#if !defined(ARDUINO_XIAO_ESP32C3)
  #error This program is meant to run on the XIAO ESP32C3 only
#endif

#if (ESP_ARDUINO_VERSION < ESP_ARDUINO_VERSION_VAL(3, 3, 6))    
  #warning Version 3.3.6 or newer of ESP32 Arduino core version is available
#endif

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
  Serial.printf("LED now %s.\n", ledStatus.c_str());
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
  request->send(404, "text/html", html_404, processor);
}

void setup() {
  Serial.begin();
  // Delay to allow for the initialization of the native USB peripheral
  // and some time for the IDE to reconnect 
  #ifdef PLATFORMIO
  delay(8000); // 8 seconds
  #else
  delay(2000); // 2 seconds
  #endif

  Serial.println("\n\nProject: async_web_led");
  Serial.println("Purpose: Toggle an external LED on and off with a button on a Web GUI");
  Serial.println("  Board: XIAO ESP32C3");                   

  // Set the digital pin connected to the LED as an output
  pinMode(led, OUTPUT);
  digitalWrite(led, 0);

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
  Serial.print("WiFi connected with IP address: ");
  Serial.println(WiFi.localIP());

  // Setup and start Web server
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    Serial.println("Index page requested");
    request->send(200, "text/html", html_index, processor);
  });
  server.on("/led", HTTP_GET, [](AsyncWebServerRequest *request){
    Serial.println("Web button pressed");
    toggleLed();
    request->send(200, "text/html", html_index, processor); // updates the client making the request only
  });
  server.onNotFound(notFound);
  server.begin();

  setLed(0);
  Serial.println("setup completed.");
}

void loop() {
}

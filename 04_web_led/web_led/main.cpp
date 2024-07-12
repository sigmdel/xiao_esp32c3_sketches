// Main module of web_led PlatformIO/Arduino sketch
// Copyright: see notice in web_led.ino

/*
 * Web controled LED sketch for XIAO ESP32C3 in platformIO
 * Based on
 *   SimpleWiFiServer.ino
 *    @ https://github.com/espressif/arduino-esp32/blob/master/libraries/WiFi/examples/SimpleWiFiServer/SimpleWiFiServer.ino
 *   by Tom Igoe and Jan Hendrik Berlin
 * and
 *   ESP32 Web server based Home automation circuit diagram and programming
 *    @ https://www.electroniclinic.com/esp32-web-server-based-home-automation-circuit-diagram-and-programming/
 *   by Shahzada Fahad (Engr)
 */

#include <Arduino.h>     // Needed in PlatformIO
#include <WiFi.h>
#include "secrets.h"     // Edit secrets.h.template and save as secrets.h
#include "webresponses.h"

// Connecting an external LED:
//  The diode's cathode (-, usually the short lead on the flat side of the LED) is connected to GND.
//  The diode's anode (+, usually the long lead on the round side of the LED) is connected to a
//  current limiting 240 ohm resistor. The other lead of the resistor is connected to an I/O pin.
//  See https://files.seeedstudio.com/wiki/XIAO_WiFi/connect-led-2.png
//
const int ledPin = D10;
const int ledOn = HIGH;

String ledState = "off";

void setLed(int value) {
  digitalWrite(ledPin, (value) ? ledOn : 1-ledOn);
  ledState = (digitalRead(ledPin) == ledOn) ? "on" : "off";
  Serial.printf("LED now %s.\n", ledState.c_str());
}

// Set web server port number to 80
WiFiServer server(80);

// Variable to store the HTTP request
String header;

// Current time
unsigned long currentTime = millis();
// Previous time
unsigned long previousTime = 0;
// Define timeout time in milliseconds (example: 2000ms = 2s)
const long timeoutTime = 2000;

void setup() {
  // Set the digital pin connected to the LED as an output
  pinMode(led, OUTPUT);
  digitalWrite(led, 0);

  Serial.begin();
  delay(2000);      // 2 second delay should be sufficient

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
  server.begin();
}

void loop(){
  WiFiClient client = server.accept();
  if (client) {                             // If a new client connects,
    Serial.println("\nNew Client.");        // print a message out in the serial port
    String currentLine = "";                // make a String to hold incoming data from the client
    currentTime = millis();
    previousTime = currentTime;
    while (client.connected() && currentTime - previousTime <= timeoutTime) { // loop while the client's connected
      currentTime = millis();
      if (client.available()) {             // if there's bytes to read from the client,
        char c = client.read();             // read a byte, then
        //Serial.write(c);                    // print it out the serial monitor
        header += c;
        if (c == '\n') {                    // if the byte is a newline character
          // if the current line is blank, you got two newline characters in a row.
          // that's the end of the client HTTP request, so send a response:
          if (currentLine.length() == 0) {
            int n = header.indexOf("HTTP/") + 7;
            Serial.printf("Header: %s...\n", header.substring(0, n).c_str());
            bool is404 = false;
            if (header.indexOf("GET /?led=toggle ") >= 0) {
              setLed(1 - digitalRead(led));
            } else {
              is404 = (header.indexOf("GET / ") <0 ) && (header.indexOf("GET /index.htm") <0 );
            }
            if (is404)
              Serial.println("Unknown request.");
            else
              Serial.println("Valid request.");
            client.print(HttpResponses(is404));
            client.print(HttpPage(is404, digitalRead(led)));
            client.println(); // The HTTP response ends with another blank line
            // Break out of the while loop
            break;
          } else { // if you got a newline, then clear currentLine
            currentLine = "";
          }
        } else if (c != '\r') {  // if you got anything else but a carriage return character,
          currentLine += c;      // add it to the end of the currentLine
        }
      }
    } //while
    // Clear the header variable
    header = "";
    // Close the connection
    client.stop();
    Serial.println("Client disconnected.");
  } // if client
} // loop

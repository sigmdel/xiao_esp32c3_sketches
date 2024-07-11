// Main module of blink_led PlatformIO/Arduino sketch
// Copyright: see notice in blin_led.ino

// Based on
//   Getting Started with Seeed Studio XIAO ESP32C3
//   @ https://wiki.seeedstudio.com/XIAO_ESP32C3_Getting_Started/

#include <Arduino.h>

// Connecting an external LED:
//  The diode's cathode (-, usually the short lead on the flat side of the LED) is connected to GND.
//  The diode's anode (+, usually the long lead on the round side of the LED) is connected to a
//  current limiting 240 ohm resistor. The other lead of the resistor is connected to an I/O pin.
//  See https://files.seeedstudio.com/wiki/XIAO_WiFi/connect-led-2.png
//
const int ledPin = D10;
const int ledOn = HIGH;

void setup() {
  Serial.begin();
  delay(2000); // 2 second delay should be sufficient for USB-CDC

  Serial.printf("Set the mode of I/O pin %d connected to the LED to OUTPUT\n", ledPin);
  pinMode(ledPin, OUTPUT);

  Serial.println("Setup completed");
}

void loop() {
  digitalWrite(ledPin, ledOn);
  Serial.print("ON, ");
  // LED on for 1/20th of a second
  delay(50);
  digitalWrite(ledPin, 1-ledOn);
  Serial.println("Off...");
  // LED off for a second
  delay(1000);
}

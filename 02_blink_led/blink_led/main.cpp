// Main module of blink_led PlatformIO/Arduino sketch
// Copyright: see notice in blin_led.ino

// Based on
//   Getting Started with Seeed Studio XIAO ESP32C3
//   @ https://wiki.seeedstudio.com/XIAO_ESP32C3_Getting_Started/


#include <Arduino.h>  // Needed in PlatformIO

// Connecting an external LED:
//  The diode's cathode (-, usually the short lead on the flat side of the LED) is connected to GND.
//  The diode's anode (+, usually the long lead on the round side of the LED) is connected to a
//  current limiting 240 ohm resistor. The other lead of the resistor is connected to an I/O pin.
//
int led = D10;

void setup() {
  // Set the digital pin connected to the LED as an output
  pinMode(led, OUTPUT);

  Serial.begin();
  delay(1000);      // 1 second delay should be sufficient for USB-CDC
  Serial.println("Setup completed");
}

void loop() {
  digitalWrite(led, HIGH);   // turn the LED on
  Serial.print("ON, ");
  delay(50);                 // for 1/20th of a second
  digitalWrite(led, LOW);    // turn the LED off
  Serial.println("Off...");
  delay(1000);               // for a second
}

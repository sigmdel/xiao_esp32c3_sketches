// Main module of blink_led PlatformIO/Arduino sketch
// Copyright: see notice in button_led.ino

#include <Arduino.h>       // Needed in platformIO
#include "mdPushButton.h"

// Connecting an external LED:
//  The diode's cathode (-, usually the short lead on the flat side of the LED) is connected to GND.
//  The diode's anode (+, usually the long lead on the round side of the LED) is connected to a
//  current limiting 240 ohm resistor. The other lead of the resistor is connected to an I/O pin.
//
int led = D10;

void toggleLed() {
  digitalWrite(led, 1-digitalRead(led));
  Serial.printf("LED now %s.\n", (digitalRead(led) ? "on" : "off"));
}

// Connecting an external button:
//  Because an internal pullup resistor will be enabled, connect one lead of a normally
//  open push button to ground and the other lead to a free I/O pin, D9 here.
//
mdPushButton button = mdPushButton(D9);

void setup() {
  // Set the digital pin connected to the LED as an output
  pinMode(led, OUTPUT);

  Serial.begin();
  delay(1000);      // 1 second delay should be sufficient for USB-CDC
  Serial.println("Setup completed");
}

void loop() {
  switch (int clicks = button.status()) {
    case -1:
      Serial.println("Long button press");
      break;
    case  0:
      /* ignore this case - no button press */;
      break;
    case  1:
      Serial.println("Button pressed once");
      toggleLed();
      break;
    default  :
      Serial.print("Button pressed ");
      Serial.print(clicks);
      Serial.println(" times");
      break;
  }
}

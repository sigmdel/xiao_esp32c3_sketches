/*
 *  See button_led.ino for license and attribution.
 */

#include <Arduino.h>
#include "mdPushButton.h"

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
//  See https://files.seeedstudio.com/wiki/XIAO_WiFi/connect-led-2.png
//
const int ledPin = D10;
const int ledOn = HIGH;

void toggleLed() {
  digitalWrite(ledPin, 1-digitalRead(ledPin));
  Serial.printf("LED now %s.\n", (digitalRead(ledPin) == ledOn) ? "on" : "off");
}

// Using the XIAO boot button. One pole is connected to I/O port 9 (also pin D9) and the other
// pole is connected to ground. So when I/O pin is LOW, that means the button is pressed.
// Because an with an external pull up resistor connected to 3.3V on I/O port 9, there is no
// need to enable an internal pull up resistor.
// mdPushButton button = mdPushButton(D9, LOW, false);
// On the other hand it will not harm anything so the defaults can be used.
mdPushButton button = mdPushButton(BOOT_PIN);

void setup() {
  Serial.begin();
  // Delay to allow for the initialization of the native USB peripheral
  // and some time for the IDE to reconnect 
  #ifdef PLATFORMIO
  delay(8000); // 8 seconds
  #else
  delay(2000); // 2 seconds
  #endif

  Serial.println("\n\nProject: button_led.ino");
  Serial.println("Purpose: Control a LED with the boot button");
  Serial.println("  Board: XIAO ESP32C3");

  Serial.printf("\nSet the mode of I/O pin %d connected to the LED to OUTPUT\n", ledPin);
  pinMode(ledPin, OUTPUT);

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

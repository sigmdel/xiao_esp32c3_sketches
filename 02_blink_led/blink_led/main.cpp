/*
 *  See blink_led.ino for license and attribution.
 */

#include <Arduino.h>

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
int ledPin = 255;
const int ledOn = HIGH;

bool boot_button_pressed = false;   

void ARDUINO_ISR_ATTR boot_button_isr() {
  boot_button_pressed = true;
}


// Checking XIAO ESP32C3 dev board with USB connector at top, antenna connector at bottom.
// Probe each pad in anti-clockwise fashion starting at top left pad labeled D0 (on the underside)
const int padcount = 11;
const char *padlabels[padcount] = {"D0", "D1", "D2", "D3", "D4", "D5", "D6", // left edge
                                   "D7", "D8", "D9", "D10"};                 // right edge
const int iopins[padcount] = {D0, D1, D2, D3, D4, D5, D6,  // GPIO  2, 3, 4,  5, 6, 7, 21, // left edge
                              D7, D8, D9, D10};            // GPIO 20, 8, 9, 10            // right edge                     


int currentindex = -1;  // initially, no gpio pin is active
unsigned long remindertimer;

void nextPad() {
  Serial.println("nextPad()");

  if (currentindex >= 0) {
    // turn current gpio pin off 
    digitalWrite(iopins[currentindex], LOW);
    pinMode(iopins[currentindex], INPUT);
  }
  currentindex++;
  if (currentindex >= padcount)
    currentindex = 0;
  ledPin = iopins[currentindex];
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, 1-ledOn);
  Serial.printf("\nConnect LED to board pad %s as gpio pin %d is turned on and off.\n", padlabels[currentindex], ledPin);
  delay(100); // cheap boot button debounce
  remindertimer = millis();
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

  Serial.println("\n\nProject: blink_led");
  Serial.println("Purpose: Test all i/o pins for as digital outputs");
  Serial.println("         Also checks digital input and interrupt of the boot button.");
  Serial.println("  Board: XIAO ESP32C3");                   

  pinMode(BOOT_PIN, INPUT_PULLUP);
  attachInterrupt(BOOT_PIN, boot_button_isr, FALLING); 
  nextPad();
}

void loop() {
  if (boot_button_pressed) {
    boot_button_pressed = false;
    nextPad();
  }                         

  if (millis() - remindertimer > 15000) {
    Serial.println("Press the BOOT button to test the next pad.");
    remindertimer = millis();
  }  

  digitalWrite(ledPin, ledOn);
  Serial.print("ON, ");
  // LED on for 1/20th of a second
  delay(50);
  digitalWrite(ledPin, 1-ledOn);
  Serial.println("Off...");
  // LED off for a second
  delay(1000);
}

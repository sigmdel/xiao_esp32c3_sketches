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

// Time in milliseconds during which duty cycle value is used
#define LEVEL_PERIOD 50 

// The increment or decrement in the duty cycle between levels.
#define DELTA 10

// The initial duty cycle above 0
#define INIT_DELTA 5         

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
unsigned long delaytime = 0;
int delta = 0;   // pwm increment or decrement value - starts at 5 to hit 255 when incrementing by DELTA = 10 
int fade = 0;    // actual pwm value 


void nextPad() {
  //Serial.println("nextPad()");

  if (currentindex >= 0) {
    // turn current gpio pin off 
    pinMode(ledPin, OUTPUT);
    digitalWrite(ledPin, LOW);
    pinMode(ledPin, INPUT);
  }
  currentindex++;
  if (currentindex >= padcount)
    currentindex = 0;
  ledPin = iopins[currentindex];

  if (ledPin == BOOT_PIN) {
    Serial.printf("\nSkipping board pad %s because gpio pin %d is the BOOT_PIN\n", padlabels[currentindex], ledPin);
    currentindex++;
    ledPin = iopins[currentindex];
  }
  pinMode(ledPin, OUTPUT);
  analogWrite(ledPin, 0);
  fade = 0;
  delta = INIT_DELTA;
  delaytime = 0;
  Serial.printf("\nConnect LED to board pad %s as gpio pin %d is pulsed\n", padlabels[currentindex], ledPin);
  delay(100); // cheap boot button debounce
  remindertimer = millis();
}  

void pulse(void) {
  if (millis() - delaytime > LEVEL_PERIOD) {
    fade += delta;
    if (fade <= 0) {
      fade = INIT_DELTA;
      delta = DELTA;
    } else if (fade >= 255) {
      fade = 255;
      delta = - DELTA;
    }
    analogWrite(ledPin, fade);
    delaytime = millis();
  }
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

  Serial.println("\n\nProject: pulse_led");
  Serial.println("Purpose: Test the pulse width modulation capabilities of all i/o pins");
  Serial.println("  Board: XIAO ESP32C3");                   

  pinMode(BOOT_PIN, INPUT_PULLUP);
  attachInterrupt(BOOT_PIN, boot_button_isr, FALLING); 
  nextPad();
}

void loop() {
  if (boot_button_pressed) {
    boot_button_pressed = false;
    delay(50);
    nextPad();
  }                         

  if (millis() - remindertimer > 15000) {
    Serial.println("Press the BOOT button to test the next pad.");
    remindertimer = millis();
  }  

  pulse();
}

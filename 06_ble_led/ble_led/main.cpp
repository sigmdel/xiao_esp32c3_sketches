/*
 *  See ble_led.ino for license and attribution.
 */

#include <Arduino.h>
#include <ArduinoBLE.h>

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

void setLed(int value) {
  digitalWrite(ledPin, (value) ? ledOn : 1-ledOn);
  Serial.printf("LED now %s.\n", (digitalRead(ledPin) == ledOn) ? "on" : "off");
}

// Bluetooth® Low Energy LED Service
BLEService ledService("19B10000-E8F2-537E-4F6C-D104768A1214");

// Bluetooth® Low Energy LED Switch Characteristic - custom 128-bit UUID, readable and writable by central
BLEByteCharacteristic switchCharacteristic("19B10001-E8F2-537E-4F6C-D104768A1214", BLERead | BLEWrite);

void setup() {
  Serial.begin();
  // Delay to allow for the initialization of the native USB peripheral
  // and some time for the IDE to reconnect 
  #ifdef PLATFORMIO
  delay(8000); // 8 seconds
  #else
  delay(2000); // 2 seconds
  #endif

  Serial.println("\n\nProject: ble_led.ino");
  Serial.println("Purpose: Toggle an external LED on and off with Bluetooth");
  Serial.println("  Board: XIAO ESP32C3");

  // begin initialization

  Serial.println("\nInitializing LED");
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, 1-ledOn);

  if (!BLE.begin()) {
    Serial.println("Could not start Bluetooth® Low Energy module!");
    while (1);
  }
  Serial.println("Bluetooth® Low Energy (BLE) module started.");

  // set advertised local name and service UUID:
  BLE.setLocalName("HOME Automation");            // this will appear in the App search result.
  BLE.setAdvertisedService(ledService);

  // add the characteristic to the service
  ledService.addCharacteristic(switchCharacteristic);

  // add service
  BLE.addService(ledService);
  // set the initial value for the characeristic, i.e. LED off
  switchCharacteristic.writeValue(0);
  Serial.println("\nLED service added.");

  BLE.advertise();
  Serial.println("\"HOME Automation\" device now being advertised");
  Serial.println("Setup completed.");

  Serial.println("\nTurn the attached LED on/off with a smartphone applications such as");
  Serial.println("\n  nRF Connect for Mobile by Nordic Semiconductor ASA");
  Serial.println("    Android: https://play.google.com/store/apps/details?id=no.nordicsemi.android.mcp");
  Serial.println("    IOS: https://apps.apple.com/us/app/nrf-connect-for-mobile/id1054362403");
  Serial.println("\n  LightBlue - Bluetooth LE by Punch Through Design");
  Serial.println("    Android: https://play.google.com/store/apps/details?id=com.punchthrough.lightblueexplorer");
  Serial.println("    IOS: https://apps.apple.com/us/app/lightblue/id557428110");
}


void loop() {
  // listen for Bluetooth® Low Energy peripherals to connect:
  BLEDevice central = BLE.central();

  // if a central is connected to peripheral:
  if (central) {
    String dName = central.deviceName();
    if (!dName.length())
      dName = central.localName();
    if (!dName.length())
      dName = "device";                 // default if no name is found for the newly connected device
    dName += ": " + central.address();  // add the latter's MAC address
    Serial.print("Connected to ");
    Serial.println(dName);

    while (central.connected()) {
      if (switchCharacteristic.written()) {
        int Rvalue=switchCharacteristic.value();
        Serial.printf("Received switchCharacteristic = %02x.\n", Rvalue);

        if ((Rvalue == 0) || (Rvalue == 1)) {
          setLed(Rvalue);
        } else {
          Serial.println("Ignored, expected 00 or 01.\n");
        }

      }
    }

    Serial.print("Disconnected from ");
    Serial.println(dName);
  }
}

// Main module of ble_led PlatformIO/Arduino sketch
// Copyright: see notice in ble_led.ino

/*
 * Bluetooth (LE) controled LED sketch for XIAO ESP32C3 in platformIO
 * Based on
 *  XIAO ESP32C3 Bluetooth Tutorial, Range test, and Home Automation
 *  @ https://www.electroniclinic.com/xiao-esp32c3-bluetooth-tutorial-range-test-and-home-automation/#XIAO_ESP32C3_Home_Automation
 *  by Shahzada Fahad (Engr)
 *
 * Turn LED on/off with either of the following Androi/IOS app:
 *  nRF Connect for Mobile
 *    Android: https://play.google.com/store/apps/details?id=no.nordicsemi.android.mcp&gl=US
 *    IOS: https://apps.apple.com/us/app/nrf-connect-for-mobile/id1054362403
 * or
 *  LightBlue
 *    Android: https://play.google.com/store/apps/details?id=com.punchthrough.lightblueexplorer&gl=US
 *    IOS: https://apps.apple.com/us/app/lightblue/id557428110
 */

#include <Arduino.h>
#include <ArduinoBLE.h>

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
  // Set the digital pin connected to the LED as an output
  pinMode(led, OUTPUT);
  digitalWrite(led, 0);

  Serial.begin();
  delay(2000);      // 2 second delay should be sufficient

  // begin initialization
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
  Serial.println("LED service added.");

  BLE.advertise();
  Serial.println("\"HOME Automation\" device now being advertised");
  Serial.println("Setup completed.");
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

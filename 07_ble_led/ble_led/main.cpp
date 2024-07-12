// Main module of ble_led PlatformIO/Arduino sketch
// Copyright: see notice in ble_led.ino

#include <Arduino.h>
#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEServer.h>

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

// This is a simplified example which can use custom UUIDs in which case
// the client will probably show the UUID for the service and characteristic
// or it can use some more or less valid reserved UUID from the Bluetooth(R)
// Assigned Numbers document https://www.bluetooth.com/specifications/assigned-numbers/
//
//#define USE_CUSTOM_UUIDS

#define BLUETOOTH_NAME  "BLE_LED2"

#ifdef USE_CUSTOM_UUIDS
  // Custom UUID for service and characteristic must not conflict with a reserved UUID
  // that is no number in the XXXXXXXX-0000-1000-8000-00805F9B34FB range
  // Generated at https://www.guidgenerator.com/
  // https://novelbits.io/uuid-for-custom-services-and-characteristics/
  #define SERVICE_UUID        "57a81fc3-3c5f-4d29-80e7-8b074e34888c"
  #define CHARACTERISTIC_UUID "2eeae074-8955-47f7-9470-73f85112974f"
#else
  #define SERVICE_UUID        "1815" //"00001815-0000-1000-8000-00805F9B34FB"  // Automation IO Service
                            //"1812" //"00001812-0000-1000-8000-00805F9B34FB"  // Human Interface Device Service
                            //"181c" //"0000181c-0000-1000-8000-00805F9B34FB"  // User Data Service
  #define CHARACTERISTIC_UUID "2BE2" //"00002BE2-0000-1000-8000-00805F9B34FB"  // Light Output
                            //"2BO5" //"00002B05-0000-1000-8000-00805F9B34FB"  // Power
#endif

BLEServer *pServer = nullptr;
BLECharacteristic *pCharacteristic = nullptr;

bool deviceConnected = false;

class MyServerCallbacks: public BLEServerCallbacks {
    void onConnect(BLEServer* pServer) {
      deviceConnected = true;
      Serial.println("Device connected");
    };

    void onDisconnect(BLEServer* pServer) {
      deviceConnected = false;
      Serial.println("Device disconnected");
      Serial.println("Restart advertising");
      BLEDevice::startAdvertising();
    }
};

class WriteCallbacks : public BLECharacteristicCallbacks {
  void onWrite(BLECharacteristic *pCharacteristic) {
    String value = pCharacteristic->getValue().c_str();
    if (value == "on") {
      setLed(1);
    } else if(value == "off"){
      setLed(0);
    } else {
      Serial.printf("Received non valid \"%s\" value \n", value.c_str());
    }
  }
};


void setup() {
  Serial.begin();
  delay(2000);

  Serial.println("Setup");

  Serial.println("Initializing LED");
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, 1-ledOn);

  Serial.println("Initializing BLEDevice");
  BLEDevice::init(BLUETOOTH_NAME);

  Serial.println("Creating a BLE server");
  pServer = BLEDevice::createServer();
  pServer->setCallbacks(new MyServerCallbacks());

  Serial.println("Adding a BLE service");
  BLEService *pService = pServer->createService(SERVICE_UUID);

  Serial.println("Adding a BLE characteristic");
  pCharacteristic = pService->createCharacteristic(
                      CHARACTERISTIC_UUID,
                      BLECharacteristic::PROPERTY_READ | BLECharacteristic::PROPERTY_WRITE);
  pCharacteristic->setCallbacks(new WriteCallbacks);

  Serial.println("Starting BLE service");
  pService->start();

  Serial.println("Add advertiser");
  BLEAdvertising *pAdvertising = BLEDevice::getAdvertising();
  pAdvertising->addServiceUUID(pService->getUUID());
  pAdvertising->setScanResponse(true);

  Serial.println("Start BLE advertising");
  BLEDevice::startAdvertising();

  Serial.print("\nSetup completed, connect to ");
  Serial.println(BLUETOOTH_NAME);
  Serial.print("Address: ");
  Serial.println(BLEDevice::getAddress().toString().c_str());
}

unsigned long timer = 0;

void loop() {
  if (millis() - timer > 5000) {
    Serial.println(" - loop busy work");
    timer = millis();
  }
}

// Main module of pin_names Platformio/Arduino sketch
// Copyright: see notice in pin_names.ino

// The XIAO ESP32C3 pin definitions are found in
// Arduino:
//   ~/.arduino15/packages/esp32/hardware/esp32/3.0.2/variants/XIAO_ESP32C3/pins_arduino.h
// PlatformIO:
//   ~/.platformio/packages/framework-arduinoespressif32/variants/XIAO_ESP32C3/pins_arduino.h

#include <Arduino.h>

void iopins(void) {
  Serial.println("\n\nXIAO ESP32C3 I/O Pin Names and Numbers");

  Serial.println("\nThe symbolic name and corresponding I/O number of the 11 digital pins");
  Serial.printf(" D0 = %2d\n", D0);
  Serial.printf(" D1 = %2d\n", D1);
  Serial.printf(" D2 = %2d\n", D2);
  Serial.printf(" D3 = %2d\n", D3);
  Serial.printf(" D4 = %2d\n", D4);
  Serial.printf(" D5 = %2d\n", D5);
  Serial.printf(" D6 = %2d\n", D6);
  Serial.printf(" D7 = %2d\n", D7);
  Serial.printf(" D8 = %2d\n", D8);
  Serial.printf(" D9 = %2d\n", D9);
  Serial.printf("D10 = %2d\n", D10);

  Serial.println("\nThe symbolic name and corresponding I/O number of the 3 analogue pins");
  Serial.printf(" A0 = %d\n", A0);
  Serial.printf(" A1 = %d\n", A1);
  Serial.printf(" A2 = %d\n", A2);
  Serial.println("Note: Ax = Dx for x = 0, 1 and 2");

  Serial.println("\nThe symbolic name and corresponding I/O number of the 7 serial pins");
  Serial.printf("  TX = %2d [UART] (=D6)\n", TX);
  Serial.printf("  RX = %2d [UART] (=D7)\n", RX);

  Serial.printf(" SDA = %2d [I2C]  (=D4)\n", SDA);
  Serial.printf(" SCL = %2d [I2C]  (=D5)\n", SCL);

  Serial.printf("  SS = %2d [SPI]  (=D7)\n", SS);
  Serial.printf("MOSI = %2d [SPI]  (=D10)\n", MOSI);
  Serial.printf("MISO = %2d [SPI]  (=D9)\n", MISO);
  Serial.printf(" SCK = %2d [SPI]  (=D8)\n", SCK);

  Serial.println("\nBoard macro");
  #ifdef ARDUINO_XIAO_ESP32C3
  Serial.println("ARDUINO_XIAO_ESP32C3 defined");
  #else
  Serial.println("ARDUINO_XIAO_ESP32C3 not defined");
  #endif
}

void setup() {
  Serial.begin();
  delay(2000); // 2 second delay should be sufficient for USB-CDC
  Serial.println("Setup completed");
}

void loop() {
  iopins();
  delay(10000); // wait 10 seconds
}

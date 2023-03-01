// Main module of pin_names Platformio/Arduino sketch
// Copyright: see notice in pin_names.ino

// The XIAO ESP32C3 pin definitions are found in
// ~/.platformio/packages/framework-arduinoespressif32/variants/XIAO_ESP32C3/pins_arduino.h

#include <Arduino.h>  // Needed in PlatformIO

void iopins(void) {
  Serial.println("\n\nXIAO ESP32C3 I/O Pin Names and Numbers");
  Serial.println("\nThe symbolic name and corresponding I/O number of the 4 analogue pins");
  Serial.printf(" A0 = %d\n", A0);
  Serial.printf(" A1 = %d\n", A1);
  Serial.printf(" A2 = %d\n", A2);
  Serial.printf(" A3 = %d\n", A3);
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
  Serial.println("\nThe symbolic name and corresponding I/O number of the 7 serial pins");
  Serial.printf("  TX = %2d [UART]\n", TX);
  Serial.printf("  RX = %2d [UART]\n", RX);

  Serial.printf(" SDA = %2d [I2C]\n", SDA);
  Serial.printf(" SCL = %2d [I2C]\n", SCL);

  Serial.printf("  SS = %2d [SPI]\n", SS);
  Serial.printf("MOSI = %2d [SPI]\n", MOSI);
  Serial.printf("MISO = %2d [SPI]\n", MISO);
  Serial.printf(" SCK = %2d [SPI]\n", SCK);
}

void setup() {
  Serial.begin();
  delay(1000); // 1 second delay should be sufficient for USB-CDC
  Serial.println("Setup completed")
}

void loop() {
  iopins();
  delay(10000); // wait 10 seconds
}


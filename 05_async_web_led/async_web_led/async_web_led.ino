/*
 * async_web_led.ino
 * Toggle an external LED on and off with a button on a Web GUI
 *
 * This is a stub to satisfy the Arduino IDE, the source code is in
 * the file main.cpp in the same directory.
 *
 * This sketch will compile in the Arduino IDE
 *
 *  1- Add https://espressif.github.io/arduino-esp32/package_esp32_index.json
 *     in the Additional Boards Manager URLS in the Preferences window.
 *  2- Install platform esp32 by Espressif version 3.0.2 or newer with the Boards Manager
 *  3- Select the XIAO_ESP32C3 board
 *  4- Versions 1.1.1 of AsyncTCP and 1.2.4 of ESPAsyncWebServer by me-no-dev must be
 *     installed. See libraries/README.md for details
 *
 * This project will compile in PlatformIO using the following platformio.ini file
 *
 *   [platformio]
 *   ; Make the Arduino IDE happy (.INO file must be in a directory of the same name)
 *   src_dir = async_web_led
 *
 *   [env:seeed_xiao_esp32c3]
 *   board = seeed_xiao_esp32c3
 *   framework = arduino
 *   platform = espressif32
 *   monitor_speed = 460800
 *   lib_deps = me-no-dev/ESP Async WebServer@^1.2.4
 *
 * Michel Deslierres
 * July 11, 2024
 *
 * Copyright 2024, Michel Deslierres. No rights reserved, this code is in the public domain.
 * In those jurisdictions where this may be a problem, the BSD Zero Clause License applies.
 * <https://spdx.org/licenses/0BSD.html> */
 // SPDX-License-Identifier: 0BSD

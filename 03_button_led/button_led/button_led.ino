/*
 * button_led.ino
 * Toggle an external LED on and off with a push button switch
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
 *  4- Install the mdPushButton library - see libraries/README.md
 * 
 * This project will compile in PlatformIO using the following platformio.ini file
 *
 *   [platformio]
 *   ; Make the Arduino IDE happy (.INO file must be in a directory of the same name)
 *   src_dir = button_led
 *
 *   [env:seeed_xiao_esp32c3]
 *   board = seeed_xiao_esp32c3
 *   framework = arduino
 *   platform = espressif32
 *   monitor_speed = 460800
 *   lib_deps =
 *      https://github.com/sigmdel/mdPushButton.git ; Shameless plug for own work
 *
 * Michel Deslierres
 * July 10, 2024
 *
 * Copyright 2024, Michel Deslierres. No rights reserved, this code is in the public domain.
 * In those jurisdictions where this may be a problem, the BSD Zero Clause License applies.
 * <https://spdx.org/licenses/0BSD.html> */
 // SPDX-License-Identifier: 0BSD

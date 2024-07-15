# XIAO ESP32C3 Arduino Sketches / PlatformIO Projects

**Source code that accompanies the July 2024 revesion of [First Look at the Seeed Studio XIAO ESP32C3](https://sigmdel.ca/michel/ha/xiao/xiao_esp32c3_intro_en.html)**.

![XIAO ESP32C3 Pinout](images/xiao_esp32c3_pinout.png) 

## Introduction

The XIAO ESP32C3 is a recent addition to the Seeed Studio XIAO series of diminutive development boards. It is based on the Espressif ESP32-C3 SoC which, unlike other Espressif microcontrollers, has a RISC-V core.

It should be possible to compile each project with PlatformIO or in the Arduino IDE and then upload them to the XIAO. There should be no unresolved dependencies. In other words, there should not be any need to install anything other than this repository. To make these sketches self-contained, private copies of all third-party libraries are included. Where a private library is included, only the necessary files were added. Please obtain the full library from the original source if it is to be used in another project.

## Arduino IDE Notes

Arduino sketches must have an `.ino` file name extension and must be contained in a directory that has the same name as the Arduino sketch (excluding the extension). Consequenty the `01_pin_names` project containts a directory named `pin_names` that in turn contains the Arduino sketch `pin_names.ino`. That sketch is basically empty as it is a long comment only. This is not a problem because the Arduino IDE will import all source files found in the sketch directory. The actual code is in `main.cpp` which is the default name of a PlatformIO project.

![Directory tree](images/dir_tree.jpg) 

To compile and then upload the sketch in the Arduino IDE, click on the **File** top menu, click on **Open...**, then navigate to the `pin_names.ino` file and open it with the system file manager.

Private libraries are stored in the subdirectory `xiao_esp32c3_sketches/libraries`.  The Arduino IDE will find libraries in that directory if the `Sketchbook location` in the Arduino Preferences is set to the parent directory, `xiao_esp32c3_sketches`. Details can be found in [libraries/README.md](libraries/README.md).

## PlatformIO Notes

Because of the Arduino sketch naming constraints, the `main.cpp` file of a project is not stored in the `src` directory. To work around this change, a `src_dir` entry is added in the `platformio.ini` file to provide the name of the directory in which `main.cpp` is found. That will be the name of the Arduino sketch as shown here for the `01_pin_names` project. 

```ini
[platformio]
; Make the Arduino IDE happy (.INO file must be in a directory of the same name)
src_dir = pin_names
```

PlatformIO will "convert" the Arduino sketch, but that is of no consequence since it contains only comments.

## List of Projects      

All these projects except for `01_pin_names` control an external LED that is connected to one of the I/O pins of the XIAO ESP32C3.

| Project | Purpose |
| ---     | ---                           |
| **01_pin_names** | Show the content of `pins_arduino.h` |
| **02_blink_led** | Show the use of `digitalWrite` and `delay` |
| **03_button_led** | Show the use of `digitalWrite` and `digitalRead`  |
| **04_web_led** | Show the use of `WiFiServer` |
| **05_async_web_led** | Show the use of `ESPAsyncWebServer` and its [template processing capabilities](https://github.com/me-no-dev/ESPAsyncWebServer#template-processing) |
| **06_ble_led** | Show the use of the `ArduinoBLE` library |
| **07_ble_led** | Show the use of the `BLE` library contained in the ESP32 Arduino core |


## Project Notes

In both `04_web_led`  and `05_web_led` the SSID and password for the Wi-Fi network must be provided in a file called `secrets.h` in the directory containing the source code. There is a model called `secrets.h.template` in the same directory that can be edited and saved under the correct name. A similar MicroPython web server script described in [12. MicroPython: Web Controlled LED](https://sigmdel.ca/michel/ha/xiao/xiao_esp32c3_intro_en.html#web). In both cases, the XIAO ESP32C3 runs a Web server that makes it possible to toggle an external LED on and off through a Web interface. Something similar is described in [14. MicroPython: Web Controlled LED](https://sigmdel.ca/michel/ha/xiao/xiao_esp32c3_intro_en.html#web) where the firmware is written in MicroPython. The source is available here: [upy_web_led.zip]("https://sigmdel.ca/michel/ha/xiao/dnld/upy_web_led.zip").

Projects `03_button_led` and `05_async_web_led` rely on third-party libraries. They are made available in the [libraries/](libraries) directory for use with the Arduino IDE without the need to install anything. In PlatformIO, private copies of the libraries will be automatically installed. 

## License

Copyright 2023, 2024 Michel Deslierres. No rights reserved. 

While the copyright pertaining to included libraries must be respected, all the code by Michel Deslierres in this repository is in the public domain. In those jurisdictions where this may be a problem, the [BSD Zero Clause License](https://spdx.org/licenses/0BSD.html) applies.

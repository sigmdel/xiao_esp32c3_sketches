# XIAO ESP32C3 Arduino Sketches / PlatformIO Projects

***February 21, 2026***

**Source code that accompanies [First Look at the Seeed Studio XIAO ESP32C3](https://sigmdel.ca/michel/ha/xiao/xiao_esp32c3_intro_en.html)**.

*Note: That post, last updated in July 2024, is now out of date and will be reworked in the future.*

![XIAO ESP32C3 Pinout](images/xiao_esp32c3_pinout.png) 


**Table of Content**
<!-- TOC -->

- [1. Introduction](#1-introduction)
- [2. Arduino IDE Notes](#2-arduino-ide-notes)
  - [2.1. Third-party libraries](#21-third-party-libraries)
- [3. PlatformIO Notes](#3-platformio-notes)
  - [3.1. Third-party libraries](#31-third-party-libraries)
- [4. List of Projects](#4-list-of-projects)
- [5. Project Notes](#5-project-notes)
- [6. Related Work](#6-related-work)
- [7. License](#7-license)

<!-- /TOC -->
---

## 1. Introduction

The XIAO ESP32C3 is a relatively recent addition to the [Seeed Studio XIAO series](https://www.seeedstudio.com/xiao-series-page) of diminutive development boards. It is based on the Espressif ESP32-C3 SoC which has a RISC-V core.

It should be possible to compile each project with PlatformIO or in the Arduino IDE without any need to install anything other than this repository.

## 2. Arduino IDE Notes

Arduino sketches must have an `.ino` file name extension and must be contained in a directory that has the same name as the Arduino sketch (excluding the extension). Consequenty the `01_pin_names` project containts a directory named `pin_names` that in turn contains the Arduino sketch `pin_names.ino`. That sketch is basically empty as it is a long comment only. This is not a problem because the Arduino IDE will import all source files found in the sketch directory. The actual code is in `main.cpp` which is the default name of a PlatformIO project.

![Directory tree](images/dir_tree.jpg) 

To compile and then upload the sketch in the Arduino IDE, click on the **File** top menu, click on **Open...**, then navigate to the `pin_names.ino` file and open it with the system file manager.

### 2.1. Third-party libraries

Some sketches require third-party libraries. To make these sketches self-contained, private copies of the needed libraries are included in the [libraries](libraries/) directory. 

The Arduino IDE will find these private libraries if the `Sketchbook location` in the Arduino Preferences is set to the parent directory, `xiao_esp32c3_sketches`. Details can be found in [libraries/README.md](libraries/README.md).

Where a private library is included, only the necessary files were added. Please obtain the full library from the original source if it is to be used in another project.

## 3. Pioarduino Notes

Because of the Arduino sketch naming constraints, the `main.cpp` file of a project is not stored in the `src` directory. To work around this change, a `src_dir` entry is added in the `platformio.ini` file to provide the name of the directory in which `main.cpp` is found. That will be the name of the Arduino sketch as shown here for the `01_pin_names` project. 

```ini
[platformio]
; Make the Arduino IDE happy (.INO file must be in a directory of the same name)
src_dir = pin_names
...
```

Pioarduino will "convert" the Arduino sketch, but that is of no consequence since it contains only comments.

### 3.1. Third-party libraries

The local copies of third party libraries will be used by pioarduino by default. This is achieved by a  `lib_dir` entry in the configuration file where needed.

```ini
[platformio]
; Make the Arduino IDE happy (.INO file must be in a directory of the same name)
src_dir = async_web_led
lib_dir = ../libraries
...
```                            

## 4. List of Projects      

Except for `01_pin_names`, these projects control an external LED that is connected to one of the I/O pins of the XIAO ESP32C3.

| Project | Purpose |
| ---     | ---    
| **00_sys_info**    | Display information about the ESP32 system. |
| **01_pin_names** | Display the mapping of XIAO ESP32C3 Arduino pin names to gpio numbers and som useful macros. |
| **02_blink_led** | Use `digitalWrite`, `delay` and io interrupts to blink a LED attached to any I/O pad of the XIAO.|
| **03_button_led** | Use `digitalWrite` and `digitalRead` to toggle a LED on and off. |
| **04_web_led** | Use a Web server to toggle a LED on and off. |
| **05_async_web_led** | Use an asyncronous Web server to toggle a LED on and off. |
| **06_ble_led** | Use Bluetooth to toggle a LED on and off with the `ArduinoBLE` library |
| **07_ble_led** | Use Bluetoooth to toggle a LED on and off with the ESP32 Arduino core `BLE` librariers |


## 5. Project Notes

In both `04_web_led`  and `05_web_led` the SSID and password for the Wi-Fi network must be provided in a file called `secrets.h` in the directory containing the source code. There is a model called `secrets.h.template` in the same directory that can be edited and saved under the correct name.

When attempting to compile the `07_ble_led/ble_led` sketch in the Arduino IDE, it is necessary to 
move the local `libraries/ArduinoBLE` library outside the `libraries/` directory. Otherwise there will
be name conflicts with the ESP32-Arduino BLE libraries. More details in [libraries/README](libraries/README.md).

## 6. Related Work

There is a MicroPython web server script described in [14. MicroPython: Web Controlled LED](https://sigmdel.ca/michel/ha/xiao/xiao_esp32c3_intro_en.html#web) which is similar to the `04_web_led` project. The source is available here: [upy_web_led.zip]("https://sigmdel.ca/michel/ha/xiao/dnld/upy_web_led.zip").

It may be interesting to look at [supermini_esp32c3_sketches](https://github.com/sigmdel/supermini_esp32c3_sketches) which are about a class of slightly bigger ESP32-C3 development boards. That repository contains modified versions of projects such as `async_web_led` and `ble_led2` that can be used to investigate connectivity problems. Some Super Mini boards are known to have problems related to the circuit between the microcontroller and the onboard ceramic antenna or with the latter.

Finally, [xiao_esp32c6_sketches](https://github.com/sigmdel/xiao_esp32c6_sketches) and [xiao_esp32c5_sketches](https://github.com/sigmdel/xiao_esp32c5_sketches) are about the XIAO ESP32C6 and XIAO ESP32C5 development boards by Seeed Studio that are also based on a RISC-V microcontroller.

## 7. License

Copyright 2023-2026 Michel Deslierres. No rights reserved. 

While the copyright pertaining to included libraries must be respected, all the code by Michel Deslierres in this repository is in the public domain. In those jurisdictions where this may be a problem, the [BSD Zero Clause License](https://spdx.org/licenses/0BSD.html) applies.

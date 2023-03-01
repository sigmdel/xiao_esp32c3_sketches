/*
 * mdPushButton.h
 * 
 * A push button Arduino library capable of returning the number of
 * consecutive button presses made in quick succession or if the 
 * button was held down for a long time. 
 * 
 * Version 0.1.1, 2020-12-23.
 *
 * Michel Deslierres <sigmdel.ca/michel>
 * 
 * Version 0.1.1 
 *     - setXXXXTime(unint16_t value) functions now return the previous time value
 */
 
 // SPDX-License-Identifier: 0BSD

#ifndef MDPUSHBUTTON_H
#define MDPUSHBUTTON_H

#include "Arduino.h"  // needed for platformIO

// May need to define this macro in Arduino IDE if a value != 0 is desired
#ifndef DEBUG_PUSH_BUTTON
#define DEBUG_PUSH_BUTTON 0  // 0 - no debugging, 1 - printSetup() available, 2 adds state machine debugging
#endif

#define DEFAULT_DEBOUNCE_PRESS_TIME      15  // delay to debounce the make part of the signal
#define DEFAULT_DEBOUNCE_RELEASE_TIME    30  // delay to debounce the break part of the signal
#define DEFAULT_MULTI_CLICK_TIME        400  // if 0, does not check for multiple button clicks
#define DEFAULT_HOLD_TIME              2000  // minimum time of button press for mdButton.status() to return a -1 (long button press)
                                             // all times in milliseconds

// Callback type of handler such as buttonClicked(int clicks)
typedef void (*callback_int)(int);

// Callback type of handler such as buttonClicked(uint8_t buttonid, int clicks);
// the button id is the hopefully unique GPIO pin to which it is connected.
typedef void (*callback_int_int)(uint8_t, int);

class mdPushButton {
  public:
    mdPushButton(uint8_t pin, uint8_t active = LOW, bool useInternalPullResistor = true);  //constructor
   
    // Current library version
    int32_t version;

    // If value < 0xFFFF then sets the time attribute and returns its previous value.
    // If value == 0xFFFF returns the current time attribute
    uint16_t setDebouncePressTime(uint16_t value);
    uint16_t setDebounceReleaseTime(uint16_t value);
    uint16_t setMultiClickTime(uint16_t value);
    uint16_t setHoldTime(uint16_t value);

    // status, number of clicks since last update
    // -1 = button held, 0 = button not pressed, 1, 2, ... number of times button pressed
    int status();

    // Set callback function to be called when the button has been pressed   
    void OnButtonClicked(callback_int); // only one push button, or using separate click handlers for each button
    void OnButtonClicked(callback_int_int);  // when using one click handler for more than on push button

    #if (DEBUG_PUSH_BUTTON > 0)
    void printSetup(void);
  private:
    uint32_t _mode;
    #else
  private:
    #endif 
    uint8_t _pin;    
    uint8_t _active; 
    uint16_t _debouncePressTime   = DEFAULT_DEBOUNCE_PRESS_TIME; 
    uint16_t _debounceReleaseTime = DEFAULT_DEBOUNCE_RELEASE_TIME;
    uint16_t _multiClickTime      = DEFAULT_MULTI_CLICK_TIME;  
    uint16_t _holdTime            = DEFAULT_HOLD_TIME;
  
    callback_int _OnClick1;
    callback_int_int _OnClick2;

       // State variables 
    unsigned long lastButtonCheck_;
    long _eventTime; 
    enum buttonState_t { AWAIT_PRESS, DEBOUNCE_PRESS, AWAIT_RELEASE, DEBOUNCE_RELEASE, AWAIT_MULTI_PRESS } _buttonState;
    int _clicks;
    int _update(void);
    uint16_t _setAttrib(uint16_t* attrib, uint16_t value);

    // Debugging
    #if (DEBUG_PUSH_BUTTON > 1)
;   buttonState_t _lastState = AWAIT_MULTI_PRESS;
    void _debug(char * msg);
    #endif  
};

#endif

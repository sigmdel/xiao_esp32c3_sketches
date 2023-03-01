/*
 * mdButton.cpp
 * 
 * A push button Arduino library capable of returning the number of
 * consecutive button presses made in quick succession or if the 
 * button was held down for a long time. 
 * 
 * Version 0.1.1, 2020-12-23.
 *
 * Michel Deslierres <sigmdel.ca/michel>
 * 
 */
 
 // SPDX-License-Identifier: 0BSD

#include "Arduino.h"
#include "mdPushButton.h"

#define VERSION 0x000102

// If active = LOW (the default) then the input must be pulled high most of the time
//   This can be done with the internal pullup (set pullup = true which is also the default value) 
//   or an external pull up resistor say 4.7K + connected to the  GPIO pin and Vcc
//
// If active = HIGH then the input must be pulled low most of the time
//   This can be done with the internal pulldown (set pullup = true which is also the default value) 
//   or an external pull down resistor say 4.7K + connected to the  GPIO pin and Vcc

mdPushButton::mdPushButton(uint8_t pin, uint8_t active, bool useInternalPullResistor) {
  _pin = pin;
  _active = active;

  int mode = INPUT;
  if (useInternalPullResistor) {
    if (active == LOW)
      mode = INPUT_PULLUP;
    else {
      // mode = INPUT_PULLDOWN where defined
      #if defined(INPUT_PULLDOWN) 
        mode = INPUT_PULLDOWN;
      #elif defined(ESP8266)
        if (pin == 16) mode = INPUT_PULLDOWN_16;
      #endif  
    }
  }  

  #if (DEBUG_PUSH_BUTTON > 0)
  _mode = mode;
  #endif

  pinMode(_pin, mode);
  _buttonState = AWAIT_PRESS;
  version = VERSION;
}

//set time attributes

uint16_t  mdPushButton::_setAttrib(uint16_t* attrib, uint16_t value) {
  uint16_t time = *attrib;
  if (value < 0xFFFF)
    *attrib = value;
  return time;
}

uint16_t mdPushButton::setDebouncePressTime(uint16_t value) { return _setAttrib(&_debouncePressTime, value); }
uint16_t mdPushButton::setDebounceReleaseTime(uint16_t value) { return _setAttrib(&_debounceReleaseTime, value); }
uint16_t mdPushButton::setMultiClickTime(uint16_t value){ return _setAttrib(&_multiClickTime, value); }
uint16_t mdPushButton::setHoldTime(uint16_t value){ return _setAttrib(&_holdTime, value); }

void mdPushButton::OnButtonClicked( callback_int cb ){_OnClick1 = cb;}
void mdPushButton::OnButtonClicked( callback_int_int cb ){_OnClick2 = cb;}

int mdPushButton::status(void) {
  int stat = _update();
  if (stat) {
    if (_OnClick2) _OnClick2(_pin, stat);
    if (_OnClick1) _OnClick1(stat);
  }  
  return stat;
}  

int mdPushButton::_update(void) {
  if (_buttonState == AWAIT_PRESS) {
    _clicks = 0;
    if (digitalRead(_pin) == _active) {
      _buttonState = DEBOUNCE_PRESS;
      _eventTime = millis();  
    }  
  } 
  
  else if (_buttonState == DEBOUNCE_PRESS) {
    if ((millis() - _eventTime) > _debouncePressTime) { 
      _buttonState = AWAIT_RELEASE;
      _eventTime = millis(); 
    }  
  } 
  
  else if (_buttonState == AWAIT_RELEASE) {
    //dbg: Serial.printf("%d ", digitalRead(_pin));
    if (!(digitalRead(_pin) == _active)) {
      if ((millis() - _eventTime) > _holdTime) {
        _clicks = -1;
      }
      _buttonState = DEBOUNCE_RELEASE;
      _eventTime = millis();
    }
  } 

  else if (_buttonState == DEBOUNCE_RELEASE) {
    if ((millis() - _eventTime) > _debounceReleaseTime) { 
      if (_clicks < 0) {
        _buttonState = AWAIT_PRESS;
        #if (DEBUG_PUSH_BUTTON  > 1)
        _debug(" return -1");
        #endif
        return -1;
      }  
      _clicks += 1;
      _buttonState = AWAIT_MULTI_PRESS;
      _eventTime = millis(); 
    }  
  } 
  
  else {   // (buttonState == AWAIT_MULTI_PRESS) 
    if (digitalRead(_pin) == _active) {
      _buttonState = DEBOUNCE_PRESS;
      _eventTime = millis(); 
    } 
    else if ((millis() - _eventTime) > _multiClickTime) {
      _buttonState = AWAIT_PRESS;
        #if (DEBUG_PUSH_BUTTON  > 1)
        _debug(" return _clicks");
        #endif
      return _clicks;
    } 
  }
  #if (DEBUG_PUSH_BUTTON > 1)
  _debug(" return 0");
  #endif  
  return 0; 
}

#if (DEBUG_PUSH_BUTTON > 0)

void mdPushButton::printSetup(void) {
  Serial.print("mdPushButton library version ");
  Serial.print(version >> 16);
  Serial.print(".");
  Serial.print(version >> 8 & 0xFF);
  Serial.print(".");
  Serial.println(version & 0xFF);
  Serial.print("Button pin number ");
  Serial.print(_pin);
  Serial.print(", active ");
  Serial.print((_active == HIGH) ? "HIGH" : "LOW");
  Serial.print(", mode ");
  switch(_mode) {
    case INPUT: Serial.println("INPUT"); break;
    #if defined(INPUT_PULLDOWN) 
    case INPUT_PULLDOWN: Serial.println("INPUT_PULLDOWN"); break;
    #endif
    #if defined(INPUT_PULLDOWN) 
    case INPUT_PULLDOWN: Serial.println("INPUT_PULLDOWN"); break;
    #endif
    #if defined(ESP8266)   
    case INPUT_PULLDOWN_16: Serial.println("INPUT_PULLDOWN_16"); break;
    #endif
    case INPUT_PULLUP: Serial.println("INPUT_PULLUP"); break;
    case OUTPUT: Serial.println("OUTPUT"); break;
    default: Serial.print("Unknown #"); Serial.println(_mode); break;
  }
}

#if (DEBUG_PUSH_BUTTON > 1)

void mdPushButton::_debug(char * msg = NULL) {
  if (_lastState != _buttonState) {
    Serial.print("DBG pushButton: _pin ");
    Serial.print(_pin);
    Serial.print(" _clicks = ");
    Serial.print(_clicks);
    Serial.print(" _buttonState = ");
    switch(_buttonState) {
      case AWAIT_PRESS: Serial.print("AWAIT_PRESS"); break;
      case DEBOUNCE_PRESS: Serial.print("DEBOUNCE_PRESS");  break;
      case AWAIT_RELEASE: Serial.print("AWAIT_RELEASE");  break; 
      case DEBOUNCE_RELEASE: Serial.print("DEBOUNCE_RESLEASE");  break;
      case AWAIT_MULTI_PRESS: Serial.print("AWAIT_MULTI_PRESS");  break;
      default: Serial.print("*** UNKNOWN ***");
    };
    Serial.println(msg);
  }
  _lastState = _buttonState;  
}    

#endif
#endif


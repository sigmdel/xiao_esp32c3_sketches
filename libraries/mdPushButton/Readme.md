# mdPushButton Library

An [Arduino](https://www.arduino.cc/) library capable of returning the number of consecutive 
presses of a push button made in quick succession or reporting if the 
button was held down for a long time.

## Original Version

The original version of this library was made available elsewhere under 
the name [mdButton.zip](https://sigmdel.ca/michel/program/esp8266/arduino/switch_debouncing_en.html#downloads). 

## mdPushButton Class Constructor

An instance of the mdPushButton class must be create for each
push button. The GPIO pin connected to one terminal of the button must 
be specified in the constructor.

    mdPushButton(uint8_t pin, uint8_t active = LOW, bool useInternalPullResistor = true);

It is assumed that when pressed, the signal from the button is
brought `LOW`, but that can be changed by `active` to `HIGH`. It is
also assumed that an internal pull-up resistor will be used to 
keep the GPIO pin `HIGH` when the push button is not activated. If
an external pull-up resistor is in place, `useInternalPullResistor`
can be set to `false`.

If the push button brings the `active HIGH` and `useInternalPullResistor` 
is `true` then an internal pull-down resistor will be activated if
that is supported.

## Polling

The state of the push button is updated when the function 

    int status(void)
    
is invoked. This function must be called at regular intervals which
is usually done in the `loop()` function of the sketch. The value 
returned by the function indicates the state of the push button:

    -1   button held down for a long time
     0   button is not pressed
     1+  number of consecutive times the button has been pressed

## Delays

A debounce delay is used when the button is pressed and when
the button is released. These attributes may be set with the 
functions

    void setDebouncePressTime(uint16_t value);
    void setDebounceReleaseTime(uint16_t value);

The time values are in milliseconds (ms).

The library waits a few milliseconds after debouncing a button
release to see if there is a subsequent button press. This wait
period is set with the function

    void setMultiClickTime(uint16_t value);
    
If this time is too short then instead of reporting a number of
consecutive button presses, the library will report distinct single
button presses. If the time is too long, the library will seem 
unresponsive.

Just how long the button must be held for a long press is set
with the function 

    void setHoldTime(uint16_t value);
   
  
The default for all these time values are as follows:

	#define DEFAULT_DEBOUNCE_PRESS_TIME      15  // delay to debounce the make part of the signal
	#define DEFAULT_DEBOUNCE_RELEASE_TIME    30  // delay to debounce the break part of the signal
	#define DEFAULT_MULTI_CLICK_TIME        400  // if 0, does not check for multiple button clicks
	#define DEFAULT_HOLD_TIME              2000  // minimum time of button press for mdButton.status() to return a -1 (long button press)

Again, all times are in milliseconds.

The physical properties of push buttons can be considerable even when comparing two switches 
of the same type made by the same producer. The default time values are conservative. Use the 
`push_button_tune.ino` sketch in the `examples` directory to test other delay values with a given switch.


## Callback Functions

Instead of testing the returned value of `status()` to see if the button 
has been pressed, a callback function can be specified. The simplest
form of a call back will be `void` function with a single integer
parameter which will be the returned number of button presses.

    void buttonPressed(int clicks)

This callback is assigned with the 'OnButtonPressed()` method.

    void OnButtonClicked(buttonPressed);

It is also possible to install a callback function with two integer
parameters. The first will be the GPIO pin number of the button,
the second the number of consecutive button presses. This is useful
when a single handler for multiple buttons is used.

    void buttonPressed2(uint8_t pin, int clicks)
    
This callback is assigned with an overloaded version of the 
'OnButtonPressed()` method.

    void OnButtonClicked(buttonPressed2);
    

# Examples

Hopefully, the examples illustrate how to use this simple library. 
The first, `push_button_basic.ino` uses the returned value from the
`status()` function directly and must therefore correctly handle a 
returned `0` which indicates that no button has been pressed. 

The second example, `push_button_callback.ino` is similar but uses a 
callback function which will never have to handle a no button pressed 
state. Note that `status()` must nevertheless be called in the 
sketch `loop()`. 


The third example, `push_button_callback2.ino` shows how a single callback can 
be used with multiple buttons.

The `push_button_tune.ino` sketch is used to find the appropriate debounce time
values and so on for a given switch. Connect the switch to an appropriate micro-controller 
development board with a supported serial port. Enter `help` in the serial
monitor for a list of commands.


# Credits

This an implementation of a debounce algorithm by Jack G. Gannsle 
described in 
[A Guide to Debouncing](http://www.eng.utah.edu/~cs5780/debouncing.pdf) 
(2004-2008). More details are available at 
[Detecting Multiple Button Clicks in an Arduino Sketch](https://sigmdel.ca/michel/program/esp8266/arduino/switch_debouncing_en.html).


# Licence

The **BSD Zero Clause** ([SPDX](https://spdx.dev/): [0BSD](https://spdx.org/licenses/0BSD.html)) licence applies.





# analog Keyboard #

Read a keyboardvalue from resistor array via one analog port of a ATMEGA.
So you can build up very simple small keyboards with up to 30 keys.

## Principle ##

If a key is pressed, the analog input reads a value greater than 10.
If the read value is same for at least reliable_time and after that the
key is released, the method returns the key number (starting with 0).

If the time is not reached the method returns 255 as neutral value.

If the input value is not stable so it differs by more than <accuracy> from the last read, the timecounter to reach the reliable_time is reset.

If a key is pressed for longer than the time set in <longpressed_time> the
function returns a value which is 128 + keyNr that was pressed. Otherwise 255 is returned as neutral result.


## Circuit schema ##

the whole array should have about 5 to 10 kOhm resistance. Here is an
example with 5 keys

```bash

                  1k        1k        1k        1k        1k
(+5v)  --------+---[R]---+---[R]---+---[R]---+---[R]---+---[R]----+
               |         |         |         |         |          |
               |         |         |         |         |          |
               \ key4    \ key3    \ key2    \ key1    \ key0     |
                \         \         \         \         \         |
               |         |         |         |         |          |
               |         |         |         |         |          |
 (analogIn) ---+---------+---------+---------+---------+---[R]----+
                                                           10k    |
                                                                 _|_ (GND)

```
## Usage ##

### Initialization
initialize like this:

```c++
#include<AnalogKbd.h>
#define PIN_ANALOG_KBD   0  // number of analog pin of ATMEGA
#define KBD_NR_OF_KEYS   4  // how many keys are built up in the circuit (max about 20)

AnalogKbd kbd(PIN_ANALOG_KBD, KBD_NR_OF_KEYS);
byte kbdValue = 255; //the value that is read from keyboard

```

### Use read() in a loop ###

Call the function kbd.read() somewhere in a loop. The method must be called at least each 10 milliseconds.


```c++
while (true)
{
    //other code
    kbdValue = kbd.read();
    if (kbdValue != 255)
    {
        // everything from 255 means, a valid keypress has been detected.
        // do something...
    }
    delay(10);
}
```

### wait until a key is pressed ###

```c++
//do sth.
kbdValue = kbd.wait_until_read();
//do next thing after a key has been pressed
```

(c) Johannes Benoit 2016-2022



// **************************************************************************
//
// Read a keyboardvalue from resistor array via one analog port of a ATMEGA.
// So you can build up very simple small keyboards with up to 20 keys.
//
//                                                        Johannes Benoit 2016
// ***************************************************************************

#ifndef ANALOGKBD_H
#define ANALOGKBD_H

#include "Arduino.h"


class AnalogKbd
{
    public:
        AnalogKbd(byte input_pin, byte nr_of_keys, byte reliable_time, int longpressed_time);

        // analyze the inputvalue and return key if result is reliable:
        byte read();
        // same, but waits in a loop until a value is returned:
        byte wait_till_read();

        // transform the analog read to a raw key number
        byte mapValue(int value);


    protected:
    private:

        // methods

        //passed parameters
        byte _input_pin;
        byte _nr_of_keys;
        byte _reliable_time;
        int _longpressed_time;
 
        // reads from analog pin
        int _kbdRead;
        int _kbdLastRead;

        // resulted value to return
        byte _kbdResult;

        //flags
        boolean _kbdIsValid;
        boolean _kbdWasReleased;

        // times for analyze the results
        long _kbdChangeTime;
        long _validateTime;
        long _kbdTime;

};

#endif // ANALOGKBD_H

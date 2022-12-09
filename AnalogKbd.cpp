#include "AnalogKbd.h"

// some parameters that control the behaviour

#define ACCURACY 5 // maximum difference tolerated between two reads of ad-value
#define KBD_RELIABLE_TIME_DELTA     40   // ms a key must be pressed
#define KBD_LONGPRESS_TIME_DELTA    600  // ms a key must be pressed for long value


AnalogKbd::AnalogKbd(byte input_pin, byte  nr_of_keys)
{
    _input_pin = input_pin;
    _nr_of_keys = nr_of_keys;

    _kbdRead = 0;
    _kbdLastRead = 0;
    _kbdResult = 255;

    _kbdIsValid = false;
    _kbdWasReleased = false;

    _kbdChangeTime = millis();
    _validateTime = millis();
    _kbdTime = millis();

}

byte AnalogKbd::mapValue(int value) {
    return map(value - 512/_nr_of_keys, 0, 1025, 0, _nr_of_keys);
}

byte AnalogKbd::wait_till_read() {
    byte _result = 255;
    while (_result == 255)
    {
        _result = read();
        delay(20);
    }
    return _result;
}

byte AnalogKbd::read() {
    _kbdRead = analogRead(_input_pin);
    _kbdTime = millis();
    if (_kbdRead < 20) {
        // read is zero

        if (_kbdWasReleased == false) {
            // change state to wasReleased = true
            //Serial.println("kbd : read " + String(_kbdRead) + " released");
            _kbdWasReleased = true;
            _kbdLastRead = 0;

            if (_kbdIsValid == true) {
                // if a valid value is on hold, return it first time key is released
                _kbdIsValid = false;
                //Serial.println("kbd :  --> result " + String(_kbdResult));
                return _kbdResult;
            }
        }
    }


    else {
        // read is non zero
        // Serial.println("read " + String(_kbdRead));
        if (abs(_kbdRead - _kbdLastRead) > ACCURACY) {
            // if this read is different to the one before, start set time

            _kbdChangeTime = millis();
        }
        else {
            // the value is unchanged
            if (_kbdWasReleased == true) {
                // there is no value on hold, the key has been released since last verification

                if (_kbdTime - _kbdChangeTime > KBD_RELIABLE_TIME_DELTA) {
                    _kbdResult = mapValue(_kbdRead);
                    _lastAdValue = _kbdRead;

                    // read the same value for at least 70ms...
                    // fix the value don't deliver it but keep it on hold
                    _kbdWasReleased = false;
                    _kbdIsValid = true;
                    _validateTime = _kbdTime; // store the time when validated
                    //Serial.println("kbd : read " + String(_kbdRead) + " --> valid.  (" + String(_kbdResult) + ")");
                }
            }
            else {
                if ((_kbdTime - _validateTime > KBD_LONGPRESS_TIME_DELTA) and _kbdIsValid) {
                    // if key is pressed longer than defined, return 128 + keynumber
                    _kbdResult = 128 + _kbdResult;

                    _kbdWasReleased = false;
                    _kbdIsValid = false;
                    return _kbdResult;

                }
            }
        }
    }
    _kbdLastRead = _kbdRead;
    return 255;
}

int AnalogKbd::getLastAdValue() {
    return _lastAdValue;
}



#include "AnalogKbd.h"

// switch to actiavate debugging code if Serial interface is available
//#define DEBUG_ON_SERIAL 1

// maximum difference tolerated between two reads
#define ACCURACY 10
AnalogKbd::AnalogKbd(byte input_pin, byte  nr_of_keys, byte reliable_time, int longpressed_time)
{
    _input_pin = input_pin;
    _nr_of_keys = nr_of_keys;
    _reliable_time = reliable_time;
    _longpressed_time = longpressed_time;

    _kbdRead = 0;
    _kbdLastRead = 0;
    _kbdResult = 255;

    _kbdIsValid = false;
    _kbdWasReleased = false;

    _kbdChangeTime = millis();
    _validateTime = millis();
    _kbdTime = millis();

}

byte AnalogKbd::mapValue(int value)
{
    return map(value - 512/_nr_of_keys, 0, 1025, 0, _nr_of_keys);
}

byte AnalogKbd::wait_till_read()
{
    byte _result = 255;
    while (_result == 255)
    {
        _result = read();
        delay(20);
    }
    return _result;
}

byte AnalogKbd::read()
{
    _kbdRead = analogRead(_input_pin);
    _kbdTime = millis();
    if (_kbdRead < 20)
    {
        // read is zero

        if (_kbdWasReleased == false)
        {
            //change state to wasReleased = true
#ifdef DEBUG_ON_SERIAL
            Serial.println("kbd : read " + String(_kbdRead) + " released");
#endif
            _kbdWasReleased = true;
            _kbdLastRead = 0;

            if (_kbdIsValid == true)
            {
                // if a valid value is on hold, return it first time key is released
                _kbdIsValid = false;
#ifdef DEBUG_ON_SERIAL
                Serial.println("kbd :  --> result " + String(_kbdResult));
#endif
                return _kbdResult;
            }
        }
    }


    else
        // read is non zero
    {
#ifdef DEBUG_ON_SERIAL
   Serial.println("read " + String(_kbdRead));
#endif // DEBUG_ON_SERIAL
        if (abs(_kbdRead - _kbdLastRead) > ACCURACY)
        {
            // if this read is different to the one before, start set time

            _kbdChangeTime = millis();
#ifdef DEBUG_ON_SERIAL
            byte dbgMap =  mapValue(_kbdRead);
            Serial.println("kbd : read " + String(_kbdRead) + " different (" + String(dbgMap) + ")");
#endif // DEBUG_ON_SERIAL

        }
        else
        {
            // the value is unchanged
            if (_kbdWasReleased == true)
            {
                // there is no value on hold, the key has been released since last verification

#ifdef DEBUG_ON_SERIAL
                byte dbgMap =  mapValue(_kbdRead);
                Serial.println("kbd : read " + String(_kbdRead) + " same. (" + String(dbgMap) + ")");
#endif // DEBUG_ON_SERIAL
                if (_kbdTime - _kbdChangeTime > _reliable_time)
                {
                    _kbdResult = mapValue(_kbdRead);
                    _lastAdValue = _kbdRead;

                    // read the same value for at least 70ms...
                    // fix the value don't deliver it but keep it on hold
                    _kbdWasReleased = false;
                    _kbdIsValid = true;
                    _validateTime = _kbdTime; // store the time when validated
#ifdef DEBUG_ON_SERIAL
                    Serial.println("kbd : read " + String(_kbdRead) + " --> valid.  (" + String(_kbdResult) + ")");
#endif // DEBUG_ON_SERIAL
                }
            }
            else
            {
                if ((_kbdTime - _validateTime > _longpressed_time) and _kbdIsValid)
                {
                    // if key is pressed longer than defined, return 128 + keynumber
                    _kbdResult = 128 + _kbdResult;

                    _kbdWasReleased = false;
                    _kbdIsValid = false;
#ifdef DEBUG_ON_SERIAL
                    Serial.println("kbd : read " + String(_kbdRead) + " long timeDelta " + String(_kbdTime - _validateTime));
                    Serial.println("kbd :  --> result " + String(_kbdResult));
#endif // DEBUG_ON_SERIAL
                    return _kbdResult;

                }
            }
        }
    }
    _kbdLastRead = _kbdRead;
    return 255;
}

int AnalogKbd::getLastAdValue()
{
    return _lastAdValue;
}



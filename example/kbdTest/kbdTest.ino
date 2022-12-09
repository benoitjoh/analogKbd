// small setup to test AnalogKbd.h

#include<AnalogKbd.h>

#define PIN_ANALOG_KBD   0  // ad0 for input of analog Keyboard...
#define KBD_NR_OF_KEYS   9  // how many keys are built up in the circuit

AnalogKbd kbd(PIN_ANALOG_KBD, KBD_NR_OF_KEYS);
signed int kbdValue = 0; //the value that is read from keyboard


void setup() {

    Serial.begin(9600);
    while (!Serial) {
        ; // wait for serial Pin to connect. otherwise reset if serial console is started :-/
    }

    Serial.write("start: \nThese are the limits between the ad-values to distinguish beween keys:\n");
    byte resOld = -1;
    for (int i=0; i<1024; i++)
    {
        byte res = kbd.mapValue(i);
        if (res != resOld)
        {
            Serial.println("advalue > " + String(i) + " -->  key " + String(res) );
            resOld = res;
        }
    }
    pinMode(13, OUTPUT);
}

void loop() {

    kbdValue = kbd.read();

    if (kbdValue != 255) {
        digitalWrite(13, 1); // short blink
        Serial.println("Key Nr.:" + String(kbdValue) + "  ADC-Value: " + String(kbd.getLastAdValue()) );
        switch (kbdValue) {
            case 0:
                Serial.println(" .. some action if key 0 pressed....");
            break;
            case 128:
                Serial.println(" ..some action if key 0 pressed long .");
            break;
        }
        if (kbdValue > 127) {
            delay(100); // if long keypress, long blink
        }
    }        
    delay(10);
    digitalWrite(13, 0);

}

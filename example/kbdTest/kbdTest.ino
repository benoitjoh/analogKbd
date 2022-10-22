// small setup to test AnalogKbd.h
#include<AnalogKbd.h>

#define PIN_ANALOG_KBD   0  // ad0 for input of analog Keyboard...
#define KBD_NR_OF_KEYS   5  // how many keys are built up in the circuit
#define KBD_RELIABLE_TIME_DELTA     30   // ms a key must be pressed
#define KBD_LONGPRESS_TIME_DELTA    600  // ms a key must be pressed for long value

AnalogKbd kbd(PIN_ANALOG_KBD, KBD_NR_OF_KEYS, KBD_RELIABLE_TIME_DELTA, KBD_LONGPRESS_TIME_DELTA);
signed int kbdValue = 0; //the value that is read from keyboard

// using the standard LCD Library
#include <LiquidCrystal.h>
//            lcd(rs, en, d4, d5, d6, d7); backlight: pin D6
LiquidCrystal lcd(8,  7,   9, 10, 11, 12);


void setup() 
{
    lcd.begin(16,2);               // initialize the lcd
    lcd.home();                   // go home
    lcd.print("KBD Test");
 
    Serial.begin(9600);
    while (!Serial)
    {
        ; // wait for serial Pin to connect. otherwise reset if serial console is started :-/
    }
    Serial.write("start: \nThese are the lower limits:\n");
    byte resOld = -1;
    for (int i=0; i<1024; i++)
    {
        byte res = kbd.mapValue(i);
        if (res != resOld)
        {
            Serial.println("map i>" + String(i) + " -->  " + String(res) );
            resOld = res;
        }
    }
    resOld = kbd.wait_till_read();
}

void loop() {
    delay(30);
    kbdValue = kbd.read();
    if (kbdValue != 255)
    {  
       //int adValue = analogRead(PIN_ANALOG_KBD);
       Serial.println("Result: " + String(kbdValue));
       lcd.setCursor(0,1);
       lcd.println("Res.: " + String(kbdValue) + " ad: " + String(kbd.getLastAdValue()));
    }
}

// small setup to test AnalogKbd.h
#include<AnalogKbd.h>

#define PIN_ANALOG_KBD   0  // ad0 for input of analog Keyboard...
#define KBD_NR_OF_KEYS   5  // how many keys are built up in the circuit

AnalogKbd kbd(PIN_ANALOG_KBD, KBD_NR_OF_KEYS);
signed int kbdValue = 0; //the value that is read from keyboard

// using the standard LCD Library
#include <LiquidCrystal.h>
//            lcd(rs, e,  d4, d5, d6, d7); 
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
    resOld = kbd.wait_till_read();
}

void loop() {
    delay(30);
    kbdValue = kbd.read();
    if (kbdValue != 255)
    {  
       lcd.setCursor(0,1);
       lcd.println("Key:" + String(kbdValue) + " ADC: " + String(kbd.getLastAdValue()));
       Serial.println("Key:" + String(kbdValue) + " ADC: " + String(kbd.getLastAdValue()) );
       switch (kbdValue)
          {
          case 0:
              // key 0 pressed...       
              Serial.println(" Key 0 pressed.");
 
              break;
          case 128:
              Serial.println(" Key 0 pressed long .");
             
              break;
          }
    }
}

#include <Arduino.h>

HardwareSerial PCSerial;
HardwareSerial PSoCSerial;

static const boolean echo = false;

void setup()
{
    PCSerial.begin(9600);
    PSoCSerial.begin(9600);
}

void loop()
{
    static int counter = 0;
    static char charFromPC = '\0';
    static char charFromPSoC = '\0';

    if (echo)
    {
        if (PCSerial.available() > 0)
        {
            charFromPC = PCSerial.read();
            PSoCSerial.print(charFromPC);
        }
        if (PSoCSerial.available() > 0)
        {
            charFromPSoC = PSoCSerial.read();
            PCSerial.print(charFromPSoC);
        }
    }
    else if (counter <= 2)
    {
        PCSerial.println("a");
        PSoCSerial.println("b");
        delay(200);
        counter++;
    }
}
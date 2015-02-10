#include <Arduino.h>

void setup()
{
    Serial.begin(9600);
    Serial1.begin(9600);
    /* add setup code here */
}

int counter = 0;

void loop()
{
    if (counter <= 2)
    {
        Serial.println("a");
        Serial1.println("b");
        delay(200);
        counter++;
    }
}

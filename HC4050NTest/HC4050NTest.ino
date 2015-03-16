#include <Arduino.h>

const int pwmPin = 9;

void setup()
{

  pinMode(pwmPin, OUTPUT);
  analogWrite(pwmPin, 128);

}

void loop()
{
  /* add main program code here */

}

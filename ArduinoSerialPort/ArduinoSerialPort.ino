#include <Arduino.h>
#include <HardwareSerial.h>

HardwareSerial* PCSerial;
HardwareSerial* PSoCSerial;

static const boolean echo = true;

void setup()
{
  PCSerial = &Serial;
  PCSerial->begin(9600);
  PSoCSerial = &Serial1;
  PSoCSerial->begin(9600);
  delay(1000);
  PCSerial->println("Serial ready!");
}

void loop()
{
  static int counter = 0;
  static char charFromPC = '\0';
  static char charFromPSoC = '\0';

  if (echo)
  {
    if (PCSerial->available() > 0)
    {
      charFromPC = PCSerial->read();
      PSoCSerial->print(charFromPC);
      // PCSerial->print(charFromPC);
    }
    if (PSoCSerial->available() > 0)
    {
      charFromPSoC = PSoCSerial->read();
      PCSerial->print(charFromPSoC);
    }
  }
  else if (counter <= 2)
  {
    PCSerial->print('a');
    PSoCSerial->print('b');
    delay(200);
    counter++;
  }
}
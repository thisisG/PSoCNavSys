/*
**
** NavSys library
** URL: https://github.com/thisisG
** Author: Geir Istad (geir dot istad at gmail dot com)
** Licence: MIT
**
*/

#ifdef __cplusplus
extern "C" {
#endif
#include "gpsinterface.h"
#ifdef __cplusplus
}
#endif

#ifdef _WIN32

#endif // _WIN32

uint8_t decodeGpsStringInNavState(NavState* navS)
{
  uint8_t success = 0;
  // Need to pass the address to the second char value since the 0th is
  // occupied by '$'
  int pack = nmea_pack_type(&(navS->gpsBuffer.gpsStringBuffer)[1],
                            navS->gpsBuffer.gpsBufferLength);

  // Check if the string is identified as a type we can decode and use and get
  // the data if the string type is recognized.
  if (pack == GPRMC)
  {
    success = 1;
    nmeaGPRMC gprmcBuffer;
    nmea_zero_GPRMC(&gprmcBuffer);
    nmea_parse_GPRMC((navS->gpsBuffer.gpsStringBuffer),
                     navS->gpsBuffer.gpsBufferLength, &gprmcBuffer);

    nmeaINFO nmeaBuffer;
    nmea_zero_INFO(&nmeaBuffer);
    nmea_GPRMC2info(&gprmcBuffer, &nmeaBuffer);

    nmeaInfoToCoord(&nmeaBuffer, &(navS->currentLocation));
    nmeaTimeInfoToSysTime(&nmeaBuffer, &(navS->time));

    navS->dCurrentHeading = (floatDegree)nmeaBuffer.direction;

    navS->currentSpeedKmh = (floatDegree)nmeaBuffer.speed;
  }
  else
  {
    // Do nothing, case here for completion
  }
  return success;
}

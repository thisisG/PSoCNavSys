/*
**
** NavSys library
** URL: https://github.com/thisisG
** Author: Geir Istad (geir dot istad at gmail dot com)
** Licence: MIT
**
*/

#ifndef GPSINTERFACE_H
#define GPSINTERFACE_H

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

#include "navtypes.h"
#include "../nmea/nmea.h"
#include "nmeafunctions.h"
#include "navfunctions.h"

#ifdef __cplusplus
}
#endif // __cplusplus

#ifdef _WIN32 // Windows specific implementations
#endif        // _WIN32

#ifdef __GNUC__ // ARM specific implementation
#endif          // __GNUC__
/*
input:
  (ptr) NavState
output:
  (uint8_t) 1 if decode of string into NavState was successful
  navS->currentLocation updated to received location
  navS->time updated to received UTC time
  navS->dCurrentHeading updated with current GPS heading
  navS->currentSpeedKmh updated to current GPS speed
remarks:
  Will check if the received string in (navS->gpsBuffer.gpsStringBuffer) is a
  valid NMEA RMC string. If it is it will decode the string and update the
  internal data of NavState to the one received.
  If the received string is not a NMEA RMC string the string is discarded.
*/
uint8_t decodeGpsStringInNavState(NavState* navS);

#endif

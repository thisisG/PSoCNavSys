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
remarks:

*/
uint8_t decodeGpsStringInNavState(NavState* navS);

#endif

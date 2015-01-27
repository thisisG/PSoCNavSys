#ifndef GPSINTERFACE_H
#define GPSINTERFACE_H

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

#include "nmea\nmea.h"
#include "nmeafunctions.h"
#include "navfunctions.h"

#ifdef __cplusplus
}
#endif // __cplusplus

#ifdef _WIN32 // Windows specific implementations
#endif // _WIN32

#ifdef __GNUC__ // ARM specific implementation
#endif // __GNUC__

void gpsStringToNavState(
    const char* gpsString, const int gpsStringSize, nmeaPACKTYPE pack, NavState* navS);

#endif
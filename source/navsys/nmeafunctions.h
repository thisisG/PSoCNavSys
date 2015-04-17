/*
**
** NavSys library
** URL: https://github.com/thisisG
** Author: Geir Istad (geir dot istad at gmail dot com)
** Licence: MIT
**
*/

#ifndef NMEAFUNCTIONS_H
#define NMEAFUNCTIONS_H

#ifdef __cplusplus
extern "C" {
#endif

#include <math.h>
#include "navfunctions.h"
#include "navtypes.h"
#include "../nmea/nmea.h"

#ifdef __cplusplus
}
#endif

// TODO
floatDegree nmeaFloatFromLongDegree(const signed16Degree degree,
                                    const signed32Degree minutes);

// TODO
floatDegree nmeaLatitudeFromCoord(const struct Coordinate* thisCoord);

// TODO
floatDegree nmeaLongitudeFromCoord(const struct Coordinate* thisCoord);

// TODO
floatDegree floatFromNmeaDegree(const floatDegree nmeaDeg);

// TODO
void longDegreeFromNmeaFloat(const floatDegree nmeaDeg,
                             signed16Degree* ptrDegree,
                             signed32Degree* ptrMinutes);

// TODO
void longLatitudeFromNmeaInfo(nmeaINFO* nmeaInfo, signed16Degree* ptrDegree,
                              signed32Degree* ptrMinutes);

// TODO
void longLongitudeFromNmeaInfo(nmeaINFO* nmeaInfo, signed16Degree* ptrDegree,
                               signed32Degree* ptrMinutes);

// TODO
void nmeaInfoToCoord(nmeaINFO* nmeaInfo, Coordinate* coord);

// TODO
void nmeaTimeInfoToSysTime(const nmeaINFO* nmeaInfo, SystemTime* time);
#endif

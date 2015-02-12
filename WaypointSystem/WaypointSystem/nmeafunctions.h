#ifndef NMEAFUNCTIONS_H
#define NMEAFUNCTIONS_H

#ifdef __cplusplus
extern "C" {
#endif

#include <math.h>
#include "navfunctions.h"
#include "navtypes.h"
#include "./nmea/nmea.h"

#ifdef __cplusplus
}
#endif

floatDegree nmeaFloatFromLongDegree(const signed16Degree degree,
                                    const signed32Degree minutes);
floatDegree nmeaLatitudeFromCoord(const struct Coordinate* thisCoord);
floatDegree nmeaLongitudeFromCoord(const struct Coordinate* thisCoord);

floatDegree floatFromNmeaDegree(const floatDegree nmeaDeg);

void longDegreeFromNmeaFloat(const floatDegree nmeaDeg,
                             signed16Degree* ptrDegree,
                             signed32Degree* ptrMinutes);

void longLatitudeFromNmeaInfo(nmeaINFO* nmeaInfo, signed16Degree* ptrDegree,
                              signed32Degree* ptrMinutes);

void longLongitudeFromNmeaInfo(nmeaINFO* nmeaInfo, signed16Degree* ptrDegree,
                               signed32Degree* ptrMinutes);

void nmeaInfoToCoord(nmeaINFO* nmeaInfo, Coordinate* coord);
#endif
#ifndef NMEAFUNCTIONS_H
#define NMEAFUNCTIONS_H

#ifdef __cplusplus
extern "C" {
#endif

#include "navfunctions.h"
#include "navtypes.h"
#include "./nmea/nmea.h"

#ifdef __cplusplus
}
#endif

floatDegree nmeaFloatFromLongDegree(const signed16Degree degree, const signed32Degree minutes);
floatDegree nmeaLatitudeFromCoord(const struct Coordinate* thisCoord);
floatDegree nmeaLongitudeFromCoord(const struct Coordinate* thisCoord);

#endif
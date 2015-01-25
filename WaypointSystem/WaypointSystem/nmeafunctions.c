#ifdef __cplusplus
extern "C" {
#endif
#include "nmeafunctions.h"
#ifdef __cplusplus
}
#endif

floatDegree nmeaFloatFromLongDegree(const signed16Degree degree, const signed32Degree minutes)
{
    floatDegree fracDeg = 100.0 * (floatDegree)(degree);
    floatDegree fracMin = (((floatDegree)(minutes)) / 10000);

    return fracDeg + fracMin;
}

floatDegree nmeaLatitudeFromCoord(const struct Coordinate* thisCoord)
{
    return nmeaFloatFromLongDegree((thisCoord->dLatitude), (thisCoord->mLatitude));
}

floatDegree nmeaLongitudeFromCoord(const struct Coordinate* thisCoord)
{
    return nmeaFloatFromLongDegree((thisCoord->dLongitude), (thisCoord->mLongitude));
}
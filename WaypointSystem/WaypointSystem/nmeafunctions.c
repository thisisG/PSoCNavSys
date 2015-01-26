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

floatDegree floatFromNmeaDegree(const floatDegree nmeaDeg)
{
    // Get the degrees using an int typecast to remove fractions
    floatDegree tempValue = (int)(nmeaDeg / 100);
    // Extract minutes and add to tempValue
    tempValue = tempValue + (((nmeaDeg / 100) - tempValue) / 60);
    return tempValue;
}

void longDegreeFromNmeaFloat(const floatDegree nmeaDeg, signed16Degree* ptrDegree, signed32Degree* ptrMinutes)
{
    // Get the degrees by division of 100 and typecasting. DDMM.mmmmmm -> DD.MMmmmm -> DD
    *ptrDegree = (signed16Degree)(nmeaDeg / 100);
    // Get minutes by subtracting (*ptrDegree)*100 then multiplying by 10000. DDMM.mmmm -> MM.mmmm -> MMmmmm
    *ptrMinutes = (signed32Degree)((nmeaDeg - ((*ptrDegree) * 100)) * 10000);
}

void longLatitudeFromNmeaInfo(nmeaINFO* nmeaInfo, signed16Degree* ptrDegree, signed32Degree* ptrMinutes)
{
    longDegreeFromNmeaFloat(nmeaInfo->lat, ptrDegree, ptrMinutes);
}

void longLongitudeFromNmeaInfo(nmeaINFO* nmeaInfo, signed16Degree* ptrDegree, signed32Degree* ptrMinutes)
{
    longDegreeFromNmeaFloat(nmeaInfo->lon, ptrDegree, ptrMinutes);
}

void nmeaInfoToCoord(nmeaINFO* nmeaInfo, Coordinate* coord)
{
    longLatitudeFromNmeaInfo(nmeaInfo, &(coord->dLatitude), &(coord->mLatitude));
    longLongitudeFromNmeaInfo(nmeaInfo, &(coord->dLongitude), &(coord->mLongitude));
}
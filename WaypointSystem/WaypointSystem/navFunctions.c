#include "navFunctions.h"

floatDegree longDegreeToFloat(const signed16Degree *degree, const signed32Degree *minutes)
{
    floatDegree fracDeg = (floatDegree)(*degree);
    floatDegree fracMin = (((floatDegree)(*minutes)) / 600000);

    // The fractional degrees from the minutes should conserve the sign of the degrees. If the degrees have negative sign subtract the minutes, if not add the minutes.
    if (fracDeg < 0)
    {
        fracDeg = fracDeg - fracMin;
    }
    else
    {
        fracDeg = fracDeg + fracMin;
    }

    return fracDeg;
}

floatDegree latitudeFromCoordinate(const struct Coordinate *thisCoord)
{
    return longDegreeToFloat(&(thisCoord->dLatitude), &(thisCoord->mLatitude));
}

floatDegree longitudeFromCoordinate(const struct Coordinate *thisCoord)
{
    return longDegreeToFloat(&(thisCoord->dLongitude), &(thisCoord->mLongitude));
}

floatDegree toDegree(const floatDegree *rAngle)
{
    return ((*rAngle)*180) / pi;
}

floatDegree toRadian(const floatDegree *dAngle)
{
    return ((*dAngle)*pi) / 180;
}
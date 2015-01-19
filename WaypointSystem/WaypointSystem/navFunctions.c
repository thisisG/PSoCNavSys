#ifdef __cplusplus
extern "C"
{
#endif
#include "navfunctions.h"
#ifdef __cplusplus
}
#endif

floatDegree longDegreeToFloat(const signed16Degree degree, const signed32Degree minutes)
{
    floatDegree fracDeg = (floatDegree)(degree);
    floatDegree fracMin = (((floatDegree)(minutes)) / 600000);

    // The fractional degrees from the minutes should conserve the sign of the degrees. 
    // If the degrees have negative sign subtract the minutes, if not add the minutes.
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
    return longDegreeToFloat((thisCoord->dLatitude), (thisCoord->mLatitude));
}

floatDegree longitudeFromCoordinate(const struct Coordinate *thisCoord)
{
    return longDegreeToFloat((thisCoord->dLongitude), (thisCoord->mLongitude));
}

floatDegree toDegree(const floatDegree rAngle)
{
    return (floatDegree)(((rAngle) * 180) / M_PI);
}

floatDegree toRadian(const floatDegree dAngle)
{
    return (floatDegree)(((dAngle)*M_PI) / 180);
}

floatDegree haversine(const floatDegree rAngle)
{
    return (floatDegree)(pow(sin((rAngle) / 2), 2));
}

floatDegree inverseHaversine(const floatDegree rAngle)
{
    return (floatDegree)(2 * atan2(sqrt((rAngle)), (sqrt(1 - (rAngle))) ) );
}

floatDegree distanceCirclePath(const struct Coordinate *coordA, const struct Coordinate *coordB)
{
    // Get latitudeA in radians
    floatDegree tempValue = latitudeFromCoordinate(&(*coordA));
    floatDegree rLatA = toRadian(tempValue);

    // Get latitudeB in radians
    tempValue = longitudeFromCoordinate(&(*coordA));
    floatDegree rLonA = toRadian(tempValue);

    // Get longitudeB in radians
    tempValue = latitudeFromCoordinate(&(*coordB));
    floatDegree rLatB = toRadian(tempValue);

    // Get longitudeA in radians
    tempValue = longitudeFromCoordinate(&(*coordB));
    floatDegree rLonB = toRadian(tempValue);

    // Find tempValue = deltaLat = rLatA - rLatB
    tempValue = rLatB - rLatA;
    // Calculate haversine() of deltaLat and store in rHaversine
    floatDegree rHaversine = haversine(tempValue);

    // Find tempValue = deltaLon = rLonA - rLonB
    tempValue = rLonB - rLonA;
    // Calculate cos(rLatA)*cos(rLatB)*haversine(deltaLong) and add to rHaversine
    rHaversine = rHaversine + (floatDegree)(cos(rLatA)*cos(rLatB))*haversine(tempValue);

    // Compute inverse haversine, multiply by radius of earth and return the greater circle path value
    rHaversine = (floatDegree)earthRadiusM * inverseHaversine(rHaversine);
    return rHaversine;
}

floatDegree distanceSphereCosine(const struct Coordinate *coordA, const struct Coordinate *coordB)
{
    // Get latitudeA in radians
    floatDegree tempValue = latitudeFromCoordinate(&(*coordA));
    floatDegree rLatA = toRadian(tempValue);

    // Get latitudeB in radians
    tempValue = longitudeFromCoordinate(&(*coordA));
    floatDegree rLonA = toRadian(tempValue);

    // Get longitudeB in radians
    tempValue = latitudeFromCoordinate(&(*coordB));
    floatDegree rLatB = toRadian(tempValue);

    // Get longitudeA in radians
    tempValue = longitudeFromCoordinate(&(*coordB));
    floatDegree rLonB = toRadian(tempValue);

    // Find tempValue = deltaLon = rLonA - rLonB
    tempValue = rLonB - rLonA;
    // Calculate part of the inner product of law of spherical cosines and store in tempValue
    tempValue = (floatDegree)(cos(rLatA)*cos(rLatB)*cos(tempValue));

    // Calculate rest of inner product of law of spherical cosines and store in tempValue
    tempValue = tempValue + (floatDegree)(sin(rLatA)*sin(rLatB));

    // Return distance as a function of inverse cos() of tempValue times radius of earth
    return (floatDegree)earthRadiusM * (floatDegree)acos(tempValue);
}

floatDegree distanceEquiRect(const struct Coordinate *coordA, const struct Coordinate *coordB)
{
    // Get latitudeA in radians
    floatDegree tempValue = latitudeFromCoordinate(&(*coordA));
    floatDegree rLatA = toRadian(tempValue);

    // Get latitudeB in radians
    tempValue = longitudeFromCoordinate(&(*coordA));
    floatDegree rLonA = toRadian(tempValue);

    // Get longitudeB in radians
    tempValue = latitudeFromCoordinate(&(*coordB));
    floatDegree rLatB = toRadian(tempValue);

    // Get longitudeA in radians
    tempValue = longitudeFromCoordinate(&(*coordB));
    floatDegree rLonB = toRadian(tempValue);
    
    // Find tempValue = avgLat = (rLatA + rLatB)/2
    tempValue = (rLatA + rLatB) / 2;
    // Calculate x = deltaLon * cos(avgLat) for equirectangular approximation
    tempValue = (rLonA - rLonB) * (floatDegree)cos(tempValue);

    // Calculate y = deltaLat = rLatA - rLatB for equirectangular approximation
    floatDegree rY = (rLatA - rLatB);

    // Return earth radius * sqrt(x^2+y^2)
    return (floatDegree)earthRadiusM * (floatDegree)sqrt(pow(tempValue, 2) + pow(rY, 2));
}
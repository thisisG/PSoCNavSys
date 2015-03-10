#ifdef __cplusplus
extern "C" {
#endif
#include "navmath.h"
#ifdef __cplusplus
}
#endif

floatDegree floatFromLongDegree(const signed16Degree degree, const signed32Degree minutes)
{
    floatDegree fracDeg = (floatDegree)(degree);
    floatDegree fracMin = (((floatDegree)(minutes)) / 600000);
    return fracDeg + fracMin;
}

floatDegree toDegree(const floatDegree rAngle) { return (floatDegree)(((rAngle)*180) / M_PI); }

floatDegree toRadian(const floatDegree dAngle) { return (floatDegree)(((dAngle)*M_PI) / 180); }

floatDegree rHaversine(const floatDegree rAngle)
{
    return (floatDegree)(pow(sin((rAngle) / 2), 2));
}

floatDegree rInverseHaversine(const floatDegree rAngle)
{
    return (floatDegree)(2 * atan2(sqrt((rAngle)), (sqrt(1 - (rAngle)))));
}

floatDegree distanceCirclePath(const floatDegree rLatA, const floatDegree rLonA,
                               const floatDegree rLatB, const floatDegree rLonB)
{
    // Find tempValue = deltaLat = rLatA - rLatB
    floatDegree tempValue = rLatB - rLatA;
    // Calculate rHaversine() of deltaLat and store in rHaversine
    floatDegree rHaver = rHaversine(tempValue);

    // Find tempValue = deltaLon = rLonA - rLonB
    tempValue = rLonB - rLonA;
    // Calculate cos(rLatA)*cos(rLatB)*rHaversine(deltaLong) and add to rHaversine
    rHaver = rHaver + (floatDegree)(cos(rLatA) * cos(rLatB)) * rHaversine(tempValue);

    // Compute inverse rHaversine, multiply by radius of earth and return the greater circle path
    // value
    rHaver = (floatDegree)earthRadiusM * rInverseHaversine(rHaver);
    return rHaver;
}

floatDegree distanceSphereCosine(const floatDegree rLatA, const floatDegree rLonA,
                                 const floatDegree rLatB, const floatDegree rLonB)
{
    // Find tempValue = deltaLon = rLonA - rLonB
    floatDegree tempValue = rLonB - rLonA;
    // Calculate part of the inner product of law of spherical cosines and store in tempValue
    tempValue = (floatDegree)(cos(rLatA) * cos(rLatB) * cos(tempValue));

    // Calculate rest of inner product of law of spherical cosines and store in tempValue
    tempValue = tempValue + (floatDegree)(sin(rLatA) * sin(rLatB));

    // Return distance as a function of inverse cos() of tempValue times radius of earth
    return (floatDegree)earthRadiusM * (floatDegree)acos(tempValue);
}

floatDegree distanceEquiRect(const floatDegree rLatA, const floatDegree rLonA,
                             const floatDegree rLatB, const floatDegree rLonB)
{

    // Find tempValue = avgLat = (rLatA + rLatB)/2
    floatDegree tempValue = (rLatA + rLatB) / 2;

    // Calculate x = deltaLon * cos(avgLat) for equirectangular approximation
    tempValue = (rLonA - rLonB) * (floatDegree)cos(tempValue);

    // Calculate y = deltaLat = rLatA - rLatB for equirectangular approximation
    floatDegree rY = (rLatA - rLatB);

    // Return earth radius * sqrt(x^2+y^2)
    return (floatDegree)earthRadiusM * (floatDegree)sqrt(pow(tempValue, 2) + pow(rY, 2));
}

floatDegree dInitialHeading(const floatDegree dLatA, const floatDegree dLonA,
                            const floatDegree dLatB, const floatDegree dLonB)
{
    floatDegree rLatA = toRadian(dLatA);
    floatDegree rLonA = toRadian(dLonA);
    floatDegree rLatB = toRadian(dLatB);
    floatDegree rLonB = toRadian(dLonB);

    floatDegree rY = sin(rLonB - rLonA) * cos(rLatB);

    floatDegree rX = (cos(rLatA) * sin(rLatB)) - (sin(rLatA) * cos(rLatB) * cos(rLonB - rLonA));

    // Return value is within +-180 degrees, normalise this to 0->360
    floatDegree dHeading = fmodf(toDegree(atan2f(rY, rX) + 180), 360);

    return dHeading;
}

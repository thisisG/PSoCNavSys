#ifdef __cplusplus
extern "C" {
#endif
#include "navfunctions.h"
#ifdef __cplusplus
}
#endif

void zeroCoordinate(Coordinate* coord)
{
    coord->dLatitude = 0;
    coord->dLongitude = 0;
    coord->mLatitude = 0;
    coord->mLongitude = 0;
    coord->priority = 0;
}

void initGpsBuffer(GpsBuffer* gpsB)
{
    gpsB->gpsBufferLength = GPS_STR_BFR_LEN;
    gpsB->gpsStringBuffer[0] = 0;
}

void initSerialBuffer(SerialBuffer* serialB)
{
    serialB->serialBufferLength = SERIAL_STR_BFR_LEN;
    serialB->serialStringBuffer[0] = 0;
}

void zeroSystemTime(SystemTime* time)
{
    time->Sec = 0;
    time->Min = 0;
    time->Hour = 0;
    time->DayOfWeek = 0;
    time->DayOfMonth = 0;
    time->DayOfYear = 0;
    time->Month = 0;
    time->Year = 0;
}

void zeroNavState(NavState* navS)
{
    initGpsBuffer(&(navS->gpsBuffer));
    initSerialBuffer(&(navS->serialBuffer));
    zeroCoordinate(&(navS->currentLocation));
    zeroCoordinate(&(navS->nextWaypoint));
    zeroSystemTime(&(navS->time));
    navS->dCurrentHeading = 0;
    navS->dOverallHeading = 0;
    navS->currentSpeedKmh = 0;
}

void updateNavState(NavState* navS)
{
    // navS->dOverallHeading = dHeadingToCurrentWP(navS);
}

#ifdef _WIN32
void printCoordData(Coordinate* coord)
{
    printf("Coordinate data: \n");
    printf("lat: %dd%dm \nlon: %dd%dm \nprio: %d\n", coord->dLatitude, coord->mLatitude,
           coord->dLongitude, coord->mLongitude, coord->priority);
}

void printCurrentCoordAndHeading(NavState* navS)
{
    printf("Current data: \n");
    printCoordData(&(navS->currentLocation));
    printf("heading: %f\n", navS->dCurrentHeading);
    printf("heading to wp: %f\n", navS->dOverallHeading);
    printf("speed: %f\n", navS->currentSpeedKmh);
}
#endif // _WIN32

floatDegree floatFromLongDegree(const signed16Degree degree, const signed32Degree minutes)
{
    floatDegree fracDeg = (floatDegree)(degree);
    floatDegree fracMin = (((floatDegree)(minutes)) / 600000);
    return fracDeg + fracMin;
}

floatDegree latitudeFromCoordinate(const struct Coordinate* thisCoord)
{
    return floatFromLongDegree((thisCoord->dLatitude), (thisCoord->mLatitude));
}

floatDegree longitudeFromCoordinate(const struct Coordinate* thisCoord)
{
    return floatFromLongDegree((thisCoord->dLongitude), (thisCoord->mLongitude));
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

floatDegree distanceCirclePath(const struct Coordinate* coordA, const struct Coordinate* coordB)
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

floatDegree distanceSphereCosine(const struct Coordinate* coordA, const struct Coordinate* coordB)
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
    tempValue = (floatDegree)(cos(rLatA) * cos(rLatB) * cos(tempValue));

    // Calculate rest of inner product of law of spherical cosines and store in tempValue
    tempValue = tempValue + (floatDegree)(sin(rLatA) * sin(rLatB));

    // Return distance as a function of inverse cos() of tempValue times radius of earth
    return (floatDegree)earthRadiusM * (floatDegree)acos(tempValue);
}

floatDegree distanceEquiRect(const struct Coordinate* coordA, const struct Coordinate* coordB)
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

floatDegree dInitialHeading(const floatDegree dLatA, const floatDegree dLonA,
                            const floatDegree dLatB, const floatDegree dLonB)
{
    floatDegree rLatA = toRadian(dLatA);
    floatDegree rLonA = toRadian(dLonA);
    floatDegree rLatB = toRadian(dLatB);
    floatDegree rLonB = toRadian(dLonB);

    floatDegree rY = sin(rLonB - rLonA) * cos(rLatB);

    floatDegree rX = (cos(rLatA) * sin(rLatB)) - (sin(rLatA) * cos(rLatB) * cos(rLonB - rLonA));

    floatDegree dBearing = fmodf(toDegree(atan2f(rY, rX) + 180), 360);

    return dBearing;
}

floatDegree dHeadingFromAtoB(const Coordinate* coordA, const Coordinate* coordB)
{
    return dInitialHeading(latitudeFromCoordinate(coordA), longitudeFromCoordinate(coordA),
                           latitudeFromCoordinate(coordB), longitudeFromCoordinate(coordB));
}

floatDegree dHeadingToCurrentWP(NavState* navS)
{
    return dHeadingFromAtoB(&(navS->currentLocation), &(navS->nextWaypoint));
}
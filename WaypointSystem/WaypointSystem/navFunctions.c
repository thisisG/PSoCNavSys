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
    int i;
    for(i=0 ; i < (gpsB->gpsBufferLength) ; i++)
    {
        gpsB->gpsStringBuffer[i] = '\0';
    }
}

void initSerialBuffer(SerialBuffer* serialB)
{
    serialB->serialBufferLength = SERIAL_STR_BFR_LEN;
    int i;
    for(i=0 ; i < (serialB->serialBufferLength) ; i++)
    {
        serialB->serialStringBuffer[i] = '\0';
    }
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
    printf("lat: %dd%dm \nlon: %dd%dm \nprio: %d\n", coord->dLatitude,
           coord->mLatitude, coord->dLongitude, coord->mLongitude,
           coord->priority);
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

floatDegree latitudeFromCoordinate(const struct Coordinate* thisCoord)
{
    return floatFromLongDegree((thisCoord->dLatitude), (thisCoord->mLatitude));
}

floatDegree longitudeFromCoordinate(const Coordinate* thisCoord)
{
    return floatFromLongDegree((thisCoord->dLongitude),
                               (thisCoord->mLongitude));
}

floatDegree distanceCirclePathAtoB(const struct Coordinate* coordA,
                                   const struct Coordinate* coordB)
{
    floatDegree rLatA = toRadian(latitudeFromCoordinate(coordA));
    floatDegree rLonA = toRadian(longitudeFromCoordinate(coordA));
    floatDegree rLatB = toRadian(latitudeFromCoordinate(coordB));
    floatDegree rLonB = toRadian(longitudeFromCoordinate(coordB));

    return distanceCirclePath(rLatA, rLonA, rLatB, rLonB);
}

floatDegree distanceSphereCosineAtoB(const struct Coordinate* coordA,
                                     const struct Coordinate* coordB)
{
    floatDegree rLatA = toRadian(latitudeFromCoordinate(coordA));
    floatDegree rLonA = toRadian(longitudeFromCoordinate(coordA));
    floatDegree rLatB = toRadian(latitudeFromCoordinate(coordB));
    floatDegree rLonB = toRadian(longitudeFromCoordinate(coordB));

    return distanceSphereCosine(rLatA, rLonA, rLatB, rLonB);
}

floatDegree distanceEquiRectAtoB(const struct Coordinate* coordA,
                                 const struct Coordinate* coordB)
{
    floatDegree rLatA = toRadian(latitudeFromCoordinate(coordA));
    floatDegree rLonA = toRadian(longitudeFromCoordinate(coordA));
    floatDegree rLatB = toRadian(latitudeFromCoordinate(coordB));
    floatDegree rLonB = toRadian(longitudeFromCoordinate(coordB));

    return distanceEquiRect(rLatA, rLonA, rLatB, rLonB);
}

floatDegree dHeadingFromAtoB(const Coordinate* coordA, const Coordinate* coordB)
{
    return dInitialHeading(
        latitudeFromCoordinate(coordA), longitudeFromCoordinate(coordA),
        latitudeFromCoordinate(coordB), longitudeFromCoordinate(coordB));
}

floatDegree dHeadingToCurrentWP(NavState* navS)
{
    return dHeadingFromAtoB(&(navS->currentLocation), &(navS->nextWaypoint));
}
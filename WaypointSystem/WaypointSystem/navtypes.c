#ifdef __cplusplus
extern "C" {
#endif
#include "navtypes.h"
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
    printf("speed: %f\n", navS->currentSpeedKmh);
}
#endif // _WIN32
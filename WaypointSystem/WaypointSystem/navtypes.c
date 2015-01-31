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

void zeroGpsBuffer(GpsBuffer* gpsB)
{
    gpsB->gpsBufferLength = GPS_STR_BFR_LEN;
    gpsB->gpsStringBuffer[0] = 0;
}

void zeroNavState(NavState* navS)
{
    zeroGpsBuffer(&(navS->gpsBuffer));
    zeroCoordinate(&(navS->currentLocation));
    zeroCoordinate(&(navS->nextWaypoint));
    navS->dCurrentHeading = 0;
    navS->dOverallHeading = 0;
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
}
#endif
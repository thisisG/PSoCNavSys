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
    gpsB->newGPSString = 0;
    gpsB->gpsBufferLength = GPS_STR_BFR_LEN;
    int i;
    for (i = 0; i < (gpsB->gpsBufferLength); i++)
    {
        gpsB->gpsStringBuffer[i] = '\0';
    }
}

void initSerialBuffer(SerialBuffer* serialB)
{
    serialB->serialBufferLength = SERIAL_STR_BFR_LEN;
    int i;
    for (i = 0; i < (serialB->serialBufferLength); i++)
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

void initStateDataStructure(StateDataStructure* stateD)
{
    stateD->stateKeeper = closestWP;
}

void zeroNavState(NavState* navS)
{
    // Want the default state of the system to be well defined at a point that
    // can handle all modes of normal operation and exceptions after the system
    // starts up.
    initStateDataStructure(&(navS->stateData));
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
    // First we want to handle new data that has arrived.
    if (navS->gpsBuffer.newGPSString == 1)
    {
        decodeGpsStringInNavState(navS);
        navS->gpsBuffer.newGPSString = 0;
    }

    // Then we want to evaluate our current state based on the stateKeeper in
    // the passed NavState structure.
    // Each valid state is handled by a discrete handler except for illegal
    // state values, in which case the handler for the normally used initial
    // state closestWP is called.
    // Each handler will return the new state decided upon inside the handler,
    // which is used to update the stateKeeper in navState.

    CurrentNavState newState = closestWP;

    switch (navS->stateData.stateKeeper)
    {
    case closestWP:
        newState = closestWPHandler(navS);
        break;
    case toWP:
        newState = toWPHandler(navS);
        break;
    case atWP:
        newState = atWPHandler(navS);
        break;
    case nextWP:
        newState = nextWPHandler(navS);
        break;
    case atGoal:
        newState = atGoalHandler(navS);
        break;
    case closestExceptionWP:
        newState = closestExceptionWPHandler(navS);
        break;
    case toExceptionWP:
        newState = toExceptionWPHandler(navS);
        break;
    case atExceptionWP:
        newState = atExceptionWPHandler(navS);
        break;
    case nextExceptionWP:
        newState = nextExceptionWPHandler(navS);
        break;
    case atExceptionGoal:
        newState = atExceptionGoalHandler(navS);
        break;
    case firstCurrentNavState:
    case lastCurrentNavState:
    default:
        // The way to handle these as they should never ocurr is to set the
        // stateKeeper to closestWP as this should be the default state in case
        // of unforseen actions.
        newState = closestWPHandler(navS);
    }
    navS->stateData.stateKeeper = newState;
}

// TODO
CurrentNavState closestWPHandler(NavState* navS) {}

// TODO
CurrentNavState toWPHandler(NavState* navS) {}

// TODO
CurrentNavState atWPHandler(NavState* navS) {}

// TODO
CurrentNavState nextWPHandler(NavState* navS) {}

// TODO
CurrentNavState atGoalHandler(NavState* navS) {}

// TODO
CurrentNavState closestExceptionWPHandler(NavState* navS) {}

// TODO
CurrentNavState toExceptionWPHandler(NavState* navS) {}

// TODO
CurrentNavState atExceptionWPHandler(NavState* navS) {}

// TODO
CurrentNavState nextExceptionWPHandler(NavState* navS) {}

// TODO
CurrentNavState atExceptionGoalHandler(NavState* navS) {}

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
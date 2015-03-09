#ifdef __cplusplus
extern "C" {
#endif
#include "./navfunctions.h"
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
    zeroCoordinate(&(stateD->WPGoal));
    zeroCoordinate(&(stateD->eWPGoal));
    stateD->maxWPDistance = MAX_WP_DISTANCE;
    stateD->arrivalWPDistance = ARRIVED_WP_DISTANCE;
    stateD->exceptionMaxWPDistance = EXCEPTION_MAX_WP_DISTANCE;
    stateD->exceptionWPArrivalDistance = EXCEPTION_ARRIVED_WP_DISTANCE;
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

    // Then we want to calculate the distance from the current position to the
    // current WP and update this in navS.
    navS->distanceToCurrentWP = distanceCirclePathAtoB(&(navS->currentLocation),
                                                       &(navS->nextWaypoint));

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

CurrentNavState toWPHandler(NavState* navS)
{
    CurrentNavState returnState = navS->stateData.stateKeeper;

    // Check if we have drifted too far off course
    if ((navS->distanceToCurrentWP) > (navS->stateData.maxWPDistance))
    {
        returnState = closestWP;
    }
    // Check if we have arrived at the destination
    else if ((navS->distanceToCurrentWP) < (navS->stateData.arrivalWPDistance))
    {
        returnState = atWP;
    }
    // If neither is fulfilled we want to remain in the same state
    else
    {
        // Do nothing, returnState is already the original state.
    }

    return returnState;
}

CurrentNavState atWPHandler(NavState* navS)
{
    // Here we always want to have a changed return state. Set the default
    // return state to closestWP to prevent getting stuck in this state in case
    // of undefined behaviour.
    CurrentNavState returnState = closestWP;

    if (coordsEqual(&(navS->currentLocation), &(navS->stateData.WPGoal)) == 1)
    {
        returnState = atGoal;
    }
    else
    {
        returnState = nextWP;
    }

    return returnState;
}

// TODO
CurrentNavState nextWPHandler(NavState* navS) {}

// TODO
CurrentNavState atGoalHandler(NavState* navS) {}

// TODO
CurrentNavState closestExceptionWPHandler(NavState* navS) {}

CurrentNavState toExceptionWPHandler(NavState* navS)
{
    CurrentNavState returnState = navS->stateData.stateKeeper;

    // Check if we are off course, if we are go to the default initial state
    // since at this point we are lost while being lost.
    if ((navS->distanceToCurrentWP) > (navS->stateData.exceptionMaxWPDistance))
    {
        returnState = closestWP;
    }
    else if ((navS->distanceToCurrentWP)
             < (navS - stateData.exceptionWPArrivalDistance))
    {
        returnState = atExceptionWP;
    }
    else
    {
        // Do nothing, returnState is already the original state.
    }

    return returnState;
}

CurrentNavState atExceptionWPHandler(NavState* navS)
{
    // Here we always want to have a changed return state. Set the default
    // return state to closestWP to prevent getting stuck in this state in case
    // of undefined behaviour.
    CurrentNavState returnState = closestWP;

    if (coordsEqual(&(navS->currentLocation), &(navS->stateData.eWPGoal)) == 1)
    {
        returnState = atExceptionGoal;
    }
    else
    {
        returnState = nextExceptionWP;
    }

    return returnState;
}

// TODO
CurrentNavState nextExceptionWPHandler(NavState* navS) {}

CurrentNavState atExceptionGoalHandler(NavState* navS)
{
    // Here we blindly return the closestWP state since we can assume that we
    // are back on track and ready to follow the main set of waypoints.
    CurrentNavState returnState = closestWP;
    return returnState;
}

uint8 coordsEqual(const struct Coordinate* coordA,
                  const struct Coordinate* coordB)
{
    // Set the default return value to 1 which means that the two coordinates
    // are equal.
    uint8 returnValue = 1;

    // Evaluate each parameter. The use of else if() allows premature
    // termination in case an inequality is found.
    if ((coordA->dLongitude) != (coordB->dLongitude))
    {
        returnValue = 0;
    }
    else if ((coordA->mLongitude) != (coordB->mLongitude))
    {
        returnValue = 0;
    }
    else if ((coordA->dLatitude) != (coordB->dLatitude))
    {
        returnValue = 0;
    }
    else if ((coordA->mLatitude) != (coordB->mLatitude))
    {
        returnValue = 0;
    }
    else if ((coordA->priority) != (coordB->priority))
    {
        returnValue = 0;
    }
    else
    {
        // Do nothing, case here for completion.
    }

    return returnValue;
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

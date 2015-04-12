/*
**
** NavSys library
** URL: https://github.com/thisisG
** Author: Geir Istad (geir dot istad at gmail dot com)
** Licence: MIT
**
*/

#ifdef __cplusplus
extern "C" {
#endif
#include "navfunctions.h"
#include "wphandler.h"
#ifdef __cplusplus
}
#endif

void initCoordinate(Coordinate* coord)
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

void initSystemTime(SystemTime* time)
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
  initCoordinate(&(stateD->WPGoal));
  initCoordinate(&(stateD->eWPGoal));
  initNavWPHandler(&(stateD->WPHandler));
  stateD->maxWPDistance = MAX_WP_DISTANCE;
  stateD->arrivalWPDistance = ARRIVED_WP_DISTANCE;
  stateD->exceptionMaxWPDistance = EXCEPTION_MAX_WP_DISTANCE;
  stateD->exceptionWPArrivalDistance = EXCEPTION_ARRIVED_WP_DISTANCE;
}

void initNavState(NavState* navS)
{
  // Want the default state of the system to be well defined at a point that
  // can handle all modes of normal operation and exceptions after the system
  // starts up.
  initStateDataStructure(&(navS->stateData));
  initGpsBuffer(&(navS->gpsBuffer));
  initSerialBuffer(&(navS->serialBuffer));
  initCoordinate(&(navS->currentLocation));
  initCoordinate(&(navS->nextWaypoint));
  initSystemTime(&(navS->time));
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
  navS->distanceToCurrentWP
    = distanceCirclePathAtoB(&(navS->currentLocation), &(navS->nextWaypoint));

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
    printf("got closestWP!\r\n\r\n");
    newState = closestWPHandler(navS);
    break;
  case toWP:
    printf("got toWP!\r\n\r\n");
    newState = toWPHandler(navS);
    break;
  case atWP:
    printf("got atWP!\r\n\r\n");
    newState = atWPHandler(navS);
    break;
  case nextWP:
    printf("got nextWP!\r\n\r\n");
    newState = nextWPHandler(navS);
    break;
  case atGoal:
    printf("got atGoal!\r\n\r\n");
    newState = atGoalHandler(navS);
    break;
  case closestExceptionWP:
    printf("got closestExceptionWP!\r\n\r\n");
    newState = closestExceptionWPHandler(navS);
    break;
  case toExceptionWP:
    printf("got toExceptionWP!\r\n\r\n");
    newState = toExceptionWPHandler(navS);
    break;
  case atExceptionWP:
    printf("got atExceptionWP!\r\n\r\n");
    newState = atExceptionWPHandler(navS);
    break;
  case nextExceptionWP:
    printf("got nextExceptionWP!\r\n\r\n");
    newState = nextExceptionWPHandler(navS);
    break;
  case atExceptionGoal:
    printf("got atExceptionGoal!\r\n\r\n");
    newState = atExceptionGoalHandler(navS);
    break;
  case firstCurrentNavState:
  case lastCurrentNavState:
  default:
    // The way to handle these as they should never occur is to set the
    // stateKeeper to closestWP as this should be the default state in case
    // of unforseen actions.
    newState = closestWPHandler(navS);
  }
  navS->stateData.stateKeeper = newState;
}

CurrentNavState closestWPHandler(NavState* navS)
{
  CurrentNavState returnState = toWP;

  // Check if cfg file is currently open, if it is close it
  checkAndCloseNavFile(navS->stateData.WPHandler.fileManager.ptrCfgFile);
  
  // Open config file
  navS->stateData.WPHandler.fileManager.ptrCfgFile
    = NAV_fopen(navS->stateData.WPHandler.fileManager.cfgFileName, "rb");

  // Read the file header and config file header.
  NavFileHeader fileHeader;
  initNavFileHeader(&fileHeader);
  NavConfigFileHeader cfgHeader;
  initNavConfigFileHeader(&cfgHeader);
  cfgGetFileHeaderCfgHeader(navS->stateData.WPHandler.fileManager.ptrCfgFile,
    &fileHeader, &cfgHeader);
  
  // Get the name of the current WP list
  getWPListName(navS->stateData.WPHandler.fileManager.ptrCfgFile,
    cfgHeader.currentWPList,
    navS->stateData.WPHandler.fileManager.wpListFileName);
  
  // We are now done with the cfg file, so we should close it.
  NAV_fclose(navS->stateData.WPHandler.fileManager.ptrCfgFile);
  navS->stateData.WPHandler.fileManager.ptrCfgFile = NULL;

  // Open the WP list file
  WPHandlerOpen(&(navS->stateData.WPHandler),
    navS->stateData.WPHandler.fileManager.wpListFileName);
  
  // Get the distance to the goal that was updated by WPHandlerOpen() for a
  // baseline distance that we compare the rest of the list to
  float minDist = distanceCirclePathAtoB(&(navS->currentLocation),
    &(navS->stateData.WPHandler.wpGoal));
  
  // Cycle through the waypoints using WPHandlerGetNextWP and find the
  // closest wp and store the number of the WP
  int32_t WPCount = 0;
  int32_t nextWPNumber = 0;
  Coordinate selectedCoord;
  initCoordinate(&selectedCoord);
  // WPHandlerNextWP returns -1 when there are no more waypoints in the list
  while (WPCount != -1)
  {
    WPCount = WPHandlerNextWP(&(navS->stateData.WPHandler),
      &(navS->stateData.nextWP));
    if (WPCount != -1)
    {
      float tempDist = distanceCirclePathAtoB(&(navS->currentLocation),
        &(navS->stateData.nextWP));
      if (tempDist < minDist)
      {
        nextWPNumber = WPCount;
        minDist = tempDist;
        selectedCoord = navS->stateData.nextWP;
      }
    }
  }
  
  // If the closest WP is within allowable distances set it as current WP, seek
  // WP set returnstate to toWP
  if (minDist < navS->stateData.maxWPDistance)
  {
    WPHandlerSeekWP(&(navS->stateData.WPHandler), nextWPNumber);
    navS->nextWaypoint = selectedCoord;
    navS->distanceToCurrentWP = minDist;
    returnState = toWP;
  }
  // If no WPs are within permitted distances set returnstate to
  // closestExceptionWP
  else
  {
    returnState = closestExceptionWP;
  }
  
  return returnState;
}

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

CurrentNavState nextWPHandler(NavState* navS)
{
  CurrentNavState returnState = toWP;

  // Update the WP and the distance
  int32_t wpCount
    = WPHandlerNextWP(&(navS->stateData.WPHandler), &(navS->nextWaypoint));

  // Check that the WP number is indeed valid
  if (wpCount != -1)
  {
    navS->distanceToCurrentWP
      = distanceCirclePathAtoB(&(navS->currentLocation), &(navS->nextWaypoint));
  }
  // If we get -1 the last wp in the list is different from the WPgoal, this
  // should never happen.
  else
  {
    // TODO ERROR REPORTING
    returnState = closestWP;
  }

  return returnState;
}

CurrentNavState atGoalHandler(NavState* navS)
{
  // We want to change the current WP list being used by the system. Hence the
  // return value should reflect that we are going to find the closest waypoint
  // in this list.
  CurrentNavState returnState = closestWP;

  // Load the config file
  checkAndCloseNavFile(navS->stateData.WPHandler.fileManager.ptrCfgFile);
  NAV_fopen(navS->stateData.WPHandler.fileManager.cfgFileName,
    "r+b");
  // Read file header and config header
  NavFileHeader fileHeader;
  initNavFileHeader(&fileHeader);
  NavConfigFileHeader cfgHeader;
  initNavConfigFileHeader(&cfgHeader);
  cfgGetFileHeaderCfgHeader(navS->stateData.WPHandler.fileManager.ptrCfgFile,
    &fileHeader, &cfgHeader);

  // Check if there are more WPlists available in the file. 
  // If there is add one to the count.
  if (cfgHeader.currentWPList < cfgHeader.numberOfWPLists)
  {
    cfgHeader.currentWPList += 1;
  }
  // If not wrap around to the first WP list
  else
  {
    cfgHeader.currentWPList = 1;
  }

  // Seek back the size of the cfg header, write it to file and close the file.
  NAV_fseek(navS->stateData.WPHandler.fileManager.ptrCfgFile,
    -SIZE_NAV_CFG_FILE_HEADER, NAV_SEEK_CUR);
  fwriteNavConfigFileHeader(&cfgHeader,
    navS->stateData.WPHandler.fileManager.ptrCfgFile);
  NAV_fclose(navS->stateData.WPHandler.fileManager.ptrCfgFile);
  navS->stateData.WPHandler.fileManager.ptrCfgFile = NULL;

  return returnState;
}

CurrentNavState closestExceptionWPHandler(NavState* navS)
{
  // Open cfg file and get the maximum number of exception waypoints.
  checkAndCloseNavFile(navS->stateData.WPHandler.fileManager.ptrCfgFile);
  navS->stateData.WPHandler.fileManager.ptrCfgFile
      = NAV_fopen(navS->stateData.WPHandler.fileManager.cfgFileName, "r+b");

  if (navS->stateData.WPHandler.fileManager.ptrCfgFile == 0)
  {
    printf("null!\r\n");
    getchar();
  }

  // Read file header and config header
  NavFileHeader fileHeader;
  initNavFileHeader(&fileHeader);
  NavConfigFileHeader cfgHeader;
  initNavConfigFileHeader(&cfgHeader);
  cfgGetFileHeaderCfgHeader(navS->stateData.WPHandler.fileManager.ptrCfgFile,
    &fileHeader, &cfgHeader);

  checkAndCloseNavFile(navS->stateData.WPHandler.fileManager.ptrCfgFile);

  uint32_t currExList = 1;
  uint32_t maxExList = cfgHeader.numberOfExeptionWPLists;

  // Want the minDist to be a distance not possible to achieve on the face of
  // the planet to ensure that the calculated distances are all less than this.
  float minDist = (2 * M_PI * earthRadiusM) + earthRadiusM;
  float tempDist = 0;
  uint32_t selectedExWPList = -1;
  uint32_t selectedExWP = -1;
  uint32_t WPCount = 0;
  Coordinate tempCoord;
  Coordinate selectedCoord;
  initCoordinate(&tempCoord);
  initCoordinate(&selectedCoord);
  // Check each file
  for (currExList = 1; currExList <= maxExList; currExList++)
  {
    // Get file name
    navS->stateData.WPHandler.fileManager.ptrCfgFile
      = NAV_fopen(navS->stateData.WPHandler.fileManager.cfgFileName, "r+b");
    getExceptionWPListName(
      navS->stateData.WPHandler.fileManager.ptrCfgFile, currExList,
      navS->stateData.WPHandler.fileManager.eWPListFileName);
    checkAndCloseNavFile(navS->stateData.WPHandler.fileManager.ptrCfgFile);
    // Open file
    WPHandlerOpen(&(navS->stateData.WPHandler),
      navS->stateData.WPHandler.fileManager.eWPListFileName);
    // Check each waypoint in each file
    WPCount = 0;
    while (WPCount != -1)
    {
      WPCount = WPHandlerNextWP(&(navS->stateData.WPHandler), &tempCoord);
      if (WPCount != -1)
      {
        tempDist
          = distanceCirclePathAtoB(&(navS->currentLocation), &(tempCoord));
        // If the distance is less than the previous stored distance
        if (tempDist < minDist)
        {
          // Store the exception list numbers and the waypoint
          minDist = tempDist;
          selectedCoord = tempCoord;
          selectedExWPList = currExList;
          selectedExWP = WPCount;
        }
      }
    }
  }

  // Open the list with the closest waypoint
  navS->stateData.WPHandler.fileManager.ptrCfgFile
    = NAV_fopen(navS->stateData.WPHandler.fileManager.cfgFileName, "r+b");
  getExceptionWPListName(navS->stateData.WPHandler.fileManager.ptrCfgFile,
                         selectedExWPList,
                         navS->stateData.WPHandler.fileManager.eWPListFileName);

  // Close the cfg file as we are now done with it
  checkAndCloseNavFile(navS->stateData.WPHandler.fileManager.ptrCfgFile);

  WPHandlerOpen(&(navS->stateData.WPHandler),
                navS->stateData.WPHandler.fileManager.eWPListFileName);
  // Seek to the selected WP
  WPHandlerSeekWP(&(navS->stateData.WPHandler), selectedExWP);
  // Set the selected waypoint as the current waypoint
  navS->nextWaypoint = selectedCoord;
  // Update the EWP goal
  WPHandlerGetGoal(&(navS->stateData.WPHandler), &(navS->stateData.eWPGoal));
  // Update the distance to the waypoint
  navS->distanceToCurrentWP = minDist;
  // Update the minimum distance for detecting arrival
  navS->stateData.exceptionMaxWPDistance = 2 * minDist;

  return toExceptionWP;
}

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
    < (navS->stateData.exceptionWPArrivalDistance))
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

  // Check if we are at goal
  if (coordsEqual(&(navS->nextWaypoint), &(navS->stateData.eWPGoal)) == 1)
  {
    returnState = atExceptionGoal;
  }
  else
  {
    returnState = nextExceptionWP;
  }
  return returnState;
}

CurrentNavState nextExceptionWPHandler(NavState* navS)
{
  CurrentNavState returnState = toExceptionWP;
  uint32_t WPCount
    = WPHandlerNextWP(&(navS->stateData.WPHandler), &(navS->nextWaypoint));
  // If we for some reason get an errenous WP as the next (return of -1) set the
  // returnstate to closestWP
  if (WPCount == -1)
  {
    // TODO LOG ERROR
    returnState = closestWP;
  }
  return returnState;
}

CurrentNavState atExceptionGoalHandler(NavState* navS)
{
  // Here we blindly return the closestWP state since we can assume that we
  // are back on track and ready to follow the main set of waypoints.
  CurrentNavState returnState = closestWP;
  return returnState;
}

uint8_t coordsEqual(const struct Coordinate* coordA,
  const struct Coordinate* coordB)
{
  // Set the default return value to 1 which means that the two coordinates
  // are equal.
  uint8_t returnValue = 1;

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
void printCoordData(const Coordinate* coord)
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
  return floatFromLongDegree((thisCoord->dLongitude), (thisCoord->mLongitude));
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
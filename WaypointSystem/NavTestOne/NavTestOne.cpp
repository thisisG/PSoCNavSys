/*
**
** NavSys library
** URL: https://github.com/thisisG
** Author: Geir Istad (geir dot istad at gmail dot com)
** Licence: MIT
**
*/


#include <iostream>
#include <fstream>
#include "navmanager\gpsemulator.h"

#ifdef __cplusplus
extern "C" {
#endif
#include <stdio.h>
#include "navsys\navtypes.h"
#include "navsys\navfunctions.h"
#include "navsys\serialcom.h"
#include "navsys\gpsinterface.h"
#ifdef __cplusplus
}
#endif

int main()
{
  NavState myNavState;
  initNavState(&myNavState);
  updateNavState(&myNavState);
  updateNavState(&myNavState);

  printf("%f\r\n", myNavState.distanceToCurrentWP);
  printf("%s\r\n", myNavState.stateData.WPHandler.fileManager.wpListFileName);
  printCoordData(&myNavState.currentLocation);
  printCoordData(&myNavState.nextWaypoint);

  return 0;
}

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
#include <sstream>
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
char justAChar;
int main()
{
  NavState myNavState;
  initNavState(&myNavState);

  GpsEmulator gpsEmu;
  gpsEmu.parseCsvToInfoVector("./../../data/NavTestOne/SimPath.csv");
  gpsEmu.generatePseudoRandomData();
  std::string dataString;
  std::ostringstream stringBuffer;
  /*
  while (gpsEmu.getNextStringToNavState(&myNavState) != -1)
  {
    updateNavState(&myNavState);
    printf("dist to WP: %f\r\n", myNavState.distanceToCurrentWP);
    printf("Current loc:\r\n");
    printCoordData(&myNavState.currentLocation);
    printf("Next WP:\r\n");
    printCoordData(&myNavState.nextWaypoint);
    printf("Heading to WP: %f\r\n", dHeadingToCurrentWP(&myNavState));
    stringBuffer << latitudeFromCoordinate(&myNavState.currentLocation) << ","
                 << longitudeFromCoordinate(&myNavState.currentLocation) << ","
                 << latitudeFromCoordinate(&myNavState.nextWaypoint) << ","
                 << longitudeFromCoordinate(&myNavState.nextWaypoint) << ","
                 << dHeadingToCurrentWP(&myNavState) << std::endl;
    //getchar();
  }
  std::ofstream output;
  output.open("SimOutput.csv");
  output << stringBuffer.str();
  */

  return 0;
}

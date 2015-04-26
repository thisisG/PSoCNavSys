/*
**
** NavSys library
** URL: https://github.com/thisisG
** Author: Geir Istad (geir dot istad at gmail dot com)
** Licence: MIT
**
*/

/*
Format for longitude/latitude is as follows:

N/S +/-
E/W +/-

Angles are denoted by a prefix r for the angle in radians and d for angle in
degrees.
Note that for mathematical operations the angle should be in radians while for
coordinate storage the angle is in degrees.

Longitude and latitude is stored as a pair of integers. a signed 16 bit integer
is used to store the degrees while a signed 32 bit integer is used to store the
minutes as MM.mmmm with 6 significant digits. The minutes are stored as MMmmmm
and by dividing by 10^4 we get the minutes with fractions. This is then
converted to fractional degrees by dividing by 60.
*/

// FUNCTION DESCRIPTION
/* FUNCTIONNAME
input:

output:

remarks:

*/
#ifndef NAVFUNCTIONS_H
#define NAVFUNCTIONS_H

#define NAVFUNCDEBUG

// Local includes
#ifdef __cplusplus
extern "C" {
#endif // __cplusplus
#include <stdint.h>

#ifdef _WIN32
#include <stdio.h>
#endif // _WIN32

#include "gpsinterface.h"
#include "navtypes.h"
#include "navmath.h"
#include "navconfig.h"
#ifdef __cplusplus
}
#endif

void initCoordinate(Coordinate* coord);
void initGpsBuffer(GpsBuffer* gpsB);
void initSerialBuffer(SerialBuffer* serialB);
void initSystemTime(SystemTime* time);
void initStateDataStructure(StateDataStructure* stateD);
void initNavState(NavState* navS);

/* updateNavState();
input:
(reference) NavState navS

output:
NA

remarks:
The main controlled for the Finite State Machine in charge of the system
behavior depending on the current conditions of the system.
The function will read and update the current position using GPS data when a new
GPS string is available in the buffer.

It will control the decision making regarding which waypoint to follow, decide
upon when a waypoint has been arrived at and if and when there is a need to
handle exceptions in the navigation system - mainly if the system have drifted
out of position and need to follow an alternative set of waypoints in order to
reach its goal.
*/
void updateNavState(NavState* navS);

/*
input:
  NavState* of which state is assessed.
output:
  CurrentNavState with new system state.
  toWP:
  navS->nextWaypoint = selectedCoord;
  navS->distanceToCurrentWP = minDist;
  closestExceptionWP:
  NA
remarks:
  Internal parameters of NavState is changed by the state handler.
  Function to handle the closestWP state. Finds closest WP in current list (in
  config file) and cycles through all WPs in order to find the closest.
  If no valid WP in range return state is closestExceptionWP.
*/
CurrentNavState closestWPHandler(NavState* navS);

 /*
input:
  NavState* of which state is assessed.
output:
  CurrentNavState with new system state.
  closestWP if lost
  atWP if arrived
remarks:
  No internal parameters of NavState is changed by this function.
  Checks if we have arrived at WP or are too far away from selected WP to
  proceed. If neither state is unchanged.
*/
CurrentNavState toWPHandler(NavState* navS);

 /*
input:
  NavState* of which state is assessed.
output:
  CurrentNavState with new system state.
  atGoal if at goal
  nextWP if not at goal
remarks:
  No internal parameters of NavState is changed by this function.
  Checks if the arrived at WP is the goal or not.
*/
CurrentNavState atWPHandler(NavState* navS);

 /*
input:
  NavState* of which state is assessed.
output:
  CurrentNavState with new system state.
  toWP:
  navS->stateData.WPHandler is changed
  navS->nextWaypoint is changed
  navS->distanceToCurrentWP is updated with new distance
  closestWP:
  NA
remarks:
  Internal parameters of NavState is changed by the state handler.
  Gets the next WP in list using WPHandlers. Returns toWP if next WP is valid.
  If WP is not valid (should never happen) it returns closestWP. In this case
  something is really wrong as there are no more WPs in list, and the current
  WP should've been detected as a goal.
*/
CurrentNavState nextWPHandler(NavState* navS);

 /*
input:
  NavState* of which state is assessed.
output:
  CurrentNavState with new system state.
  closestWP:
  Updates the config file with the new selection of WP list for the system.
  navS->stateData.WPHandler.fileManager.ptrCfgFile opened and closed
remarks:
  Internal parameters of NavState is changed by the state handler.
*/
CurrentNavState atGoalHandler(NavState* navS);

/*
input:
  NavState* of which state is assessed.
output:
  CurrentNavState with new system state.
  toExceptionWP:
  navS->stateData.WPHandler to selected EWP list
  navS->stateData.eWPGoal to goal of selected list
  navS->nextWaypoint = selectedCoord;
  navS->stateData.exceptionWPArrivalDistance = EXCEPTION_ARRIVED_WP_DISTANCE;
  navS->stateData.exceptionMaxWPDistance = minDist + EXCEPTION_MAX_WP_DISTANCE;
remarks:
  Internal parameters of NavState is changed by the state handler.
  Function to handle the closestExceptionWP state. Finds closest EWP in all
  exception lists stored on the system. Will find the closest and modify the
  exception arrival and max distance for exception mode.
*/
CurrentNavState closestExceptionWPHandler(NavState* navS);

 /*
input:
  NavState* of which state is assessed.
output:
  CurrentNavState with new system state.
  closestWP if lost
  atEWP if arrived
remarks:
  No internal parameters of NavState is changed by this function.
  Checks if we have arrived at EWP or are too far away from selected EWP to
  proceed. If neither state is unchanged.
*/
CurrentNavState toExceptionWPHandler(NavState* navS);

 /*
input:
  NavState* of which state is assessed.
output:
  CurrentNavState with new system state.
  atEGoal if at goal
  nextEWP if not at goal
remarks:
  No internal parameters of NavState is changed by this function.
  Checks if the arrived at EWP is the goal or not.
*/
CurrentNavState atExceptionWPHandler(NavState* navS);

 /*
input:
  NavState* of which state is assessed.
output:
  CurrentNavState with new system state.
  toWP:
  navS->stateData.WPHandler is changed
  navS->nextWaypoint is changed
  closestWP:
  NA
remarks:
  Internal parameters of NavState is changed by the state handler.
  Gets the next EWP in list using WPHandlers. Returns toEWP if next WP is valid.
  If WP is not valid (should never happen) it returns closestWP. In this case
  something is really wrong as there are no more EWPs in list, and the current
  EWP should've been detected as a Egoal.
*/
CurrentNavState nextExceptionWPHandler(NavState* navS);

 /*
input:
  NavState* of which state is assessed.
output:
  CurrentNavState with new system state.
  closestWP:
  NA
remarks:
  Internal parameters of NavState is not changed by the handler
*/
CurrentNavState atExceptionGoalHandler(NavState* navS);

 /*
input:
  const struct Coordinate* A
  const struct Coordinate* B
output:
  uint8_t, 1 if equal, 0 if different.
remarks:
  Used to check of two Coordinate structures contain the same information.
*/
uint8_t coordsEqual(const struct Coordinate* coordA,
                    const struct Coordinate* coordB);

 /*
input:
  NavState* containing the to-be-logged GPS string/sentence
output:
  NA
remarks:
  Writes to default gps log file defined in NAV_LOG_GPS_STRINGS in navconfig.h
  Modifies the string stored in gps buffer in NavState to make it conform to the
  format needed for text based logging.
*/
uint8_t logGpsString(NavState* navS);

 /*
input:
  NavState* containing the to-be-logged navigation information
output:
  NA
remarks:
  Writes the current navigation data to the NAV_LOG_FILE_NAME file defined in
  navconfig.h.
  Logged data is:
  day,month,year,hour,min,sec,curLat,curLon,wpLat,wpLon,wpHeading
*/
void logNavData(NavState* navS);

#ifdef _WIN32
// These are used for debugging purpouses when testing code in windows.
void printCoordData(const Coordinate* coord);
void printCurrentCoordAndHeading(NavState* navS);
#endif // _WIN32

/*
input:
  (reference) Coordinate thisCoord
output:
  (value) floatDegree latitude
remarks:
  Extracts the latitude from a Coordinate ADS and returns a fractional
  floating value.
  Note that the value stored in the ADS should be in degrees and NOT radians.
  Care should be taken to ensure that this is upheld throughout the program.
*/
floatDegree latitudeFromCoordinate(const Coordinate* thisCoord); // OK

/*
input:
  (reference) Coordinate thisCoord
output:
  (value) floatDegree latitude
remarks:
  Extracts the longitude from a Coordinate ADS and returns a fractional
  floating value.
  Note that the value stored in the ADS should be in degrees and NOT radians.
  Care should be taken to ensure that this is upheld throughout the program.
*/
floatDegree longitudeFromCoordinate(const Coordinate* thisCoord); // OK

/*
input:
  (reference) Coordinate coordA
  (reference) Coordinate coordB
output:
  (value) floatDegree distanceInM
remarks:
  Returns the great circle distance between two coordinate points assuming an
  average earth radius
*/
floatDegree distanceCirclePathAtoB(
    const struct Coordinate* coordA,
    const struct Coordinate* coordB); // TO TEST WITH BORDERLINE VALUES

/*
input:
  (reference) Coordinate coordA
  (reference) Coordinate coordB
output:
  (value) floatDegree distanceInKm
remarks:
  Returns the spherical cosine law distance between two coordinate points assuming
  an average earth radius
*/
floatDegree distanceSphereCosineAtoB(
    const Coordinate* coordA,
    const Coordinate* coordB); // TO TEST WITH BORDERLINE VALUES

/*
input:
  (reference) Coordinate coordA
  (reference) Coordinate coordB
output:
  (value) floatDegree distanceInM
remarks:
  Returns the equirectangular approximation of distance between two coordinate
  points assuming an average earth radius.
*/
floatDegree distanceEquiRectAtoB(const struct Coordinate* coordA,
                                 const struct Coordinate* coordB);

/*
input:
  (reference) Coordinate coordA
  (reference) Coordinate coordB
output:
  (value) floatDegree distanceInM
remarks:
  Returns the equirectangular approximation of distance between two coordinate
  points assuming an average earth radius.
*/
floatDegree dHeadingFromAtoB(const Coordinate* coordA,
                             const Coordinate* coordB);

/*
input:
  NavState* that is being considered
output:
  floatDegree heading from current NavState location to current WP
remarks:
  Uses the spherical form of the inverse geodesic problem by Vincentin to
  calculate heading.
*/
floatDegree dHeadingToCurrentWP(NavState* navS);

/*
input:
  NavState* that is being considered
output:
  floatDegree most recent GPS heading stored in NavState
remarks:
*/
floatDegree getCurrentHeading(NavState* navS);

/*
input:
  NavState* that is being considered
output:
  floatDegree most recent GPS speed in km/h stored in NavState
remarks:
*/
floatDegree getCurrentSpeedKmh(NavState* navS);

/*
input:
  NavState* that is being considered
output:
  floatDegree heading from current NavState location to current WP
remarks:
  Properly named interface calling dHeadingToCurrentWP.
*/
floatDegree getHeadingToCurrentWP(NavState* navS);

#endif

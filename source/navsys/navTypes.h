/*
**
** NavSys library
** URL: https://github.com/thisisG
** Author: Geir Istad (geir dot istad at gmail dot com)
** Licence: MIT
**
*/

#ifndef NAVTYPES_H
#define NAVTYPES_H

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus
#include <stdint.h>

#ifdef _WIN32
#include <stdio.h>
#endif // _WIN32
#include "navconfig.h"

#ifdef __cplusplus
}
#endif // __cplusplus

// Change this type to double if additional precision is required, note that the
// PSoC does not have an FPU hence a double will take even longer to calculate
// than a float. Best to restrict it to floats unless we need the extra
// precision of a double. Another alternative would be to use a fixed point
// library to do these calculations, but while this will increase the speed of
// the calculation it will also reduce the precision.
typedef float floatDegree;

typedef int8_t signed8Degree;
typedef uint8_t unsigned8Degree;

typedef int16_t signed16Degree;
typedef uint16_t unsigned16Degree;

typedef int32_t signed32Degree;
typedef uint32_t unsigned32Degree;

typedef int8_t coordinateCode;

/* STRUCT Coordinate
ADS that contain coordinate information.
*/
typedef struct Coordinate
{
  signed16Degree dLongitude;
  signed32Degree mLongitude;
  signed16Degree dLatitude;
  signed32Degree mLatitude;
  coordinateCode priority;
} Coordinate;

/* STRUCT GpsBuffer
ADS that contain a GPS string buffer and buffer size.
*/
typedef struct GpsBuffer
{
  uint8_t newGPSString;
  int gpsBufferLength;
  char gpsStringBuffer[GPS_STR_BFR_LEN];
} GpsBuffer;

/* STRUCT SerialBuffer
ADS that contain a serial string buffer and buffer size
*/
typedef struct SerialBuffer
{
  int serialBufferLength;
  char serialStringBuffer[SERIAL_STR_BFR_LEN];
} SerialBuffer;

/* STRUCT SystemTime
ADS that contain time information, note that this is the same structure as
RTC_TIME_DATE used in PSoC Creator for time interactions.
*/
typedef struct SystemTime
{
  uint8_t Sec;
  uint8_t Min;
  uint8_t Hour;
  uint8_t DayOfWeek;
  uint8_t DayOfMonth;
  uint16_t DayOfYear;
  uint8_t Month;
  uint16_t Year;
} SystemTime;

/* ENUM CurrentNavState
Enumerated list that is used for the Finite State Machine making up the core
navigation algorithm
The first and last entries in the list are reserved to allow iteration and
should be considered invalid values.
*/
typedef enum CurrentNavState
{
  firstCurrentNavState = 0,
  closestWP,
  toWP,
  atWP,
  nextWP,
  atGoal,
  closestExceptionWP,
  toExceptionWP,
  atExceptionWP,
  nextExceptionWP,
  atExceptionGoal,
  lastCurrentNavState
} CurrentNavState;

/* STRUCT StateData
ADS that contains the data needed to evaluate the current and next state of the
system.
*/
typedef struct StateDataStructure
{
  enum CurrentNavState stateKeeper;
  struct Coordinate WPGoal;
  struct Coordinate eWPGoal;
  struct Coordinate nextWP;
  float maxWPDistance;
  float arrivalWPDistance;
  float exceptionMaxWPDistance;
  float exceptionWPArrivalDistance;
} StateDataStructure;

/* STRUCT NavState
ADS that contain the navigation status of the system. For a single platform
there should normally only be one instance of this ADS which contains the
navigation state, variables and buffers needed for the system to function.
*/
typedef struct NavState
{
  struct StateDataStructure stateData;
  struct GpsBuffer gpsBuffer;
  struct SerialBuffer serialBuffer;
  struct Coordinate currentLocation;
  // This might be changed to a waypoint stack / queue later
  struct Coordinate nextWaypoint;
  struct SystemTime time;
  floatDegree dCurrentHeading;
  floatDegree dOverallHeading;
  float currentSpeedKmh;
  float distanceToCurrentWP;
} NavState;

#endif

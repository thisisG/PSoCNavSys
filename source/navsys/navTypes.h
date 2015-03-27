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

/***********************************************
** File type declarations
***********************************************/

// Create typedefs for file pointers in order to create interfaces for
// FS_FOpen()/fopen(), FS_Read()/fread(), FS_FSeek()/fseek() etc.
#ifdef __GNUC__
typedef FS_FILE NAV_FILE;
#else
typedef FILE NAV_FILE;
#endif // __GNUC__

/* ENUM NavFileType
Numbered list describing the type of files the system is aware of and know how
to perform operations with.
*/
typedef enum NavFileType
{
  INVALID_FILE_TYPE = 0,
  WAYPOINT_LIST_FILE = 1,
  EXCEPTION_WAYPOINT_LIST_FILE = 2,
  CONFIG_FILE = 3
} NavFileType;

/* ENUM NavVersion
Numbered list describing the type version of the file we are working with in
case it is needed for further development of the system.
*/
typedef enum NavVersion
{
  INVALID_NAV_VERSION = 0,
  NAV_VERSION_1
} NavVersion;

/* ENUM NavDataType
Numbered list describing the type of data blocks the system is aware of and know
how to perform operations on.
*/
typedef enum NavDataType
{
  INVALID_DATA_TYPE = 0,
  WAYPOINT_DATA = 1,
  EXCEPTION_WAYPOINT_DATA = 2
} NavDataType;

/* STRUCT NavFileHeader
ADS that contain information of the type of file we have opened.
Any file which the system intends to read and write to should have one of these
present as the first entry in the file.
The exception are text files which are opened in append mode only, examples of
this would a system log file which are made in pure text and not intended to be
read back by the PSoC.
*/
typedef struct NavFileHeader
{
  uint8_t fileType;
  uint8_t fileVersion;
  // The size of the header block after the header.
  // This feature is currently not used, but the space is reserved for future
  // development of the system.
  uint32_t nextHeaderSize;
} NavFileHeader;

/* STRUCT NavConfigFileHeader
ADS containing the number of WP lists and exception WP lists.
The file also contains the currently used list and should be updated to the
correct number when a new list is chosen in order to resume operations in case
of power failure or other types of system resets.
After this header there will be numberOfWPLists + numberOfExeptionWPLists
char[20] arrays containing the filenames of the lists the system should be aware
of.
*/
typedef struct NavConfigFileHeader
{
  uint32_t numberOfWPLists;
  uint32_t currentWPList;
  uint32_t numberOfExeptionWPLists;
  uint32_t currentExeptionWPList;
} NavConfigFileHeader;

/* STRUCT NavFileWPListHeader
ADS that describes the contents of a list of waypoints contained in the file.
*/
typedef struct NavFileWPListHeader
{
  Coordinate startCoordinate;
  Coordinate endCoordinate;
  uint32_t numberOfEntries;
  // The size of the header block after the header.
  // This feature is currently not used, but the space is reserved for future
  // development of the system.
  uint32_t nextHeaderSize;
} NavFileWPListHeader;

/* STRUCT NavDatablockHeader
ADS that contains information about the block of data following the header.
Any datablock should have one of these in front of them in order to allow for
future expansion of datatypes, and also allow older versions of the software to
more recent versions of stored data.
*/
typedef struct NavDatablockHeader
{
  uint8_t dataType;
  uint8_t dataVersion;
  // The size of the data block after the header.
  // This feature is currently not used, but the space is reserved for future
  // development of the system.
  uint32_t nextDataSize;
} NavDatablockHeader;

/***********************************************
** WPHandler types
***********************************************/

/* STRUCT NavWPFileManager
ADS that contain the filenames and pointers used for managing waypoints via
files.
*/
typedef struct NavWPFileManager
{
  char cfgFileName[20];
  char wpListFileName[20];
  char eWPListFileName[20];
  NAV_FILE* ptrCfgFile;
  NAV_FILE* ptrWPList;
  size_t numberOfExceptionEntries;
  size_t currentExceptionEntry;
} NavWPFileManager;

/* STRUCT NavWPHandler
ADS that contain data regarding the management of waypoints.
*/
typedef struct NavWPHandler
{
  struct NavWPFileManager fileManager;
  struct Coordinate wpGoal;
  size_t offsetFirstWPBlock;
  size_t currentWPCount;
  size_t maxWPCount;
} NavWPHandler;

/***********************************************
** Main types
***********************************************/

/* STRUCT StateData
ADS that contains the data needed to evaluate the current and next state of the
system.
*/
typedef struct StateDataStructure
{
  enum CurrentNavState stateKeeper;
  struct NavWPHandler WPHandler;
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

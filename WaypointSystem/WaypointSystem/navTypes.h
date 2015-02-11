#ifndef NAVTYPES_H
#define NAVTYPES_H

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus
#include <stdint.h>

#ifdef _WIN32
#include <stdio.h>
#endif // _WIN32
#include "config.h"

#ifdef __cplusplus
}
#endif // __cplusplus

// Change this type to double if additional precision is required, note that the PSoC does not have
// an FPU hence a double will take even longer to calculate than a float. Best to restrict it to
// floats unless we need the extra precision of a double. Another alternative would be to use a
// fixed point library to do these calculations, but while this will increase the speed of the
// calculation it will also reduce the precision.
typedef float floatDegree;

typedef int8_t signed8Degree;
typedef uint8_t unsigned8Degree;

typedef int16_t signed16Degree;
typedef uint16_t unsigned16Degree;

typedef int32_t signed32Degree;
typedef uint32_t unsigned32Degree;

typedef int8_t coordinateCode;

/* STRUCT COORDINATE
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

/* STRUCT GPSBUFFER
ADS that contain a GPS string buffer and buffer size.
*/
typedef struct GpsBuffer
{
    int gpsBufferLength;
    char gpsStringBuffer[GPS_STR_BFR_LEN];
} GpsBuffer;

/* STRUCT SERIALBUFFER
ADS that contain a serial string buffer and buffer size
*/
typedef struct SerialBuffer
{
    int serialBufferLength;
    char serialStringBuffer[SERIAL_STR_BFR_LEN];
}SerialBuffer;

/* STRUCT SYSTEMTIME
ADS that contain time information, note that this is the same structure as RTC_TIME_DATE used in
PSoC Creator for time interactions.
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

/* STRUCT NAVSTATE
ADS that contain the navigation status of the system. For a single platform there should normally
only be one instance of this ADS which contains the navigation state, variables and buffers needed
for the system to function.
*/
typedef struct NavState
{
    struct GpsBuffer gpsBuffer;
    struct SerialBuffer serialBuffer;
    struct Coordinate currentLocation;
    struct Coordinate nextWaypoint; // This might be changed to a waypoint stack / queue later
    struct SystemTime time;
    floatDegree dCurrentHeading;
    floatDegree dOverallHeading;
    float currentSpeedKmh;
} NavState;

#endif
#ifndef NAVTYPES_H
#define NAVTYPES_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

#ifdef _WIN32
#include <stdio.h>
#endif // _WIN32

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

#define GPS_STR_BFR_LEN 128

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
};

/* STRUCT NAVSTATE
ADS that contain the navigation status of the system. For a single platform there should normally
only be one instance of this ADS which contains the navigation state and variables.
*/
typedef struct NavState
{
    struct GpsBuffer gpsBuffer;
    struct Coordinate currentLocation;
    struct Coordinate nextWaypoint; // This might be changed to a waypoint stack / queue later
    floatDegree dCurrentHeading;
    floatDegree dOverallHeading;
} NavState;

void zeroCoordinate(Coordinate* coord);
void zeroGpsBuffer(GpsBuffer* gpsB);
void zeroNavState(NavState* navS);

#ifdef _WIN32
void printCoordData(Coordinate* coord);
void printCurrentCoordAndHeading(NavState* navS);
#endif // _WIN32

#endif
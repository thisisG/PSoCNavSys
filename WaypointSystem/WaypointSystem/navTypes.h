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

// Change this type to double if additional precision is required
typedef float floatDegree;

typedef int8_t signed8Degree;
typedef uint8_t unsigned8Degree;

typedef int16_t signed16Degree;
typedef uint16_t unsigned16Degree;

typedef int32_t signed32Degree;
typedef uint32_t unsigned32Degree;

typedef int8_t coordinateCode;

/* STRUCT COORDINATE
input:
    NA
output:
    NA
remarks:
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

/* STRUCT NAVSTATE
input:
    NA
output:
    NA
remarks:
    ADS that contain the navigation status of the system. For a single platform there should only
    one instance of this ADS which contains the navigation state and variables.
*/
typedef struct NavState
{
    struct Coordinate currentLocation;
    struct Coordinate nextWaypoint; // This might be changed to a waypoint stack / queue later
    floatDegree dCurrentHeading;
    floatDegree dOverallHeading;
} NavState;

void zeroCoordinate(Coordinate* coord);
void printCoordData(Coordinate* coord);
void zeroNavState(NavState* navS);

#ifdef _WIN32
void printCoordData(Coordinate* coord);
void printCurrentCoordAndHeading(NavState* navS);
#endif // _WIN32

#endif
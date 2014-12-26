#ifndef NAVTYPES_H
#define NAVTYPES_H

#ifdef _MSC_BUILD // For coding in VS where the types below are undefined
#include <stdint.h>
typedef int8_t int8;
typedef uint8_t uint8;

typedef int16_t int16;
typedef uint16_t uint16;

typedef int32_t int32;
typedef uint32_t uint32;
#endif

// Change this type to double if additional precision is required
typedef float floatDegree;

typedef int8 signed8Degree;
typedef uint8 unsigned8Degree;

typedef int16 signed16Degree;
typedef uint16 unsigned16Degree;

typedef int32 signed32Degree;
typedef uint32 unsigned32Degree;

typedef int8 coordinateCode;

enum NavStates
{
    Uninitialized = 0,
    SearchingForNextWp,
    HeadingToWp,
    ReachedWp
};

struct Coordinate
{
    signed16Degree dLongitude;
    signed32Degree mLongitude;
    signed16Degree dLatitude;
    signed32Degree mLatitude;

    coordinateCode priority;
};

struct NavCore
{
    struct Coordinate currentPosition;
    struct Coordinate nextWaypoint;
    struct Coordinate waypointStack[10];
    enum NavStates CurrentState;
};

#endif
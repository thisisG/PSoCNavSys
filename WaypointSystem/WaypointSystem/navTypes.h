#ifndef NAVTYPES_H
#define NAVTYPES_H
    
#include <stdint.h>
    
#ifdef _MSC_BUILD // For coding in VS where the types below are undefined

#endif

// Change this type to double if additional precision is required
typedef float floatDegree;

typedef int8_t signed8Degree;
typedef uint8_t unsigned8Degree;

typedef int16_t signed16Degree;
typedef uint16_t unsigned16Degree;

typedef int32_t signed32Degree;
typedef uint32_t unsigned32Degree;

typedef int8_t coordinateCode;

<<<<<<< HEAD
/* STRUCT COORDINATE
input:
    NA
output:
    NA
remarks:
    ADS that contain coordinate information.
*/
=======
enum NavStates
{
    Uninitialized = 0,
    SearchingForNextWp,
    HeadingToWp,
    ReachedWp
};

>>>>>>> origin/master
struct Coordinate
{
    signed16Degree dLongitude;
    signed32Degree mLongitude;
    signed16Degree dLatitude;
    signed32Degree mLatitude;

    coordinateCode priority;
};

<<<<<<< HEAD
/* STRUCT NAVSTATE
input:
    NA
output:
    NA
remarks:
    ADS that contain the navigation status of the system. For a single platform there should only one instance of this ADS which contains the navigation state and variables.
*/
struct NavState
{
    struct Coordinate currentLocation;
    struct Coordinate nextWaypoint; // This might be changed to a waypoint stack / queue later
    floatDegree dCurrentHeading;
    floatDegree dOverallHeading;
};


=======
struct NavCore
{
    struct Coordinate currentPosition;
    struct Coordinate nextWaypoint;
    struct Coordinate waypointStack[10];
    enum NavStates CurrentState;
};

>>>>>>> origin/master
#endif
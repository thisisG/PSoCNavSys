/*
Source code written by Geir Istad for the completion of a MEng thesis at the University of Aberdeen
Code found here is written by the author unless otherwise indicated.

Format for longitude/latitude is as follows:

N/S +/-
E/W +/-

Angles are denoted by a prefix r for the angle in radians and d for angle in degrees.
Note that for mathematical operations the angle should be in radians while for coordinate storage the angle is in degrees.

Longitude and latitude is stored as a pair of integers. a signed 16 bit integer is used to store the degrees while a signed 32 bit integer is used to store the minutes as MM.mmmm with 6 significant digits. The minutes are stored as MMmmmm and by dividing by 10^4 we get the minutes with fractions. This is then converted to fractional degrees by dividing by 60.


*/

// FUNCTION DESCRIPTION
/* FUNCTIONNAME
input:

output:

remarks:

*/

#define NAVFUNCDEBUG

#ifndef NAVFUNCTIONS_H
#define NAVFUNCTIONS_H

// Local includes
#include "navTypes.h"

// Standard library includes
#include <math.h> // Need atan2(), cos(), sin()

static const float pi = 3.141592;
static const float earthRadiusKM = 6372.8; // Average earth radius in metres

/* longDegreeToFloat();
input:  
    (reference) signed16Degree degree
    (reference) signed32Degree minutes
output: 
    (value) floatDegree fracDegree
remarks: 
    Converts a set of two numbers representing a coordinate to a floating point value with fractional parts.
    Works for both positive and negative degree.
    Note that the value calculated could be both degrees or radians, so care should be taken to convert the results to the appropriate form before performing mathematical operations on the result.
*/
floatDegree longDegreeToFloat(const signed16Degree *degree, const signed32Degree *minutes); // OK

/* latitudeFromCoordinate();
input:
    (reference) Coordinate thisCoord
output:
    (value) floatDegree latitude
remarks:
    Extracts the latitude from a Coordinate ADS and returns a fractional floating value.
    Note that the value stored in the ADS should be in degrees and NOT radians. Care should be taken to ensure that this is upheld throughout the program.
*/
floatDegree latitudeFromCoordinate(const struct Coordinate *thisCoord); // OK

/* longitudeFromCoordinate();
input:
    (reference) Coordinate thisCoord
output:
    (value) floatDegree latitude
remarks:
    Extracts the longitude from a Coordinate ADS and returns a fractional floating value.
    Note that the value stored in the ADS should be in degrees and NOT radians. Care should be taken to ensure that this is upheld throughout the program.
*/
floatDegree longitudeFromCoordinate(const struct Coordinate *thisCoord); // OK

floatDegree toDegree(const floatDegree *rAngle); // TO TEST
floatDegree toRadian(const floatDegree *dAngle); // TO TEST

floatDegree haversine(const floatDegree *rAngle); // TO TEST WITH BORDERLINE VALUES
floatDegree inverseHaversine(const floatDegree *rAngle); // TO TEST WITH BORDERLINE VALUES

/* longitudeFromCoordinate();
input:
    (reference) Coordinate coordA
    (reference) Coordinate coordB
output:
    (value) floatDegree distanceInKm
remarks:
    Returns the great circle distance between two coordinate points assuming an average earth radius of 6372.8 km.
*/
floatDegree distanceCirclePath(const struct Coordinate *coordA, const struct Coordinate *coordB); // TO TEST WITH BORDERLINE VALUES

#endif
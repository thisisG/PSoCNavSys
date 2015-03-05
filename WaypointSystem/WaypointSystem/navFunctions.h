/*
Source code written by Geir Istad for the completion of a MEng thesis at the
University of Aberdeen
The code is written by the author unless otherwise indicated.

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
#endif
#include <stdint.h>

#ifdef _WIN32
#include <stdio.h>
#endif // _WIN32

#include "navtypes.h"
#include "navmath.h"
#ifdef __cplusplus
}
#endif

void zeroCoordinate(Coordinate* coord);
void initGpsBuffer(GpsBuffer* gpsB);
void initSerialBuffer(SerialBuffer* serialB);
void zeroSystemTime(SystemTime* time);
void zeroNavState(NavState* navS);

void updateNavState(NavState* navS);

#ifdef _WIN32
void printCoordData(Coordinate* coord);
void printCurrentCoordAndHeading(NavState* navS);
#endif // _WIN32

/* latitudeFromCoordinate();
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

/* longitudeFromCoordinate();
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

/* distanceCirclePathAtoB();
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

/* distanceSphereCosineAtoB();
input:
    (reference) Coordinate coordA
    (reference) Coordinate coordB
output:
    (value) floatDegree distanceInKm
remarks:
    Returns the spherical cosine law distance between two coordinate points
    assuming an average earth radius
*/
floatDegree distanceSphereCosineAtoB(
    const Coordinate* coordA,
    const Coordinate* coordB); // TO TEST WITH BORDERLINE VALUES

/* distanceEquiRectAtoB();
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

floatDegree dHeadingFromAtoB(const Coordinate* coordA,
                             const Coordinate* coordB);

floatDegree dHeadingToCurrentWP(NavState* navS);

#endif
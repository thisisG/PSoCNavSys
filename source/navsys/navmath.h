/*
**
** NavSys library
** URL: https://github.com/thisisG
** Author: Geir Istad (geir dot istad at gmail dot com)
** Licence: MIT
**
*/

#ifndef NAVMATH_H
#define NAVMATH_H

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

// Standard library includes
#ifdef _WIN32
#include <stdio.h>
#endif // _WIN32

#include <math.h> // Need atan2(), cos(), sin()
#include <stdint.h>

// Local includes
#include "navtypes.h"

#ifdef __cplusplus
}
#endif // __cplusplus

// Define M_PI if not defined previously, note it is a double since no f
// appended
#ifndef M_PI
#define M_PI 3.1415926535
#endif

// Average earth radius in metres
static const double earthRadiusM = 6371008.7714;

/*
input:
  const signed16Degree degree
  const signed32Degree minutes
output:
  (value) floatDegree fracDegree
remarks:
  Converts a set of two numbers representing a coordinate to a floating point
  value with fractional parts.
  Works for both positive and negative degree.
  Note that the value calculated could be both degrees or radians, so care
  should be taken to convert the results to the appropriate form before
  performing mathematical operations on the result.
*/
floatDegree floatFromLongDegree(const signed16Degree degree,
                                const signed32Degree minutes); // OK

/*
input:
  const floatDegree that is converted to two integer types for storage in
  Coordinate structures
output:
  signed16Degree* that is filled with the sign and full degrees of the float
  signed32Degree* that is filled with minutes and fractional minutes of the
  converted value in the form MMmmmmmm
remarks:
*/
void longDegreeFromFloatDegree(const floatDegree fDegree,
                               signed16Degree* sDegree,
                               signed32Degree* sMinutes);

/*
input:
  const floatDegree in radians
output:
  floatDegree in degrees
remarks:
*/
floatDegree toDegree(const floatDegree rAngle);

/*
input:
  const floatDegree in degrees
output:
  floatDegree in radians
remarks:
*/
floatDegree toRadian(const floatDegree dAngle);

/*
input:
  const floatDegree in radians
output:
  floatDegree with the haversine of the angle
remarks:
  See R.W. Sinnott. Virtues of the haversine. Sky & Telescope, 68:159, 1984. for
  an explanation of the formula and why it is better to use with floating
  numbers.
*/
floatDegree rHaversine(const floatDegree rAngle);

/*
input:
  const floatDegree in radians
output:
  floatDegree with the inverse haversine of the angle
remarks:
  See R.W. Sinnott. Virtues of the haversine. Sky & Telescope, 68:159, 1984. for
  an explanation of the formula and why it is better to use with floating
  numbers.
*/
floatDegree rInverseHaversine(const floatDegree rAngle);

/*
input:
  const floatDegree latA in radians
  const floatDegree lonA in radians
  const floatDegree latB in radians
  const floatDegree lonB in radians
output:
  floatDegree with the great circle distance between the two points
remarks:
  Produces a great circle distance between point A and B using the haversine
  formula to calculate the angular separation between two points on a sphere.
*/
floatDegree distanceCirclePath(const floatDegree rLatA, const floatDegree rLonA,
                               const floatDegree rLatB,
                               const floatDegree rLonB);

/*
input:
  const floatDegree latA in radians
  const floatDegree lonA in radians
  const floatDegree latB in radians
  const floatDegree lonB in radians
output:
  floatDegree with the great circle distance between the two points
remarks:
  Uses the spherical cosine law to calculate the great circle distance between
  two points.
*/
floatDegree distanceSphereCosine(const floatDegree rLatA,
                                 const floatDegree rLonA,
                                 const floatDegree rLatB,
                                 const floatDegree rLonB);

/*
input:
  const floatDegree latA in radians
  const floatDegree lonA in radians
  const floatDegree latB in radians
  const floatDegree lonB in radians
output:
  floatDegree with the equirectangular approximation of the distance between two
  points.
remarks:
  Approximation of distance using the difference in angles. Inaccurate for large
  differences in angles as it assumes a straight edge triangle and uses the
  hypotenuse as the distance.
*/
floatDegree distanceEquiRect(const floatDegree rLatA, const floatDegree rLonA,
                             const floatDegree rLatB, const floatDegree rLonB);

/*
input:
  const floatDegree latA in radians
  const floatDegree lonA in radians
  const floatDegree latB in radians
  const floatDegree lonB in radians
output:
  floatDegree in degrees for the heading from A to B
remarks:
  Uses the spherical version of Vincenti's solution to the inverse geodesic
  problem to get the initial heading for travelling from A to B.
*/
floatDegree dInitialHeading(const floatDegree dLatA, const floatDegree dLonA,
                            const floatDegree dLatB, const floatDegree dLonB);

#endif // NAVMATH_H

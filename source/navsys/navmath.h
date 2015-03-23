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

/* floatFromLongDegree();
input:
signed16Degree degree
signed32Degree minutes
output:
(value) floatDegree fracDegree
remarks:
Converts a set of two numbers representing a coordinate to a floating point
value with fractional parts.
Works for both positive and negative degree.
Note that the value calculated could be both degrees or radians, so care should
be taken to convert the results to the appropriate form before performing
mathematical operations on the result.
*/
floatDegree floatFromLongDegree(const signed16Degree degree,
                                const signed32Degree minutes); // OK

// TODO
void longDegreeFromFloatDegree(const floatDegree fDegree,
                               signed16Degree* sDegree,
                               signed32Degree* sMinutes);

// TODO
floatDegree
toDegree(const floatDegree rAngle); // TO TEST WITH BORDERLINE VALUES

// TODO
floatDegree
toRadian(const floatDegree dAngle); // TO TEST WITH BORDERLINE VALUES

// TODO
floatDegree
rHaversine(const floatDegree rAngle); // TO TEST WITH BORDERLINE VALUES

// TODO
floatDegree
rInverseHaversine(const floatDegree rAngle); // TO TEST WITH BORDERLINE VALUES

// TODO
floatDegree distanceCirclePath(const floatDegree rLatA, const floatDegree rLonA,
                               const floatDegree rLatB,
                               const floatDegree rLonB);

// TODO
floatDegree distanceSphereCosine(const floatDegree rLatA,
                                 const floatDegree rLonA,
                                 const floatDegree rLatB,
                                 const floatDegree rLonB);

// TODO
floatDegree distanceEquiRect(const floatDegree rLatA, const floatDegree rLonA,
                             const floatDegree rLatB, const floatDegree rLonB);

// TODO
floatDegree dInitialHeading(const floatDegree dLatA, const floatDegree dLonA,
                            const floatDegree dLatB, const floatDegree dLonB);

#endif // NAVMATH_H

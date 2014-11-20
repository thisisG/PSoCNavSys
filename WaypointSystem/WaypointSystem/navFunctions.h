/*
Source code written by Geir Istad for the completion of a MEng thesis at the University of Aberdeen
Code found here is written by the author unless otherwise indicated.

Format for longitude/latitude is as follows:

N/S +/-
E/W +/-

*/

#ifndef NAVFUNCTIONS_H
#define NAVFUNCTIONS_H

// Local includes
#include "navTypes.h"

// Standard library includes
#include <math.h> // Need atan2(), cos(), sin()

coordDegree dmsToCoord(arcDegree degree, arcMinute minute, arcSecond second);
coordDegree latitudeFromCoordinates(struct Coordinate *thisCoord);
coordDegree longitudeFromCoordinates(struct Coordinate *thisCoord);


#endif
#include "navFunctions.h"

#include <stdio.h>


int main()
{
    printf("test\n");
    arcDegree deg = 10;
    arcMinute min = 2;
    arcSecond sec = 23;
    coordDegree thisDeg = dmsToCoord(deg, min, sec);
    printf("%d:%d:%d = %f \n",deg,min,sec, thisDeg);

    struct Coordinate myCoords;

    myCoords.latitudeDegree = deg;
    myCoords.latitudeMinute = min;
    myCoords.latitudeSecond = sec;

    coordDegree latDeg = latitudeFromCoordinates(&myCoords);
    printf("%d:%d:%d = %f \n", deg, min, sec, latDeg);
    coordDegree lonDeg = longitudeFromCoordinates(&myCoords);
    printf("%d:%d:%d = %f \n", deg, min, sec, lonDeg);

    return 0;
}
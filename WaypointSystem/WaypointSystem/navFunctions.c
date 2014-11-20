#include "navFunctions.h"

coordDegree dmsToCoord(arcDegree degree, arcMinute minute, arcSecond second)
{
    return( (coordDegree)degree + (((coordDegree)(minute)+((coordDegree)(second) / 10)) / 60));
}

coordDegree latitudeFromCoordinates(struct Coordinate *thisCoord)
{
    return dmsToCoord(thisCoord->latitudeDegree, thisCoord->latitudeMinute, thisCoord->latitudeSecond);
}

coordDegree longitudeFromCoordinates(struct Coordinate *thisCoord)
{
    return dmsToCoord(thisCoord->longitudeDegree, thisCoord->longitudeMinute, thisCoord->longitudeSecond);
}
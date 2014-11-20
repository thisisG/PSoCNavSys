#ifndef NAVTYPES_H
#define NAVTYPES_H

#ifdef _MSC_BUILD // For coding in VS where int8 is not defined, typedef int8 as __int8
typedef __int8 int8;
#endif

typedef float coordDegree;
typedef int8 arcDegree, arcMinute, arcSecond;
typedef int8 coordinateCode;

struct Coordinate
{
    arcDegree latitudeDegree;
    arcMinute latitudeMinute;
    arcSecond latitudeSecond;

    arcDegree longitudeDegree;
    arcMinute longitudeMinute;
    arcSecond longitudeSecond;

    coordinateCode priority;
};

#endif
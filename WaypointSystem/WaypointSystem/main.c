#include <stdio.h>
#include <iostream>
#ifdef __cplusplus
extern "C"
{
#endif

#include "navfunctions.h"
#include "navtypes.h"

#ifdef __cplusplus
}
#endif

int main()
{
    /*
    Test for longDegreeToFloat()
    */
    // First create a sequence of bits of known value and compare the calculated value to hand calculated values
    // Define the degrees we want to test with

    // Use degA = 32(000000100000), degB = -33
    // Set the degrees, one negative and one positive. Should use a signed 8 bit integer
    signed16Degree degA = 32;
    signed16Degree degB = -33;

    // Define the minutes we want to test with
    signed32Degree minA = 573688;
    signed32Degree minB = 239534;

    floatDegree floatingDegreesA = longDegreeToFloat(degA, minA);
    printf("longDegreeToFloat() = %6f\n", floatingDegreesA);

    floatDegree floatingDegreesB = longDegreeToFloat(degB, minB);
    printf("longDegreeToFloat() = %6f\n", floatingDegreesB);

    // Load the set into coordinate ADS
    struct Coordinate coordA, coordB;

    coordA.dLatitude = degA;
    coordA.mLatitude = minA;
    coordA.dLongitude = degB;
    coordA.mLongitude = minB;

    floatingDegreesA = latitudeFromCoordinate(&coordA);
    printf("latitudeFromCoordinate(&coordA) = %6f\n", floatingDegreesA);

    floatingDegreesB = longitudeFromCoordinate(&coordA);
    printf("longitudeFromCoordinate(&coordA) = %6f\n", floatingDegreesB);

    // Test if coordinates work in an array
    struct Coordinate coordArray[100];

    for (int arrayIndex = 0; arrayIndex < 100; arrayIndex++)
    {
        coordArray[arrayIndex].dLatitude = degA;
        coordArray[arrayIndex].mLatitude = minA;
        coordArray[arrayIndex].dLongitude = degB;
        coordArray[arrayIndex].mLongitude = minB;
    }

    // Check a few random ones if the correct value is stored
    printf("latitudeFromCoordinate(&coordArray[0]) = %6f\n", latitudeFromCoordinate(&coordArray[0]));
    printf("latitudeFromCoordinate(&coordArray[65]) = %6f\n", latitudeFromCoordinate(&coordArray[65]));
    printf("latitudeFromCoordinate(&coordArray[99]) = %6f\n", latitudeFromCoordinate(&coordArray[99]));

    /*
    Test the toDegree() and toRadian() functions
    */
    floatDegree inDegA = 213.7453;
    floatDegree inRadA = toRadian(inDegA);
    printf("%f in deg = %f in rad\n", inDegA, inRadA);

    floatDegree inDegB = 180;
    floatDegree inRadB = toRadian(inDegB);
    printf("%f in deg = %f in rad\n", inDegB, inRadB);

    floatDegree inDegC = 0;
    floatDegree inRadC = toRadian(inDegC);
    printf("%f in deg = %f in rad\n", inDegC, inRadC);

    floatDegree inDegD = 360;
    floatDegree inRadD = toRadian(inDegD);
    printf("%f in deg = %f in rad\n", inDegD, inRadD);

    /*
    Test great circle path function
    */
    // CoordA should be lat 36d37'26.65"N, lon 33d38'6.48"W
    // Expecting a length around 1750512 m for this set
    // dlatA = 36, mlatA = 37.4442 or dlatA = 36.62407
    // dlonA = -33, mlonA = 38.1080 or dlongA = -33.63513
    coordA.dLatitude = 36;
    coordA.mLatitude = 374442;
    coordA.dLongitude = -33;
    coordA.mLongitude = 381080;

    // CoordB should be lat 23d56'25.70"N, lon 44d31'18.55"W
    // dlatB = 23, mlatA = 56.4283 or dlatB = 23.94047167
    // dlonB = -44, mlonA = 31.3091 or dlongB = -44.5218183333
    coordB.dLatitude = 23;
    coordB.mLatitude = 564283;
    coordB.dLongitude = -44;
    coordB.mLongitude = 313091;

    float distanceAB = distanceCirclePath(&coordA, &coordB);
    printf("haversine distance from coordA to coordB is %f\n", distanceAB);

    /*
    Test law of spherical cosines function
    */
    distanceAB = distanceSphereCosine(&coordA, &coordB);
    printf("spherical cosine distance from coordA to coordB is %f\n", distanceAB);

    /*
    Test equirectangular approximation function
    */
    distanceAB = distanceEquiRect(&coordA, &coordB);
    printf("equirectangular approximation distance from coordA to coordB is %f\n", distanceAB);

    /*
    Test the creation of a NavState and its initialization
    */
    struct NavState myNavState = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
    std::cout << "0 latitude in myNavState.currentLocation: " << myNavState.currentLocation.dLatitude << std::endl;

    myNavState.currentLocation = coordA;
    std::cout << "New latitude in myNavState.currentLocation: " << myNavState.currentLocation.dLatitude << std::endl;

    /*
    Random test area
    */
    //floatDegree testRadianFromFunction = distanceCirclePath(&coordA, &coordA);
    //printf("expecting latitude, should be 32.956146 distanceCirclePath(&coordA, &coordA) = %f\n", testRadianFromFunction);
    return 0;
}
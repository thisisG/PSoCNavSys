#include "navFunctions.h"

#include <stdio.h>


int main()
{
    unsigned32Degree allBits = 4294967295;

    // Test for longDegreeToFloat()
    // First create a sequence of bits of known value and compare the calculated value to hand calculated values
    // Define the degrees we want to test with

    // Use degA = 32(000000100000), degB = -33
    // Set the degrees, one negative and one positive. Should use a signed 8 bit integer
    signed16Degree degA = 32;
    signed16Degree degB = -33;

    // Define the minutes we want to test with
    // Use minA = 57.3688 (573688 = 10001100000011111000), minB = 
    signed32Degree minA = 573688;
    signed32Degree minB = 239534;

    floatDegree floatingDegreesA = longDegreeToFloat(&degA, &minA);

    printf("longDegreeToFloat() = %6f\n", floatingDegreesA);

    floatDegree floatingDegreesB = longDegreeToFloat(&degB, &minB);

    printf("longDegreeToFloat() = %6f\n", floatingDegreesB);

    // Load the set into coordinate ADS
    
    struct Coordinate coordA;

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

    return 0;
}
// NavTester.cpp : Defines the entry point for the console application.
//

#include <stdio.h>
#include <iostream>
#include <fstream>

#include "./../WaypointSystem/gpsemulator.h"

#ifdef __cplusplus
extern "C" {
#endif

#include "./../WaypointSystem/navtypes.h"
#include "./../WaypointSystem/navfunctions.h"
#include "./../WaypointSystem/serialcom.h"
#include "./../WaypointSystem/gpsinterface.h"

#ifdef __cplusplus
}
#endif

int main()
{
    NavState myNavState;
    zeroNavState(&myNavState);

    GpsEmulator myGpsEmulator;
    myGpsEmulator.parseCsvToInfoVector("./../TestData/csvtestC.csv");
    // myGpsEmulator.generatePseudoRandomData();

    std::ofstream myFile;
    myFile.open("./../TestData/outcsvtestC.csv",
                std::ios_base::out | std::ios_base::binary);

    floatDegree lastLat = NAN;
    floatDegree lastLon = NAN;
    floatDegree thisLat = NAN;
    floatDegree thisLon = NAN;
    bool newData = true;
    int varCount = 0;

    while (newData)
    {
        myGpsEmulator.getNextStringToNavState(&myNavState);
        gpsStringToNavState(&myNavState);

        thisLat = latitudeFromCoordinate(&(myNavState.currentLocation));
        thisLon = longitudeFromCoordinate(&(myNavState.currentLocation));

        if ((thisLat == lastLat) && (thisLon == lastLon))
        {

            newData = false;
        }
        else
        {
            ++varCount;
            navDataToSerialBuffer(&myNavState);
            myFile << myNavState.serialBuffer.serialStringBuffer;
            lastLat = thisLat;
            lastLon = thisLon;
        }
    }
    std::cout << "varcount = " << varCount << std::endl;
    myFile.close();

    return 0;
}

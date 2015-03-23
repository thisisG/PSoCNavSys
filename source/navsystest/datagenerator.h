/*
**
** NavSys library
** URL: https://github.com/thisisG
** Author: Geir Istad (geir dot istad at gmail dot com)
** Licence: MIT
**
*/

#ifndef DATAGENERATOR_H
#define DATAGENERATOR_H

#include <iostream>
#include <fstream>
#include "navmanager\gpsemulator.h"

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

#include "./../navsys/navtypes.h"
#include "./../navsys/navfunctions.h"
#include "./../navsys/gpsinterface.h"
#include "./../navsys/navtypes.h"
#include "./../navsys/navtypes.h"
#include "./../navsys/serialcom.h"

#ifdef __cplusplus
}
#endif // __cplusplus

void csvTestGenerator()
{
  std::string filename = "csvtestC";
  std::string fileending = ".csv";
  std::string directory = "./../TestData/";

  std::string inputFile = directory + filename + fileending;
  std::string outputFile = directory + "out" + filename + fileending;
  std::string nmeaOutputFile = directory + "nmea" + filename + fileending;

  std::cout << inputFile << std::endl;
  std::cout << nmeaOutputFile << std::endl;

  NavState myNavState;
  zeroNavState(&myNavState);

  GpsEmulator myGpsEmulator;
  myGpsEmulator.parseCsvToInfoVector(inputFile);
  // myGpsEmulator.generatePseudoRandomData();

  std::ofstream myFile;
  myFile.open(outputFile, std::ios_base::out | std::ios_base::binary);

  std::ofstream myNmeaFile;
  myNmeaFile.open(nmeaOutputFile, std::ios_base::out | std::ios_base::binary);

  floatDegree lastLat = NAN;
  floatDegree lastLon = NAN;
  floatDegree thisLat = NAN;
  floatDegree thisLon = NAN;
  bool newData = true;
  int varCount = 0;

  while (newData)
  {
    myGpsEmulator.getNextStringToNavState(&myNavState);
    decodeGpsStringInNavState(&myNavState);

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
      myNmeaFile << myNavState.gpsBuffer.gpsStringBuffer;
      lastLat = thisLat;
      lastLon = thisLon;
    }
  }
  std::cout << "varcount = " << varCount << std::endl;
  myFile.close();
  myNmeaFile.close();
}

#endif // DATAGENERATOR_H

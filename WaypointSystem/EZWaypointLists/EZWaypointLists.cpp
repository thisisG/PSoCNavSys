// EZWaypointLists.cpp : Defines the entry point for the console application.
//

#include <iostream>
#include <fstream>
#include "navmanager\gpsemulator.h"
#include "csv\csv.h"

#ifdef __cplusplus
extern "C" {
#endif
#include <stdio.h>
#include "navsys\wphandler.h"
#ifdef __cplusplus
}
#endif

void floatDegreeToCoord(const float csvLat, const float csvLon,
                        Coordinate *coord) {
  floatDegree tempFloatDeg = 0;
  signed16Degree tempSig16Deg = 0;
  signed32Degree tempSig32Deg = 0;

  tempSig16Deg = (signed16Degree)csvLat;
  coord->dLatitude = tempSig16Deg;
  tempFloatDeg = csvLat - tempSig16Deg;
  tempSig32Deg = (signed32Degree)(tempFloatDeg * 600000);
  coord->mLatitude = tempSig32Deg;

  tempSig16Deg = (signed16Degree)csvLon;
  coord->dLongitude = tempSig16Deg;
  tempFloatDeg = csvLon - tempSig16Deg;
  tempSig32Deg = (signed32Degree)(tempFloatDeg * 600000);
  coord->mLongitude = tempSig32Deg;
}

int main() {
  // Set the directory the data should be pulled from
  char dir[64] = "./../../data/ProperTest/";
  // Set config filename
  char wpConfigName[20] = DEFAULT_CONFIG_FILE;
  // Set base name of csvs where wplists and exception lists are defined
  char wpBase[20] = "Reg";
  char exWPBase[20] = "Ex";
  char csvExtension[4] = "csv";
  // Set number of wplists and exception lists
  size_t numberWPLists = 1;
  size_t numberExWPLists = 4;
  // Set base name of generated WP lists and EWP lists
  char wpOutputBase[20] = "ptWP";
  char wpOutputExtension[20] = "wp";
  char exWPOutputBase[20] = "ptEWP";
  char exWPOutputExtension[20] = "ewp";

  // Create template config file
  makeTemplateCfgFile(wpConfigName, NAV_VERSION_1);

  char tempFullName[80] = "";
  char tempShortName[20] = "";
  float tempLat = 0, tempLong = 0;
  Coordinate tempCoord;
  initCoordinate(&tempCoord);
  // For each WP csv input file generate a binary WP list
  for (size_t wpFileCount = 1; wpFileCount <= numberWPLists; wpFileCount++) {
    // Generate the file name and path and make a template WP list file
    snprintf(tempFullName, sizeof(tempFullName), "%s%s%d.%s", dir, wpBase,
             wpFileCount, csvExtension);
    snprintf(tempShortName, sizeof(tempShortName), "%s%d.%s", wpOutputBase,
             wpFileCount, wpOutputExtension);
    makeTemplateWPListFile(tempShortName, NAV_VERSION_1, WAYPOINT_LIST_FILE);
    // Add list to config file
    addWPListToCfgFile(wpConfigName, tempShortName, WAYPOINT_LIST_FILE);
    // Load the source csv file using a csv reader
    io::CSVReader<2> in(tempFullName);
    // Read all coords and load into wp file
    while (in.read_row(tempLat, tempLong)) {
      printf("read lat: %f, read lon: %f\r\n", tempLat, tempLong);
      floatDegreeToCoord(tempLat, tempLong, &tempCoord);
      appendCoordToWPListFile(tempShortName, &tempCoord, WAYPOINT_DATA);
      printCoordData(&tempCoord);
    }
  }

  // For each EWP csv input file generate a binary EWP list
  for (size_t exWPFileCount = 1; exWPFileCount <= numberExWPLists;
       exWPFileCount++) 
  {
    // Generate the file name and path and make a template WP list file
    snprintf(tempFullName, sizeof(tempFullName), "%s%s%d.%s", dir, exWPBase,
             exWPFileCount, csvExtension);
    snprintf(tempShortName, sizeof(tempShortName), "%s%d.%s", exWPOutputBase,
             exWPFileCount, exWPOutputExtension);
    makeTemplateWPListFile(tempShortName, NAV_VERSION_1, WAYPOINT_LIST_FILE);
    // Add list to config file
    addWPListToCfgFile(wpConfigName, tempShortName,
                       EXCEPTION_WAYPOINT_LIST_FILE);
    // Load the source csv file using a csv reader
    io::CSVReader<2> in(tempFullName);
    // Read all coords and load into wp file
    while (in.read_row(tempLat, tempLong)) {
      printf("read lat: %f, read lon: %f\r\n", tempLat, tempLong);
      floatDegreeToCoord(tempLat, tempLong, &tempCoord);
      appendCoordToWPListFile(tempShortName, &tempCoord, WAYPOINT_DATA);
      printCoordData(&tempCoord);
    }
  }
  return 0;
}

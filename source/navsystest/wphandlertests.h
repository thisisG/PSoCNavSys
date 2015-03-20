/*
**
** NavSys library
** URL: https://github.com/thisisG
** Author: Geir Istad (geir dot istad at gmail dot com)
** Licence: MIT
**
*/

#ifndef WPHANDLERTESTS_H
#define WPHANDLERTESTS_H

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

#include "./../navsys/wphandler.h"
#include "testhelper.h"

#ifdef __cplusplus
}
#endif // __cplusplus

void testWPHandler()
{
  uint8_t testPassed = 1;
  size_t numberItemsWritten = 0;
  size_t numberItems = 0;
  const char testName[64] = "testWPHandler()";
  const int arrayLength = 5;
  const char testFileName[20] = "wphndlr.tst";
  NAV_FILE* navFile;

  printTestHeader(testName);

  //////
  // Initialise test structures
  //////

  // Test structure
  NavWPHandler testWPHandler;
  initNavWPHandler(&testWPHandler);

  // File structures
  NavFileHeader fileHeader;
  initNavFileHeader(&fileHeader);

  NavFileWPListHeader WPListHeader;
  initNavFileWPListHeader(&WPListHeader);

  // Data structures
  NavDatablockHeader navDataHdrArrayIn[arrayLength];
  NavDatablockHeader navDataHdrArrayOut[arrayLength];
  Coordinate coordIn[arrayLength];
  Coordinate coordOut[arrayLength];

  // Initialise test arrays
  size_t i = 0;
  for (i = 0; i < arrayLength; i++)
  {
    initNavDatablockHeader(&navDataHdrArrayIn[i]);
    initNavDatablockHeader(&navDataHdrArrayOut[i]);

    zeroCoordinate(&coordIn[i]);
    zeroCoordinate(&coordOut[i]);
  }

  //////
  // Load test data
  //////

  // Load the input arrays with data
  for (i = 0; i < arrayLength; i++)
  {
    navDataHdrArrayIn[i].dataType = WAYPOINT_DATA;
    navDataHdrArrayIn[i].dataVersion = NAV_VERSION_1;
    // The data size should be the real size of a coordinate but is currently
    // not used.
    // Here to allow the system to be further developed.
    navDataHdrArrayIn[i].nextDataSize = 0;

    coordIn[i].dLatitude = 255;
    coordIn[i].dLongitude = i + 5;
    coordIn[i].mLatitude = i + 23;
    coordIn[i].mLongitude = i + 88;
    coordIn[i].priority = i + 65;
  }

  // Create a test-file with the input as generated data
  numberItemsWritten = generateWPListFile(testFileName, navDataHdrArrayIn,
                                          coordIn, arrayLength);

  // Check if the number of items written is the expected size
  // Each dataheader have 3 members, each coordinate have 5 members
  numberItems = (arrayLength * 3) + (arrayLength * 5);
  if (numberItemsWritten != numberItems)
  {
    testPassed = 0;
    NAV_printf("Number of items written to file different from expected.\r\n");
  }

  // TODO Continue here asap

  if (testPassed)
  {
    printPassed(testName);
  }
  else
  {
    printFailed(testName);
  }
}

#endif // WPHANDLERTESTS_H

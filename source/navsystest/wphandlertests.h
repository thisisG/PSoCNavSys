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
  size_t readCoords = 0;
  char testFileName[20] = "wphndlr.tst";

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
  Coordinate tempCoord;
  zeroCoordinate(&tempCoord);
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

    coordIn[i].dLatitude = i;
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

  // Let WPhandler open the file
  WPHandlerOpen(&testWPHandler, testFileName);

  // Get the goal into a temp Coordinate structure
  WPHandlerGetGoal(&testWPHandler, &tempCoord);

  // Check if goal is the last entry in the array
  if (coordsEqual(&tempCoord, &(coordIn[arrayLength - 1])) == 0)
  {
    testPassed = 0;
    NAV_printf(
        "Retreived goal is not same as last entry in coordinate array.\r\n");
  }

  // Get each WP until list is empty and store in coordOut array
  i = 0;
  while (WPHandlerNextWP(&testWPHandler, &coordOut[i]))
  {
    i++;
  }

  // Check that coordIn contains same as coordOut
  for (i = 0; i < arrayLength; i++)
  {
    readCoords++;
    if (coordsEqual(&(coordIn[i]), &(coordOut[i])) == 0)
    {
      testPassed = 0;
      NAV_printf("coordIn != coordOut\r\n");
    }
  }

  // Check that we've read the correct number of coords
  if (readCoords != arrayLength)
  {
    testPassed = 0;
    NAV_printf("readCoords != arrayLength\r\n");
  }

  // Test if WPHandlerSeekWP functions as expected.
  // Three test cases - first(0), one in the middle and last(arrayLength-1).
  WPHandlerSeekWP(&testWPHandler, 0);
  zeroCoordinate(&tempCoord);
  WPHandlerNextWP(&testWPHandler, &tempCoord);
  if (coordsEqual(&(coordIn[0]), &tempCoord) == 0)
  {
    testPassed = 0;
    NAV_printf("coordIn[0] != tempCoord\r\n");
  }

  WPHandlerSeekWP(&testWPHandler, 3);
  zeroCoordinate(&tempCoord);
  WPHandlerNextWP(&testWPHandler, &tempCoord);
  if (coordsEqual(&(coordIn[3]), &tempCoord) == 0)
  {
    testPassed = 0;
    NAV_printf("coordIn[3] != tempCoord\r\n");
  }

  WPHandlerSeekWP(&testWPHandler, arrayLength - 1);
  zeroCoordinate(&tempCoord);
  WPHandlerNextWP(&testWPHandler, &tempCoord);
  if (coordsEqual(&(coordIn[arrayLength - 1]), &tempCoord) == 0)
  {
    testPassed = 0;
    NAV_printf("coordIn[arrayLength - 1] != tempCoord\r\n");
  }

  if (testPassed)
  {
    printPassed(testName);
  }
  else
  {
    printFailed(testName);
  }
}

uint8_t testmakeTemplateCfgFile()
{
  uint8_t testPassed = 1;
  const char testName[64] = "testmakeTemplateCfgFile()";
  const char testFileName[20] = "cfgtmplt.tst";
  NAV_FILE* navFile;
  NavVersion version = NAV_VERSION_1;

  printTestHeader(testName);

  makeTemplateCfgFile(testFileName, version);

  navFile = NAV_fopen(testFileName, "rb");

  NavFileHeader fileHeader;
  initNavFileHeader(&fileHeader);
  NavConfigFileHeader cfgHeader;
  initNavConfigFileHeader(&cfgHeader);

  freadNavFileHeader(&fileHeader, navFile);
  if (fileHeader.fileType != CONFIG_FILE)
  {
    testPassed = 0;
    NAV_printf("fileHeader.fileType != CONFIG_FILE\r\n");
  }
  else if (fileHeader.fileVersion != version)
  {
    testPassed = 0;
    NAV_printf("fileHeader.fileVersion != version\r\n");
  }

  freadNavConfigFileHeader(&cfgHeader, navFile);
  if (cfgHeader.numberOfWPLists != 0)
  {
    testPassed = 0;
    NAV_printf("cfgHeader.numberOfWPLists != 0");
  }
  else if (cfgHeader.currentWPList != 0)
  {
    testPassed = 0;
    NAV_printf("cfgHeader.currentWPList != 0");
  }
  else if (cfgHeader.numberOfExeptionWPLists != 0)
  {
    testPassed = 0;
    NAV_printf("cfgHeader.numberOfExeptionWPLists != 0");
  }
  else if (cfgHeader.currentExeptionWPList != 0)
  {
    testPassed = 0;
    NAV_printf("cfgHeader.currentExeptionWPList != 0");
  }

  if (testPassed == 1)
  {
    printPassed(testName);
  }
  else
  {
    printFailed(testName);
  }

  return testPassed;
}

#endif // WPHANDLERTESTS_H

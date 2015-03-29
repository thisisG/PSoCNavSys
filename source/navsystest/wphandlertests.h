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

uint8_t testWPHandler()
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
  initCoordinate(&tempCoord);
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

    initCoordinate(&coordIn[i]);
    initCoordinate(&coordOut[i]);
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
  initCoordinate(&tempCoord);
  while (WPHandlerNextWP(&testWPHandler, &tempCoord) != -1)
  {
    coordOut[i] = tempCoord;
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
  initCoordinate(&tempCoord);
  WPHandlerNextWP(&testWPHandler, &tempCoord);
  if (coordsEqual(&(coordIn[0]), &tempCoord) == 0)
  {
    testPassed = 0;
    NAV_printf("coordIn[0] != tempCoord\r\n");
  }

  WPHandlerSeekWP(&testWPHandler, 3);
  initCoordinate(&tempCoord);
  WPHandlerNextWP(&testWPHandler, &tempCoord);
  if (coordsEqual(&(coordIn[3]), &tempCoord) == 0)
  {
    testPassed = 0;
    NAV_printf("coordIn[3] != tempCoord\r\n");
  }

  WPHandlerSeekWP(&testWPHandler, arrayLength - 1);
  initCoordinate(&tempCoord);
  WPHandlerNextWP(&testWPHandler, &tempCoord);
  if (coordsEqual(&(coordIn[arrayLength - 1]), &tempCoord) == 0)
  {
    testPassed = 0;
    NAV_printf("coordIn[arrayLength - 1] != tempCoord\r\n");
  }

  printConclusion(testPassed, testName);

  return testPassed;
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

  printConclusion(testPassed, testName);

  return testPassed;
}

uint8_t testaddWPListFileToCfgFile()
{
  uint8_t testPassed = 1;
  const char testName[64] = "testaddWPListFileToCfgFile()";
  const char cfgFileName[20] = "cfgtmp.tst";
  const char WPListIn1[20] = "aAaAaA.bBb";
  const char WPListIn2[20] = "TtTtTt.OoO";
  char WPListOut1[20] = "";
  char WPListOut2[20] = "";
  NAV_FILE* navFile;
  NavVersion version = NAV_VERSION_1;

  printTestHeader(testName);

  makeTemplateCfgFile(cfgFileName, version);
  if (addWPListToCfgFile(cfgFileName, WPListIn1, WAYPOINT_LIST_FILE) != 1)
  {
    testPassed = 0;
    NAV_printf("addWPListFileToCfgFile(cfgFileName, WPListIn1) != 1\r\n");
  }

  if (addWPListToCfgFile(cfgFileName, WPListIn2, WAYPOINT_LIST_FILE) != 1)
  {
    testPassed = 0;
    NAV_printf("addWPListFileToCfgFile(cfgFileName, WPListIn2) != 1\r\n");
  }

  navFile = NAV_fopen(cfgFileName, "rb");
  NAV_fseek(navFile, SIZE_NAV_FILE_HEADER + SIZE_NAV_CFG_FILE_HEADER,
            NAV_SEEK_SET);
  NAV_fread(WPListOut1, sizeof(char[20]), 1, navFile);
  NAV_fread(WPListOut2, sizeof(char[20]), 1, navFile);
  if (strncmp(WPListIn1, WPListOut1, 20) != 0)
  {
    testPassed = 0;
    NAV_printf("strncmp(WPListIn1, WPListOut1, 20) != 0\r\n");
  }
  if (strncmp(WPListIn2, WPListOut2, 20) != 0)
  {
    testPassed = 0;
    NAV_printf("strncmp(WPListIn2, WPListOut2, 20) != 0\r\n");
  }

  printConclusion(testPassed, testName);

  return testPassed;
}

uint8_t testmakeTemplateAndAppend()
{
  uint8_t testPassed = 1;
  const char testName[64] = "testmakeTemplateAndAppend()";
  const char cfgFileName[20] = "cfgapp.tst";

  const char WPListIn1[20] = "WP1aAaA.bBb";
  const char WPListIn2[20] = "WP2TtTt.OoO";
  char WPListOut1[20] = "";
  char WPListOut2[20] = "";

  const char ExWPListIn1[20] = "EX1mMpP.bBb";
  const char ExWPListIn2[20] = "EX2EeEe.OoO";
  char ExWPListOut1[20] = "";
  char ExWPListOut2[20] = "";

  const size_t arrayLength = 5;

  Coordinate WPCoordIn1[arrayLength];
  Coordinate WPCoordIn2[arrayLength];
  Coordinate WPCoordOut1[arrayLength];
  Coordinate WPCoordOut2[arrayLength];

  Coordinate EWPCoordIn1[arrayLength];
  Coordinate EWPCoordIn2[arrayLength];
  Coordinate EWPCoordOut1[arrayLength];
  Coordinate EWPCoordOut2[arrayLength];

  NAV_FILE* cfgFile;
  NAV_FILE* WPListFile1;
  NAV_FILE* WPListFile2;
  NAV_FILE* exWPListFile1;
  NAV_FILE* exWPListFile2;
  NavVersion version = NAV_VERSION_1;

  printTestHeader(testName);

  // Create the input data and initialise all coord structures
  size_t i = 0;
  for (i = 0; i < arrayLength; i++)
  {
    initCoordinate(&WPCoordIn1[i]);
    initCoordinate(&WPCoordIn2[i]);
    initCoordinate(&WPCoordOut1[i]);
    initCoordinate(&WPCoordOut2[i]);

    WPCoordIn1[i].dLatitude = i * i;
    WPCoordIn1[i].dLongitude = i + 2;
    WPCoordIn1[i].mLatitude = i;
    WPCoordIn1[i].mLongitude = 3 * i;
    WPCoordIn1[i].priority = i;

    WPCoordIn2[i].dLatitude = -(int32_t)(i * i);
    WPCoordIn2[i].dLongitude = -(int32_t)(i + 2);
    WPCoordIn2[i].mLatitude = -(int32_t)(i);
    WPCoordIn2[i].mLongitude = -(int32_t)(3 * i);
    WPCoordIn2[i].priority = i;

    initCoordinate(&EWPCoordIn1[i]);
    initCoordinate(&EWPCoordIn2[i]);
    initCoordinate(&EWPCoordOut1[i]);
    initCoordinate(&EWPCoordOut2[i]);

    EWPCoordIn1[i].dLatitude = i * i + 3;
    EWPCoordIn1[i].dLongitude = i + 7;
    EWPCoordIn1[i].mLatitude = i * 4;
    EWPCoordIn1[i].mLongitude = 3 * i * i;
    EWPCoordIn1[i].priority = i;

    EWPCoordIn2[i].dLatitude = -(int32_t)(i * i + 3);
    EWPCoordIn2[i].dLongitude = -(int32_t)(i + 7);
    EWPCoordIn2[i].mLatitude = -(int32_t)(i * 4);
    EWPCoordIn2[i].mLongitude = -(int32_t)(3 * i * i);
    EWPCoordIn2[i].priority = i;
  }

  // Create template cfg file and template wp files
  makeTemplateCfgFile(cfgFileName, NAV_VERSION_1);
  makeTemplateWPListFile(WPListIn1, NAV_VERSION_1, WAYPOINT_LIST_FILE);
  makeTemplateWPListFile(WPListIn2, NAV_VERSION_1, WAYPOINT_LIST_FILE);
  makeTemplateWPListFile(ExWPListIn1, NAV_VERSION_1,
                         EXCEPTION_WAYPOINT_LIST_FILE);
  makeTemplateWPListFile(ExWPListIn2, NAV_VERSION_1,
                         EXCEPTION_WAYPOINT_LIST_FILE);

  // Fill the WPLists with data from input arrays
  for (i = 0; i < arrayLength; i++)
  {
    appendCoordToWPListFile(WPListIn1, &(WPCoordIn1[i]), WAYPOINT_DATA);
    appendCoordToWPListFile(WPListIn2, &(WPCoordIn2[i]), WAYPOINT_DATA);
    appendCoordToWPListFile(ExWPListIn1, &(EWPCoordIn1[i]),
                            EXCEPTION_WAYPOINT_DATA);
    appendCoordToWPListFile(ExWPListIn2, &(EWPCoordIn2[i]),
                            EXCEPTION_WAYPOINT_DATA);
  }

  // Add the wp and exception wp lists to the cfg file
  addWPListToCfgFile(cfgFileName, ExWPListIn1, EXCEPTION_WAYPOINT_LIST_FILE);
  addWPListToCfgFile(cfgFileName, WPListIn1, WAYPOINT_LIST_FILE);
  addWPListToCfgFile(cfgFileName, ExWPListIn2, EXCEPTION_WAYPOINT_LIST_FILE);
  addWPListToCfgFile(cfgFileName, WPListIn2, WAYPOINT_LIST_FILE);

  // Open the cfg file
  cfgFile = NAV_fopen("cfgapp.tst", "rb");
  // Check if names were stored correctly
  getWPListName(cfgFile, 1, WPListOut1);
  getWPListName(cfgFile, 2, WPListOut2);
  getExceptionWPListName(cfgFile, 1, ExWPListOut1);
  getExceptionWPListName(cfgFile, 2, ExWPListOut2);
  if (strncmp(WPListIn1, WPListOut1, 20) != 0)
  {
    testPassed = 0;
    NAV_printf("strncmp(WPListIn1, WPListOut1, 20) != 0\r\n");
  }
  if (strncmp(WPListIn2, WPListOut2, 20) != 0)
  {
    testPassed = 0;
    NAV_printf("strncmp(WPListIn2, WPListOut2, 20) != 0\r\n");
  }
  if (strncmp(ExWPListIn1, ExWPListOut1, 20) != 0)
  {
    testPassed = 0;
    NAV_printf("strncmp(ExWPListIn1, ExWPListOut1, 20) != 0\r\n");
  }
  if (strncmp(ExWPListIn2, ExWPListOut2, 20) != 0)
  {
    testPassed = 0;
    NAV_printf("strncmp(ExWPListIn2, ExWPListOut2, 20) != 0\r\n");
  }
  // Close the cfg file
  checkAndCloseNavFile(cfgFile);

  // Open the WPListFiles
  WPListFile1 = NAV_fopen(WPListOut1, "rb");
  WPListFile2 = NAV_fopen(WPListOut2, "rb");
  exWPListFile1 = NAV_fopen(ExWPListOut1, "rb");
  exWPListFile2 = NAV_fopen(ExWPListOut2, "rb");
  
  // Read the navFileHeader for each file
  NavFileHeader fileHdrWP1;
  initNavFileHeader(&fileHdrWP1);
  NavFileHeader fileHdrWP2;
  initNavFileHeader(&fileHdrWP2);
  NavFileHeader fileHdrEWP1;
  initNavFileHeader(&fileHdrEWP1);
  NavFileHeader fileHdrEWP2;
  initNavFileHeader(&fileHdrEWP2);

  freadNavFileHeader(&fileHdrWP1, WPListFile1);
  freadNavFileHeader(&fileHdrWP2, WPListFile2);
  freadNavFileHeader(&fileHdrEWP1, exWPListFile1);
  freadNavFileHeader(&fileHdrEWP2, exWPListFile2);
  // Check that the version and type are all well defined
  if (fileHdrWP1.fileType != WAYPOINT_LIST_FILE
      || fileHdrWP1.fileVersion != NAV_VERSION_1)
  {
    testPassed = 0;
    NAV_printf("Error in fileHdrWP1\r\n");
  }
  if (fileHdrWP2.fileType != WAYPOINT_LIST_FILE
    || fileHdrWP2.fileVersion != NAV_VERSION_1)
  {
    testPassed = 0;
    NAV_printf("Error in fileHdrWP2\r\n");
  }
  if (fileHdrEWP1.fileType != EXCEPTION_WAYPOINT_LIST_FILE
    || fileHdrEWP1.fileVersion != NAV_VERSION_1)
  {
    testPassed = 0;
    NAV_printf("Error in fileHdrEWP1\r\n");
  }
  if (fileHdrEWP2.fileType != EXCEPTION_WAYPOINT_LIST_FILE
    || fileHdrEWP2.fileVersion != NAV_VERSION_1)
  {
    testPassed = 0;
    NAV_printf("Error in fileHdrEWP2\r\n");
  }

  // Read the wp list header for each file
  NavFileWPListHeader listHdrWP1;
  initNavFileWPListHeader(&listHdrWP1);
  NavFileWPListHeader listHdrWP2;
  initNavFileWPListHeader(&listHdrWP2);
  NavFileWPListHeader listHdrEWP1;
  initNavFileWPListHeader(&listHdrEWP1);
  NavFileWPListHeader listHdrEWP2;
  initNavFileWPListHeader(&listHdrEWP2);

  freadNavFileWPListHeader(&listHdrWP1, WPListFile1);
  freadNavFileWPListHeader(&listHdrWP2, WPListFile2);
  freadNavFileWPListHeader(&listHdrEWP1, exWPListFile1);
  freadNavFileWPListHeader(&listHdrEWP2, exWPListFile2);
  // Check that the start and goals are the first and last entry of input
  // arrays.
  if (coordsEqual(&(listHdrWP1.startCoordinate),&(WPCoordIn1[0])) == 0)
  {
    testPassed = 0;
    NAV_printf("&(listHdrWP1.startCoordinate),&(WPCoordIn1[0])) == 0\r\n");
  }
  if (coordsEqual(&(listHdrWP2.startCoordinate), &(WPCoordIn2[0])) == 0)
  {
    testPassed = 0;
    NAV_printf("&(listHdrWP2.startCoordinate),&(WPCoordIn2[0])) == 0\r\n");
  }
  if (coordsEqual(&(listHdrEWP1.startCoordinate), &(EWPCoordIn1[0])) == 0)
  {
    testPassed = 0;
    NAV_printf("&(listHdrEWP1.startCoordinate),&(EWPCoordIn1[0])) == 0\r\n");
  }
  if (coordsEqual(&(listHdrEWP2.startCoordinate), &(EWPCoordIn2[0])) == 0)
  {
    testPassed = 0;
    NAV_printf("&(listHdrEWP2.startCoordinate),&(EWPCoordIn2[0])) == 0\r\n");
  }
  // Checking goals
  if (coordsEqual(&(listHdrWP1.endCoordinate), &(WPCoordIn1[arrayLength - 1]))
      == 0)
  {
    testPassed = 0;
    NAV_printf(
        "&(listHdrWP1.endCoordinate),&(WPCoordIn1[arrayLength - 1])) == 0\r\n");
  }
  if (coordsEqual(&(listHdrWP2.endCoordinate), &(WPCoordIn2[arrayLength - 1]))
      == 0)
  {
    testPassed = 0;
    NAV_printf(
        "&(listHdrWP2.endCoordinate),&(WPCoordIn2[arrayLength - 1])) == 0\r\n");
  }
  if (coordsEqual(&(listHdrEWP1.endCoordinate), &(EWPCoordIn1[arrayLength - 1]))
      == 0)
  {
    testPassed = 0;
    NAV_printf("&(listHdrEWP1.endCoordinate),&(EWPCoordIn1[arrayLength - 1])) "
               "== 0\r\n");
  }
  if (coordsEqual(&(listHdrEWP2.endCoordinate), &(EWPCoordIn2[arrayLength - 1]))
      == 0)
  {
    testPassed = 0;
    NAV_printf("&(listHdrEWP2.endCoordinate),&(EWPCoordIn2[arrayLength - 1])) "
               "== 0\r\n");
  }

  // Read in all of the coordinate data to output arrays.
  NavDatablockHeader mockBlockHdr;
  initNavDatablockHeader(&mockBlockHdr);
  for (i = 0; i < arrayLength; i++)
  {
    freadNavDatablockHeader(&mockBlockHdr, WPListFile1);
    freadCoordinate(&(WPCoordOut1[i]), WPListFile1);
    freadNavDatablockHeader(&mockBlockHdr, WPListFile2);
    freadCoordinate(&(WPCoordOut2[i]), WPListFile2);
    freadNavDatablockHeader(&mockBlockHdr, exWPListFile1);
    freadCoordinate(&(EWPCoordOut1[i]), exWPListFile1);
    freadNavDatablockHeader(&mockBlockHdr, exWPListFile2);
    freadCoordinate(&(EWPCoordOut2[i]), exWPListFile2);
  }
  // Check if outputs are same as inputs.
  for (i = 0; i < arrayLength; i++)
  {
    if (coordsEqual(&(WPCoordIn1[i]),&(WPCoordOut1[i])) == 0)
    {
      testPassed = 0;
      NAV_printf("WPCoordIn1 != WPCoordOut1");
    }
    if (coordsEqual(&(WPCoordIn2[i]), &(WPCoordOut2[i])) == 0)
    {
      testPassed = 0;
      NAV_printf("WPCoordIn2 != WPCoordOut2");
    }
    if (coordsEqual(&(EWPCoordIn1[i]), &(EWPCoordOut1[i])) == 0)
    {
      testPassed = 0;
      NAV_printf("EWPCoordIn1 != EWPCoordOut1");
    }
    if (coordsEqual(&(EWPCoordIn2[i]), &(EWPCoordOut2[i])) == 0)
    {
      testPassed = 0;
      NAV_printf("EWPCoordIn2 != EWPCoordOut2");
    }
  }
  NAV_fclose(WPListFile1);
  NAV_fclose(WPListFile2);
  NAV_fclose(exWPListFile1);
  NAV_fclose(exWPListFile2);

  printConclusion(testPassed, testName);

  return testPassed;
}

#endif // WPHANDLERTESTS_H

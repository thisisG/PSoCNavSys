/*
**
** NavSys library
** URL: https://github.com/thisisG
** Author: Geir Istad (geir dot istad at gmail dot com)
** Licence: MIT
**
*/

#ifndef NAVFILESTESTS_H
#define NAVFILESTESTS_H

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

#include "./../navsys/navfiles.h"
#include "testhelper.h"

#ifdef __cplusplus
}
#endif // __cplusplus

void testfCoordinate()
{
  uint8_t testPassed = 1;
  size_t bytesWritten = 0;
  size_t bytesRead = 0;
  const char testName[64] = "testfCoordinate()";
  const int arrayLength = 5;
  const char coordFile[20] = "coord.tst";
  NAV_FILE* navFile;
  Coordinate testArrayIn[arrayLength];
  Coordinate testArrayOut[arrayLength];

  printTestHeader(testName);
  size_t i = 0;
  for (i = 0; i < arrayLength; i++)
  {
    initCoordinate(&testArrayIn[i]);
    initCoordinate(&testArrayOut[i]);
    // Fill array with numbers that are sensible and different
    testArrayIn[i].dLatitude = i;
    testArrayIn[i].dLongitude = i + 5;
    testArrayIn[i].mLatitude = i + 23;
    testArrayIn[i].mLongitude = i + 88;
    testArrayIn[i].priority = i + 65;
  }

  navFile = NAV_fopen(coordFile, "wb");

  for (i = 0; i < arrayLength; i++)
  {
    bytesWritten += fwriteCoordinate(&testArrayIn[i], navFile);
  }

  NAV_fclose(navFile);
  navFile = NULL;

  navFile = NAV_fopen(coordFile, "rb");

  for (i = 0; i < arrayLength; i++)
  {
    bytesRead += freadCoordinate(&testArrayOut[i], navFile);
  }

  for (i = 0; i < arrayLength; i++)
  {
    if ((coordsEqual(&testArrayIn[i], &testArrayOut[i])) == 0)
    {
      testPassed = 0;
    }
  }

  if (bytesWritten != bytesRead)
  {
    testPassed = 0;
    NAV_printf("read/write size different.\r\n");
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

void testfNavFileHeader()
{
  uint8_t testPassed = 1;
  size_t bytesWritten = 0;
  size_t bytesRead = 0;
  const char testName[64] = "testfNavFileHeader()";
  const int arrayLength = 5;
  const char testFileName[20] = "hdrfile.tst";
  NAV_FILE* navFile;
  NavFileHeader testArrayIn[arrayLength];
  NavFileHeader testArrayOut[arrayLength];

  printTestHeader(testName);
  
  size_t i = 0;
  for (i = 0; i < arrayLength; i++)
  {
    initNavFileHeader(&testArrayIn[i]);
    initNavFileHeader(&testArrayOut[i]);
    // Fill array with numbers that are sensible and different
    testArrayIn[i].fileType = i;
    testArrayIn[i].fileVersion = i + 4;
    testArrayIn[i].nextHeaderSize = i + 9;
  }

  navFile = NAV_fopen(testFileName, "wb");

  for (i = 0; i < arrayLength; i++)
  {
    bytesWritten += fwriteNavFileHeader(&testArrayIn[i], navFile);
  }

  NAV_fclose(navFile);
  navFile = NULL;
  navFile = NAV_fopen(testFileName, "rb");

  for (i = 0; i < arrayLength; i++)
  {
    bytesRead += freadNavFileHeader(&testArrayOut[i], navFile);
  }

  for (i = 0; i < arrayLength; i++)
  {
    if ((compNavFileHeader(&testArrayIn[i], &testArrayOut[i])) == 0)
    {
      testPassed = 0;
    }
  }

  if (bytesWritten != bytesRead)
  {
    testPassed = 0;
    NAV_printf("read/write size different.\r\n");
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

void testfWPListHeader()
{
  uint8_t testPassed = 1;
  size_t bytesWritten = 0;
  size_t bytesRead = 0;
  const char testName[64] = "testfWPListHeader()";
  const int arrayLength = 5;
  const char testFileName[20] = "wplstf.tst";
  NAV_FILE* navFile;
  NavFileWPListHeader testArrayIn[arrayLength];
  NavFileWPListHeader testArrayOut[arrayLength];

  Coordinate testCoordA;
  Coordinate testCoordB;
  initCoordinate(&testCoordA);
  initCoordinate(&testCoordB);

  printTestHeader(testName);

  size_t i = 0;
  for (i = 0; i < arrayLength; i++)
  {
    initNavFileWPListHeader(&testArrayIn[i]);
    initNavFileWPListHeader(&testArrayOut[i]);
    // Fill structure with sensible data
    testCoordA.dLatitude = i;
    testCoordA.dLongitude = i + 2;
    testCoordA.mLatitude = i + 5;
    testCoordA.mLatitude = i + 7;
    testCoordA.priority = i + 9;
    testCoordB.dLatitude = (2 * i);
    testCoordB.dLongitude = (2 * i) + 3;
    testCoordB.mLatitude = (2 * i) + 6;
    testCoordB.mLatitude = (2 * i) + 9;
    testCoordB.priority = (2 * i) + 10;

    testArrayIn[i].startCoordinate = testCoordA;
    testArrayIn[i].endCoordinate = testCoordB;
    testArrayIn[i].numberOfEntries = (i * i * i) + 45;
    testArrayIn[i].nextHeaderSize = (i * i * i * i) + 77;
  }

  navFile = NAV_fopen(testFileName, "wb");

  for (i = 0; i < arrayLength; i++)
  {
    bytesWritten += fwriteNavFileWPListHeader(&testArrayIn[i], navFile);
  }

  NAV_fclose(navFile);
  navFile = NULL;
  navFile = NAV_fopen(testFileName, "rb");

  for (i = 0; i < arrayLength; i++)
  {
    bytesRead += freadNavFileWPListHeader(&testArrayOut[i], navFile);
  }

  for (i = 0; i < arrayLength; i++)
  {
    if ((compNavFileWPListHeader(&testArrayIn[i], &testArrayOut[i])) == 0)
    {
      testPassed = 0;
    }
  }

  if (bytesWritten != bytesRead)
  {
    testPassed = 0;
    NAV_printf("read/write size different.\r\n");
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

void testfNavDatablockHeader()
{
  uint8_t testPassed = 1;
  size_t bytesWritten = 0;
  size_t bytesRead = 0;
  const char testName[64] = "testfNavDatablockHeader()";
  const int arrayLength = 5;
  const char testFileName[20] = "blkhdr.tst";
  NAV_FILE* navFile;
  NavDatablockHeader testArrayIn[arrayLength];
  NavDatablockHeader testArrayOut[arrayLength];

  printTestHeader(testName);

  size_t i = 0;
  for (i = 0; i < arrayLength; i++)
  {
    initNavDatablockHeader(&testArrayIn[i]);
    initNavDatablockHeader(&testArrayOut[i]);
    // Fill array with numbers that are sensible and different
    testArrayIn[i].dataType = i;
    testArrayIn[i].dataVersion = i + 4;
    testArrayIn[i].nextDataSize = i + 9;
  }

  navFile = NAV_fopen(testFileName, "wb");

  for (i = 0; i < arrayLength; i++)
  {
    bytesWritten += fwriteNavDatablockHeader(&testArrayIn[i], navFile);
  }

  NAV_fclose(navFile);
  navFile = NULL;
  navFile = NAV_fopen(testFileName, "rb");

  for (i = 0; i < arrayLength; i++)
  {
    bytesRead += freadNavDatablockHeader(&testArrayOut[i], navFile);
  }

  for (i = 0; i < arrayLength; i++)
  {
    if ((compNavDatablockHeader(&testArrayIn[i], &testArrayOut[i])) == 0)
    {
      testPassed = 0;
    }
  }

  if (bytesWritten != bytesRead)
  {
    testPassed = 0;
    NAV_printf("read/write size different.");
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

#endif // NAVFILESTESTS_H

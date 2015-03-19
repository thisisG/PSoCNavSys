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
  size_t bytesWritten = 0;
  size_t bytesRead = 0;
  const char testName[64] = "testWPHandler()";
  const int arrayLength = 5;
  const char testFileName[20] = "wphndlr.tst";
  NAV_FILE* navFile;

  NavWPHandler testWPHandler;
  initNavWPHandler(&testWPHandler);

  NavDatablockHeader navDataHdrArrayIn[arrayLength];
  NavDatablockHeader navDataHdrArrayOut[arrayLength];

  Coordinate coordIn[arrayLength];
  Coordinate coordOut[arrayLength];



  printTestHeader(testName);

  for (size_t i = 0; i < arrayLength; i++)
  {
    initNavDatablockHeader(&testArrayIn[i]);
    initNavDatablockHeader(&testArrayOut[i]);
    // Fill array with numbers that are sensible and different
    testArrayIn[i] = { i, i + 4, i + 9 };
  }

  navFile = NAV_fopen(testFileName, "wb");

  for (size_t i = 0; i < arrayLength; i++)
  {
    bytesWritten += fwriteNavDatablockHeader(&testArrayIn[i], navFile);
  }

  NAV_fclose(navFile);
  navFile = NULL;
  navFile = NAV_fopen(testFileName, "rb");

  for (size_t i = 0; i < arrayLength; i++)
  {
    bytesRead += freadNavDatablockHeader(&testArrayOut[i], navFile);
  }

  for (size_t i = 0; i < arrayLength; i++)
  {
    if ((compNavDatablockHeader(&testArrayIn[i], &testArrayOut[i])) == 0)
    {
      testPassed = 0;
    }
  }

  if (bytesWritten != bytesRead)
  {
    testPassed = 0;
    NAV_fprint("read/write size different.");
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

}


#endif // WPHANDLERTESTS_H

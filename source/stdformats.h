// Function descriptions for header files
/* functionName();
input:
output:
remarks:
*/

// Header file separator for sections
/***********************************************
**
**
**
***********************************************/

// Copyright notice for top of header/source files
/*
**
** NavSys library
** URL: https://github.com/thisisG
** Author: Geir Istad (geir dot istad at gmail dot com)
** Licence: MIT
**
*/

// GNU ifdef / else block
#ifdef __GNUC__

#else

#endif // __GNUC__

// Test template
void testFunction()
{
  uint8_t testPassed = 1;
  const char testName[64] = "testName()";
  printTestHeader(testName);

  const int arrayLength = 5;
  Coordinate coordArrayIn[arrayLength];
  for (size_t i = 0; i < arrayLength; i++)
  {
    zeroCoordinate(&coordArrayIn[i]);
    // Fill array with numbers that are sensible
    coordArrayIn[i] = { i, i, i, i, i };
  }

  NAV_FILE *navFile;
  const char coordFile[20] = "coord.tst";

  navFile = NAV_fopen(coordFile, "wb");

  for (size_t i = 0; i < arrayLength; i++)
  {
    fwriteCoordinate(&coordArrayIn[i], navFile);
  }

  NAV_fclose(navFile);
  navFile = NULL;

  navFile = NAV_fopen(coordFile, "rb");

  Coordinate coordArrayOut[arrayLength];

  for (size_t i = 0; i < arrayLength; i++)
  {
    zeroCoordinate(&coordArrayOut[i]);
    freadCoordinate(&coordArrayOut[i], navFile);
  }

  for (size_t i = 0; i < arrayLength; i++)
  {
    if (coordsEqual(&coordArrayIn[i], &coordArrayOut[i]);)
    {
      testPassed = 0;
    }
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

// File test template
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

#endif // NAVFILESTESTS_H

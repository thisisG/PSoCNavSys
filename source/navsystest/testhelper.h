/*
**
** NavSys library
** URL: https://github.com/thisisG
** Author: Geir Istad (geir dot istad at gmail dot com)
** Licence: MIT
**
*/

#ifndef TESTHELPER_H
#define TESTHELPER_H

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

#include <stdio.h>
#include ".\..\navsys\navfunctions.h"

#ifdef __cplusplus
}
#endif // __cplusplus

void NAV_printf(const char* message)
{
// GNU ifdef / else block
#ifdef __GNUC__
  UART_GPS_PutString(message);
#else
  printf(message);
#endif // __GNUC__
}
void printStars()
{
  const char stars[20] = "****************\r\n";
  NAV_printf(stars);
}

void printTestHeader(const char* message)
{
  printStars();
  NAV_printf("Running: ");
  NAV_printf(message);
  NAV_printf("\r\n");
  printStars();
}

void printFailed(const char* message)
{
  printStars();
  NAV_printf(message);
  NAV_printf(" failed!");
  NAV_printf("\r\n");
  printStars();
}

void printPassed(const char* message)
{
  printStars();
  NAV_printf(message);
  NAV_printf(" passed!");
  NAV_printf("\r\n");
  printStars();
}

uint8_t compNavFileHeader(const struct NavFileHeader* headerA,
                          const struct NavFileHeader* headerB)
{
  uint8_t returnValue = 1;

  // Evaluate each parameter. The use of else if() allows premature
  // termination in case an inequality is found.
  if ((headerA->fileType) != (headerB->fileType))
  {
    returnValue = 0;
  }
  else if ((headerA->fileVersion) != (headerB->fileVersion))
  {
    returnValue = 0;
  }
  else if ((headerA->nextHeaderSize) != (headerB->nextHeaderSize))
  {
    returnValue = 0;
  }
  else
  {
    // Do nothing, case here for completion.
  }

  return returnValue;
}

uint8_t compNavFileWPListHeader(const struct NavFileWPListHeader* headerA,
                                const struct NavFileWPListHeader* headerB)
{
  uint8_t returnValue = 1;

  // Evaluate each parameter. The use of else if() allows premature
  // termination in case an inequality is found.
  if (coordsEqual(&(headerA->startCoordinate), &(headerB->startCoordinate))
      == 0)
  {
    returnValue = 0;
  }
  else if (coordsEqual(&(headerA->endCoordinate), &(headerB->endCoordinate))
           == 0)
  {
    returnValue = 0;
  }
  else if ((headerA->numberOfEntries) != (headerB->numberOfEntries))
  {
    returnValue = 0;
  }
  else if ((headerA->nextHeaderSize) != (headerB->nextHeaderSize))
  {
    returnValue = 0;
  }
  else
  {
    // Do nothing, case here for completion.
  }

  return returnValue;
}

uint8_t compNavDatablockHeader(const struct NavDatablockHeader* headerA,
  const struct NavDatablockHeader* headerB)
{
  uint8_t returnValue = 1;

  // Evaluate each parameter. The use of else if() allows premature
  // termination in case an inequality is found.
  if ((headerA->dataType) != (headerB->dataType))
  {
    returnValue = 0;
  }
  else if ((headerA->dataVersion) != (headerB->dataVersion))
  {
    returnValue = 0;
  }
  else if ((headerA->nextDataSize) != (headerB->nextDataSize))
  {
    returnValue = 0;
  }
  else
  {
    // Do nothing, case here for completion.
  }

  return returnValue;
}
#endif // TESTHELPER_H

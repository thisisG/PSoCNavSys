/*
**
** NavSys library
** URL: https://github.com/thisisG
** Author: Geir Istad (geir dot istad at gmail dot com)
** Licence: MIT
**
*/

#ifndef MAVMATHTESTS_H
#define MAVMATHTESTS_H

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

#include "./../navsys/navmath.h"
#include "testhelper.h"
#include <math.h>

#ifdef __cplusplus
}
#endif // __cplusplus

void testNavMath()
{
  uint8_t testPassed = 1;
  const int arrayLength = 10;
  const char testName[64] = "testNavMath()";
  floatDegree floatArrayIn[arrayLength];
  floatDegree floatArrayOut[arrayLength];
  signed16Degree degrees[arrayLength];
  signed32Degree minutes[arrayLength];
  floatDegree haversineResults[arrayLength];
  floatDegree inverseHaversineResults[arrayLength];
  size_t i = 0;
  floatDegree tempFloat = -180.0f;
  floatDegree deltaDegree = 44.46235f;
  floatDegree errorLimit = 0.0001f;

  printTestHeader(testName);

  // Fill the floatArrayIn with values from -180 to 180
  // Start and end case manually
  floatArrayIn[0] = -180.0;
  floatArrayIn[arrayLength - 1] = 180.0;
  // Middle cases with decimals
  for (i = 1; i < arrayLength - 1; i++)
  {
    tempFloat += deltaDegree;
    floatArrayIn[i] = tempFloat;
  }

  // Convert all numbers in floatArrayIn to degrees/minutes pairs and back in to
  // floatArrayOut
  for (i = 0; i < arrayLength; i++)
  {
    longDegreeFromFloatDegree(floatArrayIn[i], &degrees[i], &minutes[i]);
    floatArrayOut[i] = floatFromLongDegree(degrees[i], minutes[i]);
  }

  // Check if the results are within error limits
  for (i = 0; i < arrayLength; i++)
  {
    if (fabs(floatArrayIn[i] - floatArrayOut[i]) > errorLimit)
    {
      testPassed = 0;
      NAV_printf("floatArrayIn - floatArrayOut > errorLimit!\r\n");
    }
  }

  // Calculate the haversine and inverse haversine of each floatArrayIn
  for (i = 0; i < arrayLength; i++)
  {
    haversineResults[i] = rHaversine(toRadian(floatArrayIn[i]));
    inverseHaversineResults[i]
        = toDegree(rInverseHaversine(haversineResults[i]));
  }

  // Check if the results are within error limits, note that the value returned
  // by the inverse haversine is unsigned so using the absolute of the input
  // value as reference
  for (i = 0; i < arrayLength; i++)
  {
    if (fabs(fabs(floatArrayIn[i]) - inverseHaversineResults[i]) > errorLimit)
    {
      testPassed = 0;
      NAV_printf("floatArrayIn - inverseHaversineResults > errorLimit!\r\n");
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

#endif // MAVMATHTESTS_H

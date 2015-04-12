/*
**
** NavSys library
** URL: https://github.com/thisisG
** Author: Geir Istad (geir dot istad at gmail dot com)
** Licence: MIT
**
*/

#pragma once

#include <iostream>
#include <vector>
#include <math.h>
#include <random>
#include "./../csv/csv.h"

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

#include "./../nmea/nmea.h"
#include "./../navsys/navtypes.h"
#include "./../navsys/navfunctions.h"

#ifdef __cplusplus
}
#endif // __cplusplus

class GpsEmulator
{
  public:
  GpsEmulator();
  ~GpsEmulator();
  void parseCsvToInfoVector(const std::string& csvFile);
  void generatePseudoRandomData();
  int32_t getNextStringToCharBuffer(char* buffer, int bufferSize);
  int32_t getNextStringToNavState(NavState* navS);

  private:
  int nmeaVectorLength = 0;
  std::vector<nmeaINFO> nmeaInfoVector;
  std::default_random_engine* genSpeed;
  std::uniform_real_distribution<double>* distSpeed;
  std::default_random_engine* genHeading;
  std::uniform_real_distribution<double>* distHeading;

  void csvDataToNmeaInfo(const float dLat, const float dLong,
                         const float dHeading, nmeaINFO* nmeaBuffer);
  void initRandomGenerators();
  void cleanupRandomGenerators();
  double generateSpeed();
  double generateHeading();
};

/*
**
** NavSys library
** URL: https://github.com/thisisG
** Author: Geir Istad (geir dot istad at gmail dot com)
** Licence: MIT
**
*/

#include "gpsemulator.h"

GpsEmulator::GpsEmulator() {}

GpsEmulator::~GpsEmulator()
{
  // May have a memory leak here due to not deleting the vector and rnd
  // generators. Fix later!
}

void GpsEmulator::parseCsvToInfoVector(const std::string& csvFile)
{
  io::CSVReader<9> in(csvFile);

  // Variables for parsing
  std::string csvType, csvName, csvDesc;
  float csvLat = 0, csvLon = 0, csvAlt = 0, csvCourse = 0, csvTotDistKm = 0,
        csvIntervalDistM = 0;
  nmeaINFO nmeaBuffer;
  nmea_zero_INFO(&nmeaBuffer);

  while (in.read_row(csvType, csvLat, csvLon, csvAlt, csvCourse, csvTotDistKm,
                     csvIntervalDistM, csvName, csvDesc))
  {
    csvDataToNmeaInfo(csvLat, csvLon, csvCourse, &nmeaBuffer);
    nmeaInfoVector.push_back(nmeaBuffer);
  }
  nmeaVectorLength = nmeaInfoVector.size();
}

void GpsEmulator::generatePseudoRandomData()
{
  if (nmeaVectorLength > 0)
  {
    initRandomGenerators();
    nmeaInfoVector[0].speed = 1; // Set start speed to 1 km/h
    for (int i = 1; i < nmeaVectorLength; i++)
    {
      nmeaInfoVector[i].speed = generateSpeed();
      nmeaInfoVector[i].direction = generateHeading();
    }
    cleanupRandomGenerators();
  }
}

int32_t GpsEmulator::getNextStringToCharBuffer(char* buffer, int bufferSize)
{
  static int strCounter = 0;
  int32_t returnValue = 0;
  int gpsStringLength = nmea_generate(&buffer[0], bufferSize,
                                      &nmeaInfoVector[strCounter], GPRMC);
  buffer[gpsStringLength] = 0;
  if (strCounter < nmeaVectorLength - 1)
  {
    ++strCounter;
    returnValue = strCounter;
  }
  else
  {
    // Change returnvalue to indicate no more strings
    returnValue = -1;
  }
  return returnValue;
}

int32_t GpsEmulator::getNextStringToNavState(NavState* navS)
{
  navS->gpsBuffer.newGPSString = 1;
  return getNextStringToCharBuffer(navS->gpsBuffer.gpsStringBuffer,
                            navS->gpsBuffer.gpsBufferLength);
}

void GpsEmulator::csvDataToNmeaInfo(const float dLat, const float dLong,
                                    const float dHeading, nmeaINFO* nmeaBuffer)
{
  nmeaBuffer->lat = nmea_degree2ndeg(dLat);
  nmeaBuffer->lon = nmea_degree2ndeg(dLong);
  nmeaBuffer->direction = dHeading;
}

void GpsEmulator::initRandomGenerators()
{
  unsigned randomSeed = 1337;
  genSpeed = new std::default_random_engine(randomSeed);
  distSpeed = new std::uniform_real_distribution<double>(0.0, 1.85);
  genHeading = new std::default_random_engine(randomSeed);
  distHeading = new std::uniform_real_distribution<double>(0.0, 360.0);
}

void GpsEmulator::cleanupRandomGenerators()
{
  delete genSpeed;
  genSpeed = nullptr;
  delete distSpeed;
  distSpeed = nullptr;
  delete genHeading;
  genHeading = nullptr;
  delete distHeading;
  distHeading = nullptr;
}

double GpsEmulator::generateSpeed()
{
  double speed = (*distSpeed)(*genSpeed);
  return speed;
}

double GpsEmulator::generateHeading()
{
  double heading = (*distHeading)(*genHeading);
  return heading;
}

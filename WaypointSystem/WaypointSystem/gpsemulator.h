#pragma once

#include <iostream>
#include <vector>
#include <math.h>
#include <random>
#include "csv.h"

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

#include "nmea\nmea.h"
#include "navtypes.h"

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
    void getNextStringToCharBuffer(char* buffer, int bufferSize);
    void getNextStringToNavState(NavState* navS);

private:
    int nmeaVectorLength = 0;
    std::vector<nmeaINFO> nmeaInfoVector;
    std::default_random_engine* genSpeed;
    std::uniform_real_distribution<double>* distSpeed;
    std::default_random_engine* genHeading;
    std::uniform_real_distribution<double>* distHeading;

    void csvDataToNmeaInfo(const float dLat, const float dLong, const float dHeading,
                           nmeaINFO* nmeaBuffer);
    void initRandomGenerators();
    void cleanupRandomGenerators();
    double generateSpeed();
    double generateHeading();
};

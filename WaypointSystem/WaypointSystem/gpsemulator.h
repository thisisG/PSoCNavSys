#pragma once

#include <iostream>
#include <vector>
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
    void getNextStringToCharBuffer(char* buffer, int bufferSize);
    void getNextStringToNavState(NavState* navS);
    void parseCsvToInfoVector(const std::string& csvFile);

private:
    int nmeaVectorLength = 0;
    std::vector<nmeaINFO> nmeaInfoVector;
    void csvDataToNmeaInfo(
        const float dLat, const float dLong, const float dHeading, nmeaINFO* nmeaBuffer);
};

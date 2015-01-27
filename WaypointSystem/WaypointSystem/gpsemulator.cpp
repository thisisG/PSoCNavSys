#include "gpsemulator.h"

GpsEmulator::GpsEmulator() {}

GpsEmulator::~GpsEmulator()
{
    // May have a memory leak here due to not deleting the vector. Fix later!
}

void GpsEmulator::parseCsvToInfoVector(const std::string& csvFile)
{
    io::CSVReader<9> in(csvFile);

    // Variables for parsing
    std::string csvType, csvName, csvDesc;
    float csvLat = 0, csvLon = 0, csvAlt = 0, csvCourse = 0, csvTotDistKm = 0, csvIntervalDistM = 0;
    nmeaINFO nmeaBuffer;
    nmea_zero_INFO(&nmeaBuffer);

    while (in.read_row(csvType, csvLat, csvLon, csvAlt, csvCourse, csvTotDistKm, csvIntervalDistM,
        csvName, csvDesc))
    {
        csvDataToNmeaInfo(csvLat, csvLon, csvCourse, &nmeaBuffer);
        nmeaInfoVector.push_back(nmeaBuffer);
    }
    nmeaVectorLength = nmeaInfoVector.size();
}

void GpsEmulator::getNextString(char* buffer, int bufferSize)
{
    static int strCounter = 0;
    int gpsStringLength = nmea_generate(&buffer[0], bufferSize, &nmeaInfoVector[strCounter], GPRMC);
    buffer[gpsStringLength] = 0;
    if (strCounter < nmeaVectorLength-1)
    {
        ++strCounter;
    }
    else
    {
        // Do not increment counter since we are at the final point
    }
}

void GpsEmulator::csvDataToNmeaInfo(
    const float dLat, const float dLong, const float dHeading, nmeaINFO* nmeaBuffer)
{
    nmeaBuffer->lat = nmea_degree2ndeg(dLat);
    nmeaBuffer->lon = nmea_degree2ndeg(dLong);
    nmeaBuffer->direction = dHeading;
}
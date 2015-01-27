#ifdef __cplusplus
extern "C" {
#endif
#include "gpsinterface.h"
#ifdef __cplusplus
}
#endif

#ifdef _WIN32

#endif // _WIN32

void gpsStringToNavState(
    const char* gpsString, const int gpsStringSize, nmeaPACKTYPE pack, NavState* navS)
{
    if (pack == GPRMC)
    {
        nmeaGPRMC gprmcBuffer;
        nmea_zero_GPRMC(&gprmcBuffer);
        nmea_parse_GPRMC(&gpsString[0], gpsStringSize, &gprmcBuffer);

        nmeaINFO nmeaBuffer;
        nmea_zero_INFO(&nmeaBuffer);
        nmea_GPRMC2info(&gprmcBuffer, &nmeaBuffer);

        nmeaInfoToCoord(&nmeaBuffer, &(navS->currentLocation));

        navS->dCurrentHeading = (floatDegree)nmeaBuffer.direction;
    }
    else
    {
        return;
    }
}
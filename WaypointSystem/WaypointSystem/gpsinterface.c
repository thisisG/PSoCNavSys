#ifdef __cplusplus
extern "C" {
#endif
#include "gpsinterface.h"
#ifdef __cplusplus
}
#endif

#ifdef _WIN32

#endif // _WIN32

void gpsStringToNavState(NavState* navS)
{
    // Need to pass the address to the second char value since the 0th is occupied by '$'
    int pack = nmea_pack_type(&(navS->gpsBuffer.gpsBuffer)[1], navS->gpsBuffer.gpsBufferLength);
    // DEBUG
    // printf("pack number: %d", pack);
    if (pack == GPRMC)
    {
        nmeaGPRMC gprmcBuffer;
        nmea_zero_GPRMC(&gprmcBuffer);
        nmea_parse_GPRMC((navS->gpsBuffer.gpsBuffer), navS->gpsBuffer.gpsBufferLength,
                         &gprmcBuffer);

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
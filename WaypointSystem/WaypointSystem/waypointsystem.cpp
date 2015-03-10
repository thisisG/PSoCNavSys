
#include <iostream>
#include <iomanip>

#ifdef __cplusplus
extern "C" {
#endif
#include <stdio.h>

#include "nmea/nmea.h"
#include "nmea/config.h"
#include "navsys/navtypes.h"
#include "navsys/navfunctions.h"
#include "navsys/nmeafunctions.h"
#include "navsys/gpsinterface.h"
#include "navsys/serialcom.h"
#include "navsys/navmath.h"

#ifdef __cplusplus
}
#endif

#include "csv/csv.h"
#include "navmanager/gpsemulator.h"

void csvDataToCoord(const float csvLat, const float csvLon, Coordinate& coord)
{
    floatDegree tempFloatDeg = 0;
    signed16Degree tempSig16Deg = 0;
    signed32Degree tempSig32Deg = 0;

    tempSig16Deg = (signed16Degree)csvLat;
    coord.dLatitude = tempSig16Deg;
    tempFloatDeg = csvLat - tempSig16Deg;
    tempSig32Deg = (signed32Degree)(tempFloatDeg * 600000);
    coord.mLatitude = tempSig32Deg;

    tempSig16Deg = (signed16Degree)csvLon;
    coord.dLongitude = tempSig16Deg;
    tempFloatDeg = csvLon - tempSig16Deg;
    tempSig32Deg = (signed32Degree)(tempFloatDeg * 600000);
    coord.mLongitude = tempSig32Deg;
}

void coordToNmeaInfo(Coordinate& i_coord, nmeaINFO& o_nmeaInfo)
{
    nmea_zero_INFO(&o_nmeaInfo);

    o_nmeaInfo.lat = nmeaLatitudeFromCoord(&i_coord);
    o_nmeaInfo.lon = nmeaLongitudeFromCoord(&i_coord);
    o_nmeaInfo.sig = 3;
    o_nmeaInfo.fix = 3;
    o_nmeaInfo.speed = 1 * NMEA_TUS_MS;
    o_nmeaInfo.elv = 4.7;
    o_nmeaInfo.satinfo.inuse = 1;
    o_nmeaInfo.satinfo.inview = 1;
}

int main()
{
    /*
    Test for floatFromLongDegree()
    */
    // First create a sequence of bits of known value and compare the calculated
    // value to hand calculated values
    // Define the degrees we want to test with

    // Use degA = 32(000000100000), degB = -33
    // Set the degrees, one negative and one positive. Should use a signed 8 bit
    // integer
    signed16Degree degA = 32;
    signed16Degree degB = -33;

    // Define the minutes we want to test with
    signed32Degree minA = 573688;
    signed32Degree minB = 239534;
    floatDegree floatingDegreesA = floatFromLongDegree(degA, minA);
    printf("floatFromLongDegree() = %6f\n", floatingDegreesA);

    floatDegree floatingDegreesB = floatFromLongDegree(degB, minB);
    printf("floatFromLongDegree() = %6f\n", floatingDegreesB);

    // Load the set into coordinate ADS
    struct Coordinate coordA, coordB;
    zeroCoordinate(&coordA);
    zeroCoordinate(&coordB);

    coordA.dLatitude = degA;
    coordA.mLatitude = minA;
    coordA.dLongitude = degB;
    coordA.mLongitude = minB;

    floatingDegreesA = latitudeFromCoordinate(&coordA);
    printf("latitudeFromCoordinate(&coordA) = %6f\n", floatingDegreesA);

    floatingDegreesB = longitudeFromCoordinate(&coordA);
    printf("longitudeFromCoordinate(&coordA) = %6f\n", floatingDegreesB);

    // Test if coordinates work in an array
    struct Coordinate coordArray[100];

    for (int arrayIndex = 0; arrayIndex < 100; arrayIndex++)
    {
        coordArray[arrayIndex].dLatitude = degA;
        coordArray[arrayIndex].mLatitude = minA;
        coordArray[arrayIndex].dLongitude = degB;
        coordArray[arrayIndex].mLongitude = minB;
    }

    // Check a few random ones if the correct value is stored
    printf("latitudeFromCoordinate(&coordArray[0]) = %6f\n",
           latitudeFromCoordinate(&coordArray[0]));
    printf("latitudeFromCoordinate(&coordArray[65]) = %6f\n",
           latitudeFromCoordinate(&coordArray[65]));
    printf("latitudeFromCoordinate(&coordArray[99]) = %6f\n",
           latitudeFromCoordinate(&coordArray[99]));

    std::cout << std::endl;

    /*
    Test the toDegree() and toRadian() functions
    */
    floatDegree inDegA = 213.7453f;
    floatDegree inRadA = toRadian(inDegA);
    printf("%f in deg = %f in rad\n", inDegA, inRadA);

    floatDegree inDegB = 180.0f;
    floatDegree inRadB = toRadian(inDegB);
    printf("%f in deg = %f in rad\n", inDegB, inRadB);

    floatDegree inDegC = 0.0f;
    floatDegree inRadC = toRadian(inDegC);
    printf("%f in deg = %f in rad\n", inDegC, inRadC);

    floatDegree inDegD = 360.0f;
    floatDegree inRadD = toRadian(inDegD);
    printf("%f in deg = %f in rad\n", inDegD, inRadD);

    std::cout << std::endl;

    /*
    Test great circle path function
    */
    // CoordA should be lat 36d37'26.65"N, lon 33d38'6.48"W
    // Expecting a length around 1750512 m for this set
    // dlatA = 36, mlatA = 37.4442 or dlatA = 36.62407
    // dlonA = -33, mlonA = 38.1080 or dlongA = -33.63513
    coordA.dLatitude = 36;
    coordA.mLatitude = 374442;
    coordA.dLongitude = -33;
    coordA.mLongitude = 381080;

    // CoordB should be lat 23d56'25.70"N, lon 44d31'18.55"W
    // dlatB = 23, mlatA = 56.4283 or dlatB = 23.94047167
    // dlonB = -44, mlonA = 31.3091 or dlongB = -44.5218183333
    coordB.dLatitude = 23;
    coordB.mLatitude = 564283;
    coordB.dLongitude = -44;
    coordB.mLongitude = 313091;

    float distanceAB = distanceCirclePathAtoB(&coordA, &coordB);
    printf("haversine distance from coordA to coordB is %f\n", distanceAB);

    std::cout << std::endl;

    /*
    Test law of spherical cosines function
    */
    distanceAB = distanceSphereCosineAtoB(&coordA, &coordB);
    printf("spherical cosine distance from coordA to coordB is %f\n",
           distanceAB);

    std::cout << std::endl;

    /*
    Test equirectangular approximation function
    */
    distanceAB = distanceEquiRectAtoB(&coordA, &coordB);
    printf(
        "equirectangular approximation distance from coordA to coordB is %f\n",
        distanceAB);

    std::cout << std::endl;

    /*
    Test the creation of a NavState and its initialization
    */
    struct NavState myNavState;
    zeroNavState(&myNavState);
    printCurrentCoordAndHeading(&myNavState);

    myNavState.currentLocation = coordA;
    std::cout << "New latitude in myNavState.currentLocation: "
              << myNavState.currentLocation.dLatitude << std::endl;

    std::cout << std::endl;

    /*
    Test CSV reader with csv.h in exe directory
    */
    io::CSVReader<9> in("./../TestData/csvtestA.csv");
    std::cout << "Loaded file" << std::endl;

    // Variables to contain read data and count
    std::string csvType, csvName, csvDesc;
    int csvLineCount = 0, wpCount = 0, wpDivisor = 10;
    float csvLat = 0, csvLon = 0, csvAlt = 0, csvCourse = 0, csvTotDistKm = 0,
          csvIntervalDistM = 0;

    // Create array to hold all coordinates and a buffer structure
    std::vector<Coordinate> coordVector;
    std::vector<Coordinate> wpVector;
    Coordinate csvCoordBuffer;
    // Check if the thing is empty
    std::cout << "Size of coordVector at creation: " << int(coordVector.size())
              << std::endl;

    while (in.read_row(csvType, csvLat, csvLon, csvAlt, csvCourse, csvTotDistKm,
                       csvIntervalDistM, csvName, csvDesc))
    {
        csvDataToCoord(csvLat, csvLon, csvCoordBuffer);

        if (csvLineCount == 0)
        {
            std::cout << "First coord lat/lon: " << csvCoordBuffer.dLatitude
                      << "d, " << csvCoordBuffer.mLatitude << "m/"
                      << csvCoordBuffer.dLongitude << "d, "
                      << csvCoordBuffer.mLongitude << "m" << std::endl;
        }

        if (csvLineCount % wpDivisor == 0)
        {
            ++wpCount;
            wpVector.push_back(csvCoordBuffer);
        }

        ++csvLineCount;
        coordVector.push_back(csvCoordBuffer);
    }

    // How many csv entries did we have?
    std::cout << "Number of entries in csv file: " << csvLineCount << std::endl;
    // How many WP do we have?
    std::cout << "Number of wps: " << wpCount << std::endl;
    // Check if the coordVector is filled and equal to entries
    std::cout << "Size of coordVector at end: " << int(coordVector.size())
              << std::endl;
    // Check if the wpVector is filled and equal to wps
    std::cout << "Size of wpVector at end: " << int(wpVector.size())
              << std::endl;

    // Check if the coordVector contains data
    std::cout << "Latitude for first coordVector entry: "
              << coordVector.front().dLatitude << std::endl;
    std::cout << "Latitude for last coordVector entry: "
              << coordVector.back().dLatitude << std::endl;

    // Check if the wpVector contains data
    std::cout << "Latitude for first wpVector entry: "
              << wpVector.front().dLatitude << std::endl;
    std::cout << "Latitude for last wpVector entry: "
              << wpVector.back().dLatitude << std::endl;

    // Check if the last WP is same as last coord, if not increment wpCount and
    // add last coord as an extra WP
    if (wpVector.back().dLatitude != coordVector.back().dLatitude)
    {
        ++wpCount;
        wpVector.push_back(coordVector.back());
    }

    // Check that last value for vectors is the same
    std::cout << "Latitude for last coordVector entry: "
              << coordVector.back().dLatitude << std::endl;
    std::cout << "Latitude for last wpVector entry: "
              << wpVector.back().dLatitude << std::endl;

    // How many WP do we have?
    std::cout << "Number of wps updated: " << wpCount << std::endl;
    // Check if the wpVector is filled and equal to wps
    std::cout << "Size of wpVector at end updated: " << int(wpVector.size())
              << std::endl;
    std::cout << std::endl;

    /*
        Test Coord to GPS string function
        */
    const int gpsStringBufferSize = 1024;
    char gpsStringBuffer[gpsStringBufferSize] = "";
    // Check that string is empty
    std::cout << "empty gpsString: " << gpsStringBuffer << std::endl;

    // Create structures for testing
    nmeaINFO nmeaBuffer;
    nmea_zero_INFO(&nmeaBuffer);
    Coordinate coordBuffer = coordVector[0];

    // Load test data into NMEA buffer
    coordToNmeaInfo(coordBuffer, nmeaBuffer);

    // Generate a string from buffer
    int gpsStringLength = 0;

    gpsStringLength = nmea_generate(&gpsStringBuffer[0], gpsStringBufferSize,
                                    &nmeaBuffer, GPGGA);

    gpsStringBuffer[gpsStringLength] = 0;
    std::cout << "Test GPS strings: \n" << gpsStringBuffer;

    // Compare to data stored in coordBuffer
    std::cout << "Latitude for coordBuffer: " << coordBuffer.dLatitude << "d"
              << coordBuffer.mLatitude << "m" << std::endl;
    std::cout << "Longitude for coordBuffer: " << coordBuffer.dLongitude << "d"
              << coordBuffer.mLongitude << "m" << std::endl;
    std::cout << "Longitude for coordBuffer in float: "
              << nmeaFloatFromLongDegree(coordBuffer.dLongitude,
                                         coordBuffer.mLongitude) << std::endl;
    std::cout << std::endl;

    /*
    Test GPS string to Coord conversion
    */
    nmea_zero_INFO(&nmeaBuffer);
    nmeaGPGGA GPGGAbuffer;
    nmea_zero_GPGGA(&GPGGAbuffer);

    nmea_parse_GPGGA(&gpsStringBuffer[0], gpsStringBufferSize, &GPGGAbuffer);
    nmea_GPGGA2info(&GPGGAbuffer, &nmeaBuffer);

    std::cout << "nmeaBuffer.lat: " << std::setprecision(8) << nmeaBuffer.lat
              << std::endl;
    std::cout << "nmeaBuffer.lon: " << std::setprecision(8) << nmeaBuffer.lon
              << std::endl;

    // Get latitude in longDegree format from nmeaBuffer
    signed16Degree dlatitudeBuffer = 0;
    signed32Degree mlatitudeBuffer = 0;
    signed16Degree dlongitudeBuffer = 0;
    signed32Degree mlongitudeBuffer = 0;

    longLatitudeFromNmeaInfo(&nmeaBuffer, &dlatitudeBuffer, &mlatitudeBuffer);
    std::cout << "lat from buff: " << dlatitudeBuffer << "d" << mlatitudeBuffer
              << "m" << std::endl;
    longLongitudeFromNmeaInfo(&nmeaBuffer, &dlongitudeBuffer,
                              &mlongitudeBuffer);
    std::cout << "lon from buff: " << dlongitudeBuffer << "d"
              << mlongitudeBuffer << "m" << std::endl;

    // Zero coordinate buffer and load nmea info to it
    zeroCoordinate(&coordBuffer);
    printCoordData(&coordBuffer);
    nmeaInfoToCoord(&nmeaBuffer, &coordBuffer);
    printCoordData(&coordBuffer);
    std::cout << std::endl;

    /*
    Set current position in NavState from GPS string
    For this we're interested in the GPRMC string string containing much of the
    same data as GPGGA but also provides current speed and whatnot. This is the
    string that we want to use the most, and if need be the only string to use
    if
    we can only use one!
    */
    nmeaGPRMC gprmcBuffer;
    nmea_zero_GPRMC(&gprmcBuffer);
    nmea_info2GPRMC(&nmeaBuffer, &gprmcBuffer);

    gpsStringLength = nmea_generate(&gpsStringBuffer[0], gpsStringBufferSize,
                                    &nmeaBuffer, GPRMC);

    gpsStringBuffer[gpsStringLength] = 0;
    std::cout << "Test GPS strings: \n" << gpsStringBuffer;

    // The string lacks heading. Not great! Create a mock heading.
    nmeaBuffer.direction = 214.3;
    nmea_info2GPRMC(&nmeaBuffer, &gprmcBuffer);

    gpsStringLength = nmea_generate(&gpsStringBuffer[0], gpsStringBufferSize,
                                    &nmeaBuffer, GPRMC);

    gpsStringBuffer[gpsStringLength] = 0;
    std::cout << "Test GPS string: \n" << gpsStringBuffer;
    std::cout << std::endl;

    /*
    Testing GpsEmulator
    */
    GpsEmulator gpsEmu;
    gpsEmu.parseCsvToInfoVector("./../TestData/csvtestA.csv");
    gpsEmu.generatePseudoRandomData();

    gpsEmu.getNextStringToNavState(&myNavState);
    std::cout << "Test GPS taken from testing interface: \n"
              << myNavState.gpsBuffer.gpsStringBuffer;
    std::cout << std::endl;

    for (size_t i = 0; i < 3; i++)
    {
        gpsEmu.getNextStringToNavState(&myNavState);
        std::cout << "String: \n" << myNavState.gpsBuffer.gpsStringBuffer;
        decodeGpsStringInNavState(&myNavState);
        printCurrentCoordAndHeading(&myNavState);
        std::cout << std::endl;
    }

    /*
    Testing navDataToSerialBufferThing
    */

    for (size_t j = 0; j < 3; j++)
    {
        gpsEmu.getNextStringToNavState(&myNavState);
        std::cout << "String: \n" << myNavState.gpsBuffer.gpsStringBuffer;
        decodeGpsStringInNavState(&myNavState);
        printCurrentCoordAndHeading(&myNavState);
        navDataToSerialBuffer(&myNavState);
        std::cout << "Created string:\n"
                  << myNavState.serialBuffer.serialStringBuffer;
        std::cout << std::endl;
    }

    /*
    Test the UartBuffer cookie interactions
    */

    UartBuffer myUartBuffer;
    initUartBuffer(&myUartBuffer);
    char uartCharBuff[6] = "12345";
    std::cout << "strlen(uartCharBuff): " << strlen(uartCharBuff) << std::endl;
    std::cout << "written bytes: "
              << uartWriter(&myUartBuffer, uartCharBuff, strlen(uartCharBuff))
              << std::endl;

    std::cout << "strlen(myUartBuffer.outputBuffer): "
              << strlen(myUartBuffer.outputBuffer) << std::endl;
    std::cout << "myUartBuffer.outputBuffer: " << myUartBuffer.outputBuffer
              << std::endl;

    std::cout << std::endl;

    std::cout
        << "Move tail to head position to test continuing writing to the buffer"
        << std::endl;
    myUartBuffer.outputTail = myUartBuffer.outputHead;
    std::cout << "myUartBuffer.outputBuffer: " << myUartBuffer.outputBuffer
              << std::endl;
    std::cout << "strlen(uartCharBuff): " << strlen(uartCharBuff) << std::endl;
    std::cout << "written bytes: "
              << uartWriter(&myUartBuffer, uartCharBuff, strlen(uartCharBuff))
              << std::endl;
    std::cout << "myUartBuffer.outputBuffer: " << myUartBuffer.outputBuffer
              << std::endl;

    std::cout << std::endl;

    std::cout << "Move tail to head and write to the buffer to test "
                 "overwriting parts of it" << std::endl;
    myUartBuffer.outputTail = myUartBuffer.outputHead;
    std::cout << "myUartBuffer.outputBuffer: " << myUartBuffer.outputBuffer
              << std::endl;
    char uartCharBuffB[10] = "abcd";
    std::cout << "strlen(uartCharBuff): " << strlen(uartCharBuffB) << std::endl;
    std::cout << "written bytes: "
              << uartWriter(&myUartBuffer, uartCharBuffB, strlen(uartCharBuffB))
              << std::endl;
    std::cout << "myUartBuffer.outputBuffer: " << myUartBuffer.outputBuffer
              << std::endl;
    /*
    Random test area
    */
    // floatDegree testRadianFromFunction = distanceCirclePathAtoB(&coordA,
    // &coordA);
    // printf("expecting latitude, should be 32.956146
    // distanceCirclePathAtoB(&coordA,
    // &coordA) = %f\n", testRadianFromFunction);

    // std::cout << sizeof(Coordinate) << std::endl;
    return 0;
}
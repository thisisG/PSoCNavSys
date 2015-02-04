#ifdef __cplusplus
extern "C" {
#endif
#include "serialcom.h"
#include "nmeafunctions.h"
#ifdef __cplusplus
}
#endif

void initUartBuffer(UartBuffer* uartBuff)
{
    uartBuff->outputBuffer[UART_BUFFER_LENGTH] = { 0 };
    uartBuff->inputBuffer[UART_BUFFER_LENGTH] = { 0 };
    uartBuff->bufferLength = UART_BUFFER_LENGTH;
    uartBuff->outputHead = 0;
    uartBuff->outputTail = 0;
    uartBuff->inputHead = 0;
    uartBuff->inputTail = 0;
}

ssize_t uartOutputWriter(void* outCookie, const char* buffer, size_t size)
{
    UartBuffer* cookie = (UartBuffer*)outCookie;

    //// Ensure that the buffer will not over flor
    // if (outCookie->)
    //{

    //}
}

void navDataToSerialBuffer(NavState* navS)
{
    floatDegree currentNmeaLat = nmeaLatitudeFromCoord(&(navS->currentLocation));
    floatDegree currentNmeaLon = nmeaLongitudeFromCoord(&(navS->currentLocation));
    floatDegree currentHeading = navS->dCurrentHeading;
    floatDegree headingToWp = dHeadingToCurrentWP(navS);
    float distanceToWp = distanceCirclePathAtoB(&(navS->currentLocation), &(navS->nextWaypoint));
    floatDegree nextWpNmeaLat = nmeaLatitudeFromCoord(&(navS->nextWaypoint));
    floatDegree nextWpNmeaLon = nmeaLongitudeFromCoord(&(navS->nextWaypoint));
#ifdef _WIN32
    _snprintf(navS->serialBuffer.serialStringBuffer, navS->serialBuffer.serialBufferLength,
              "hhmmss.ss,%4.4f,%5.4f,%3.3f,%3.3f,%8.1f,%4.4f,%4.4f\r\n", currentNmeaLat,
              currentNmeaLon, currentHeading, headingToWp, distanceToWp, nextWpNmeaLat,
              nextWpNmeaLon);
#endif // _WIN32
#ifdef __GNUC__ // ARM specific implementation
    snprintf(navS->serialBuffer.serialStringBuffer, navS->serialBuffer.serialBufferLength,
             "hhmmss.ss,%4.4f,%5.4f,%3.3f,%3.3f,%8.1f,%4.4f,%4.4f\n", currentNmeaLat,
             currentNmeaLon, currentHeading, headingToWp, distanceToWp, nextWpNmeaLat,
             nextWpNmeaLon);
#endif // __GNUC__
}
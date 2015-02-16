#ifdef __cplusplus
extern "C" {
#endif
#include "serialcom.h"
#ifdef __cplusplus
}
#endif

void initUartBuffer(UartBuffer* uartBuff)
{
    uartBuff->bufferLength = UART_BUFFER_LENGTH;
    size_t i;
    for (i = 0; i < uartBuff->bufferLength; i++)
    {
        uartBuff->outputBuffer[i] = 0;
        uartBuff->inputBuffer[i] = 0;
    }
    uartBuff->outputHead = 0;
    uartBuff->outputTail = 0;
    uartBuff->inputHead = 0;
    uartBuff->inputTail = 0;
}

ssize_t uartWriter(void* outCookie, const char* buffer, size_t size)
{
    UartBuffer* outWriteCookie = (UartBuffer*)outCookie;
    ssize_t byteCount = 0;
    
    // Disable PSoC GPS RX IRS
    #ifdef __GNUC__
    GPS_TX_ISR_Disable();
    #endif // __GNUC__
    
    size_t buffLength = outWriteCookie->bufferLength;
    size_t startHead = outWriteCookie->outputHead;
    size_t headPlusOne = (startHead + 1) % (buffLength - 1);
    // Check if size is larger than effective buffer length, return error (-1)
    // if it is.
    if (size > (buffLength - 1))
    {
        #ifdef _WIN32
        // DEBUG
        printf("size error\n");
        #endif // _WIN32
        return -1;
    }
    // If size is correct fill the buffer.
    else
    {
        while ((byteCount < size) && (headPlusOne != outWriteCookie->outputTail))
        {
            outWriteCookie->outputBuffer[(outWriteCookie->outputHead)]
                = buffer[byteCount];
            ++byteCount;
            outWriteCookie->outputHead = headPlusOne;
            headPlusOne = (headPlusOne + 1) % (buffLength - 1);
        }
    }
    // Check if we wrote all the data, if not reset the head to the starting
    // position and set byteCount to -1 to indicate an error while writing.
    if (byteCount != size)
    {
        outWriteCookie->outputHead = startHead;
        byteCount = -1;
    }
    
    // Enable PSoC GPS RX IRS
    #ifdef __GNUC__
    GPS_TX_ISR_Enable();
    #endif // __GNUC__
    
    return byteCount;
}


/*
** THIS FUNCTION IS WORK IN PROGRESS, RESUME ASAP
*/
ssize_t uartReader(void* inCookie, char* buffer, size_t size)
{
    UartBuffer* inReadCookie = (UartBuffer*)inCookie;
    ssize_t byteCount = 0;
    
    
    // Disable PSoC GPS RX IRS
    #ifdef __GNUC__
    GPS_RX_ISR_Disable();
    #endif // __GNUC__
    
    size_t buffLength = inReadCookie->bufferLength;
    size_t startHead = inReadCookie->inputHead;
    size_t headPlusOne = (startHead + 1) % (buffLength - 1);
    size_t startTail = (inReadCookie->inputTail);

    if (size > (buffLength - 1))
    {
        #ifdef _WIN32
        // DEBUG
        printf("size error\n");
        #endif // _WIN32
        return -1;
    }
    
    while (((inReadCookie->inputTail) != (inReadCookie->inputHead))
           && (byteCount < size))
    {
        buffer[byteCount]
            = inReadCookie->inputBuffer[(inReadCookie->inputTail)];
        ++(inReadCookie->inputTail);
        ++byteCount;
    }

    // Enable PSoC GPS RX IRS
    #ifdef __GNUC__
    GPS_RX_ISR_Enable();
    #endif // __GNUC__
    
    return byteCount;
}

int uartSeeker(void* cookie, off_t* position, int whence)
{
    // This function SHOULD normally be unused as the buffer is a FIFO buffer
    // and should do cyclic read/write on the buffer. Return -1 as this is
    // interpreted as an error.
    return -1;
}

int uartCleaner(void* cookie)
{
    // Since we are using statically assigned memory there should be no need for
    // closing the file. Return -1 as this is interpreted as an error.
    return -1;
}

void navDataToSerialBuffer(NavState* navS)
{
    floatDegree currentLat
        = floatFromNmeaDegree(nmeaLatitudeFromCoord(&(navS->currentLocation)));
    floatDegree currentLon
        = floatFromNmeaDegree(nmeaLongitudeFromCoord(&(navS->currentLocation)));
    floatDegree currentHeading = navS->dCurrentHeading;
    floatDegree headingToWp = dHeadingToCurrentWP(navS);
    float distanceToWp = distanceCirclePathAtoB(&(navS->currentLocation),
                                                &(navS->nextWaypoint));
    floatDegree nextWpLat
        = floatFromNmeaDegree(nmeaLatitudeFromCoord(&(navS->nextWaypoint)));
    floatDegree nextWpLon
        = floatFromNmeaDegree(nmeaLongitudeFromCoord(&(navS->nextWaypoint)));

#ifdef _WIN32
    _snprintf(navS->serialBuffer.serialStringBuffer,
              navS->serialBuffer.serialBufferLength,
              "hhmmss.ss,%4.8f,%5.8f,%3.3f,%3.3f,%8.1f,%4.8f,%4.8f\r\n",
              currentLat, currentLon, currentHeading, headingToWp, distanceToWp,
              nextWpLat, nextWpLon);
#endif // _WIN32

#ifdef __GNUC__ // ARM specific implementation
    snprintf(navS->serialBuffer.serialStringBuffer,
             navS->serialBuffer.serialBufferLength,
             "hhmmss.ss,%4.4f,%5.4f,%3.3f,%3.3f,%8.1f,%4.4f,%4.4f\n",
             currentLat, currentLon, currentHeading, headingToWp, distanceToWp,
             nextWpLat, nextWpLon);
#endif // __GNUC__
}
#ifdef __cplusplus
extern "C" {
#endif
#include "./serialcom.h"
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

// TODO Revisit uartWriter() and see if it causes the UART bug that causes long
// strings to echo improperly.
ssize_t uartWriter(volatile void* outCookie, const char* buffer, size_t size)
{
    UartBuffer* outWriteCookie = (UartBuffer*)outCookie;
    ssize_t byteCount = 0;

// Disable PSoC GPS TX IRS
#ifdef __GNUC__
    GPS_TX_ISR_Disable();
#endif // __GNUC__

    size_t cookieBuffLength = outWriteCookie->bufferLength;
    size_t startHead = outWriteCookie->outputHead;
    size_t headPlusOne = (startHead + 1) % (cookieBuffLength - 1);

    // Check if size is larger than effective buffer length, return error (-1)
    // if it is.
    if (size > (cookieBuffLength - 1))
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
        while ((byteCount < size)
               && (headPlusOne != outWriteCookie->outputTail))
        {
            outWriteCookie->outputBuffer[(outWriteCookie->outputHead)]
                = buffer[byteCount];
            ++byteCount;
            outWriteCookie->outputHead = headPlusOne;
            headPlusOne = (headPlusOne + 1) % (cookieBuffLength - 1);
        }
    }
    // Check if we wrote all the data, if not reset the head to the starting
    // position and set byteCount to -1 to indicate an error while writing.
    if (byteCount != size)
    {
        outWriteCookie->outputHead = startHead;
        byteCount = -1;
    }

// Enable PSoC GPS TX IRS
#ifdef __GNUC__
    GPS_TX_ISR_Enable();
#endif // __GNUC__

    return byteCount;
}

// TODO Revisit uartReader() and see if it causes the UART bug that causes long
// strings to echo improperly.
ssize_t uartReader(volatile void* inCookie, char* buffer, size_t size)
{
    UartBuffer* inReadCookie = (UartBuffer*)inCookie;

// Disable PSoC GPS RX IRS
#ifdef __GNUC__
    GPS_RX_ISR_Disable();
#endif // __GNUC__

    size_t cookieBuffSize = inReadCookie->bufferLength;
    size_t startHead = (inReadCookie->inputHead);
    size_t startTail = (inReadCookie->inputTail);

    // Want to read into the string buffer until end of line is detected.
    char lastChar = '\0';
    char readChar = '\0';
    uint8 readingBuffer = 1;
    size_t tailCookie = startTail;
    ssize_t byteCount = 0;

    while (readingBuffer == 1)
    {
        // Read a character from cookieBuffer using the tail and store it into
        // the string buffer.
        readChar = inReadCookie->inputBuffer[tailCookie];
        buffer[byteCount] = readChar;
        ++byteCount;
        tailCookie = (tailCookie + 1) % (cookieBuffSize - 1);

        // Check if we are at the end of line, if we are - add a null terminator
        // for the string and stop reading of cookie into buffer.
        if (((lastChar == '\r') && (readChar == '\n'))
            || ((lastChar == '\n') && (readChar == '\r')))
        {
            ++byteCount;
            buffer[byteCount] = '\0';
            readingBuffer = 0;
        }
        // If we are not at the end of a line, store the read character value
        // and allow the loop to continue.
        else
        {
            lastChar = readChar;
        }
    }
    // When done reading update the tail of the cookie.
    inReadCookie->inputTail = tailCookie;

// Enable PSoC GPS RX IRS
#ifdef __GNUC__
    GPS_RX_ISR_Enable();
#endif // __GNUC__

    return byteCount;
}

int uartSeeker(volatile void* cookie, off_t* position, int whence)
{
    // This function SHOULD normally be unused as the buffer is a FIFO buffer
    // and should do cyclic read/write on the buffer. Return -1 as this is
    // interpreted as an error.
    return -1;
}

int uartCleaner(volatile void* cookie)
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
              (navS->serialBuffer.serialBufferLength) - 1,
              "hhmmss.ss,%4.8f,%5.8f,%3.3f,%3.3f,%8.1f,%4.8f,%4.8f\r\n",
              currentLat, currentLon, currentHeading, headingToWp, distanceToWp,
              nextWpLat, nextWpLon);
#endif // _WIN32

#ifdef __GNUC__ // ARM specific implementation
    sprintf(navS->serialBuffer.serialStringBuffer,
             "hhmmss.ss,%4.8f,%5.8f,%3.3f,%3.3f,%.1f,%4.8f,%4.8f\r\n",
             currentLat, currentLon, currentHeading, headingToWp, distanceToWp,
             nextWpLat, nextWpLon);
#endif // __GNUC__
}

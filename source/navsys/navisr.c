/*
**
** NavSys library
** URL: https://github.com/thisisG
** Author: Geir Istad (geir dot istad at gmail dot com)
** Licence: MIT
**
*/

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

#include "navisr.h"

#ifdef __cplusplus
}
#endif // __cplusplus

void gpsISRSetUartBuffer(volatile struct UartBuffer* mainBuffer)
{
  ptrUartBuff = mainBuffer;
}

CY_ISR(gpsRxISR)
{
  static uint8 lastChar = '\0';
  size_t headPlusOne = (ptrUartBuff->inputHead + 1) 
                        % (ptrUartBuff->bufferLength - 1);
  uint8 readChar = UART_GPS_GetChar();
  while (readChar != 0)
  {
    // Detect line terminators in the form of either \r\n or \n\r, in which
    // case switch on the (global) rxStringReady flag to allow routines
    // to detect and read the new string.
    if (((lastChar == '\r') && (readChar == '\n'))
        || ((lastChar == '\n') && (readChar == '\r')))
    {
      rxStringReady = 1;
    }
    ptrUartBuff->inputBuffer[ptrUartBuff->inputHead] = readChar;
    ptrUartBuff->inputHead = headPlusOne;
    headPlusOne = (headPlusOne + 1) % (ptrUartBuff->bufferLength - 1);
    lastChar = readChar;
    readChar = UART_GPS_GetChar();
  }
}

CY_ISR(gpsTxISR)
{
  // Fill the UART Tx buffer with characters stored in an instance of a
  // UartBuffer data structure.
  // Since this ISR is only activated when the Tx UART buffer is empty we know
  // that we have 4 slots available in the buffer.
  // Find up to 4 characters in the buffer and putChar() these to the UART.
  size_t tail = (ptrUartBuff->outputTail);
  size_t head = (ptrUartBuff->outputHead);
  size_t buffLength = (ptrUartBuff->bufferLength);
  size_t charCount = 0;
  uint8 sendChar = '\0';

  // Send up to 4 characters or until tail == head.
  while (tail != head && charCount < 4)
  {
    // Get a new character to send
    sendChar = ptrUartBuff->outputBuffer[tail];
    // Send the character
    UART_GPS_PutChar(sendChar);
    ++charCount;
    // Increment the tail to the size of the buffer
    tail = (tail + 1) % (buffLength - 1);
    // Update the tail in the UART buffer data structure
    ptrUartBuff->outputTail = tail;
  }
  // Check if tail = head, if it is disable Tx ISR as there are no more
  // characters due for sending.
  if (tail == head)
  {
    UART_GPS_SetTxInterruptMode(0u);
  }
  // Clear software triggered pending ISR, the cost of checking if it needs to
  // be cleared is as many cycles as it is to clear it. Just clearing it is
  // just as good as checking and then clearing will double the amount of
  // instructions needed.
  GPS_TX_ISR_ClearPending();
}

/* [] END OF FILE */

#ifndef SERIALCOM_H
#define SERIALCOM_H

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus
#include <stdio.h>
#include <stddef.h>
#include "navtypes.h"
#include "navFunctions.h"
#ifdef __cplusplus
}
#endif // __cplusplus

#define UART_BUFFER_LENGTH (128)

typedef int ssize_t;

typedef struct UartBuffer
{
    char outputBuffer[UART_BUFFER_LENGTH];
    char inputBuffer[UART_BUFFER_LENGTH];
    int bufferLength;
    int outputHead;
    int outputTail;
    int inputHead;
    int inputTail;
} UartBuffer;

void initUartBuffer(UartBuffer* uartBuff);

ssize_t uartOutputWriter();

void navDataToSerialBuffer(NavState* navS);

#endif // SERIALCOM_H
#ifndef SERIALCOM_H
#define SERIALCOM_H

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus
#include <stdio.h>
#include <stddef.h>
#include <sys/types.h>
#include "config.h"
#include "navtypes.h"
#include "navfunctions.h"
#include "nmeafunctions.h"

#ifdef __GNUC__
#include "GPS_RX_ISR.h"
#include "GPS_TX_ISR.h"
#endif // __GNUC__

#ifdef __cplusplus
}
#endif // __cplusplus

// Typedefs required so that VS2013 will not complain, these should be included
// on a POSIX system such as the PSoC
#ifdef _WIN32
typedef int ssize_t;
typedef int64_t off64_t;
typedef uint8_t uint8;
// START COPYRIGHT NOTICE UoC
/*
* Copyright (c) 1990 The Regents of the University of California.
* All rights reserved.
*
* Redistribution and use in source and binary forms are permitted
* provided that the above copyright notice and this paragraph are
* duplicated in all such forms and that any documentation,
* advertising materials, and other materials related to such
* distribution and use acknowledge that the software was developed
* by the University of California, Berkeley.  The name of the
* University may not be used to endorse or promote products derived
* from this software without specific prior written permission.
* THIS SOFTWARE IS PROVIDED ``AS IS'' AND WITHOUT ANY EXPRESS OR
* IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
* WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
*
*   @(#)stdio.h 5.3 (Berkeley) 3/15/86
*/
typedef ssize_t cookie_read_function_t(void* __cookie, char* __buf, size_t __n);
typedef ssize_t cookie_write_function_t(void* __cookie, const char* __buf,
                                        size_t __n);
typedef int cookie_seek_function_t(void* __cookie, off_t* __off, int __whence);
typedef int cookie_close_function_t(void* __cookie);

typedef struct
{
    /* These four struct member names are dictated by Linux; hopefully,
    they don't conflict with any macros.  */
    cookie_read_function_t* read;
    cookie_write_function_t* write;
    cookie_seek_function_t* seek;
    cookie_close_function_t* close;
} cookie_io_functions_t;
// END COPYRIGHT NOTICE UoC
#endif // _WIN32

#ifdef __GNUC__
typedef _off64_t off64_t;
#endif // __GNUC__

/* STRUCT UARTBUFFER
ADS that contains software FIFO buffers for sending (Tx) and receiving (Rx) over
UART.

Items due for sending is stored in outputBuffer[outputHead] character by
character. The Tx interrupt is then enabled to be triggered by having an empty
FIFO buffer and will continoue to trigger until outputTail == outputHead, at
which point the ISR handler will disable the FIFO triggered Tx interrupt.
Items received are stored in inputBuffer[inputHead] character by character,
triggered by the UART Rx interrupt on byte received and is managed by the ISR
handler. When a end of line '\r\n' or '\n\r' is detected it will raise a global
flag which will allow a string to be read.
*/
volatile struct UartBuffer
{
    char outputBuffer[UART_BUFFER_LENGTH];
    char inputBuffer[UART_BUFFER_LENGTH];
    size_t bufferLength;
    size_t outputHead;
    size_t outputTail;
    size_t inputHead;
    size_t inputTail;
};

typedef struct UartBuffer UartBuffer;

/* initUartBuffer();
input:
    (ptr) UartBuffer uartBuff
output:
    NA
remarks:
    
*/
void initUartBuffer(UartBuffer* uartBuff);

/* uartWriter();
input:
    (ptr) void outCookie
    (ptr) const char buffer
    (value) size_t size
output:
    NA
remarks:

*/
ssize_t uartWriter(volatile void* outCookie, const char* buffer, size_t size);

/* uartReader();
input:
    (ptr) void inCookie
    (ptr) char buffer
    (value) size_t size
output:
    (value) ssize_t byteCount
remarks:
    
*/
ssize_t uartReader(volatile void* inCookie, char* buffer, size_t size);

/* uartSeeker();
input:
    (ptr) void cookie
    (ptr) off_t position
    (value) int whence
output:
    (value) ssize_t byteCount
remarks:
    
*/
int uartSeeker(volatile void* cookie, off_t* position, int whence);

/* longitudeFromCoordinate();
input:
    (ptr) void cookie
output:
    (value) int // TODO add return value
remarks:

*/
int uartCleaner(volatile void* cookie);

/* navDataToSerialBuffer();
input:
    (ptr) NavState navS
output:
    NA
remarks:
    
*/
void navDataToSerialBuffer(NavState* navS);

#endif // SERIALCOM_H
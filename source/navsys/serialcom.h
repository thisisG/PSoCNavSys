/*
**
** NavSys library
** URL: https://github.com/thisisG
** Author: Geir Istad (geir dot istad at gmail dot com)
** Licence: MIT
**
*/

#ifndef SERIALCOM_H
#define SERIALCOM_H

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus
#include <stdio.h>
#include <stddef.h>
#include <sys/types.h>
#include "navconfig.h"
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
  // These four struct member names are dictated by Linux; hopefully, they don't
  // conflict with any macros.
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
struct UartBuffer
{
  char outputBuffer[UART_BUFFER_LENGTH];
  char inputBuffer[UART_BUFFER_LENGTH];
  size_t bufferLength;
  size_t outputHead;
  size_t outputTail;
  size_t inputHead;
  size_t inputTail;
};

// VS2013 does not like the below definition, so will only use it for the PSoC
#ifdef __GNUC__
typedef volatile struct UartBuffer UartBuffer;
#else  // And make another one for VS2013
typedef struct UartBuffer UartBuffer;
#endif // __GNUC__

/* initUartBuffer();
input:
  (ptr) UartBuffer uartBuff
output:
  NA
remarks:
  Initialised the structure to default values.
*/
void initUartBuffer(UartBuffer* uartBuff);

/* uartWriter();
input:
  volatile void outCookie where the data is written to
  const char* buffer where the data is read from
  size_t the size to write to outCookie
output:
  ssize_t byteCount written
  outWriteCookie->outputBuffer
  outWriteCookie->outputHead
remarks:
  Used to transfer data from a buffer string to a UART ring buffer structure
  Is used for TX using a UART on a PSoC
  Will disable TX ISR before modifying data
  Will enable TX ISR when done modifying data
*/
ssize_t uartWriter(volatile void* outCookie, const char* buffer, size_t size);

/*
input:
  volatile void inCookie* where the data is read from
  char* buffer where the data is written to
  size_t to write to buffer
output:
  ssize_t byteCount read
  buffer[N] is written to with N-1 characters
  inReadCookie->inputTail tail is updated when data transfer is complete
remarks:
  Used to transfer data from a UART ring buffer to a buffer
  Is used for RX using a UART on a PSoC
  Will disable RX ISR before modifying data
  Will enable RX ISR when done modifying data
  Will continoue reading until the end of a string is detected
  NOTE: No check is made to ensure the buffer* is kept within the size of the
  buffer. This could lead to improper behaviour of the system by writing to out
  of bounds values. Should be corrected.
  // TODO: Ensure buffer overflow is prevented.
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
  Not currently used.
*/
int uartSeeker(volatile void* cookie, off_t* position, int whence);

/* longitudeFromCoordinate();
input:
  (ptr) void cookie
output:
  (value) int // TODO add return value
remarks:
  Not currently used.
*/
int uartCleaner(volatile void* cookie);

/* navDataToSerialBuffer();
input:
  (ptr) NavState navS
output:
  NA
remarks:
  Not currently used.
*/
void navDataToSerialBuffer(NavState* navS);

#endif // SERIALCOM_H

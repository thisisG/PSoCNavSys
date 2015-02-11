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
#include "navFunctions.h"
#include "nmeafunctions.h"
#ifdef __cplusplus
}
#endif // __cplusplus

// Typedefs required so that VS2013 will not complain, included on a POSIX
// system such as the PSoC
#ifdef _WIN32
typedef int ssize_t;
typedef int64_t off64_t;
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
*	@(#)stdio.h	5.3 (Berkeley) 3/15/86
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

typedef struct UartBuffer
{
    char outputBuffer[UART_BUFFER_LENGTH];
    char inputBuffer[UART_BUFFER_LENGTH];
    size_t bufferLength;
    size_t outputHead;
    size_t outputTail;
    size_t inputHead;
    size_t inputTail;
} UartBuffer;

void initUartBuffer(UartBuffer* uartBuff);

ssize_t uartWriter(void* outCookie, const char* buffer, size_t size);
ssize_t uartReader(void* inCookie, char* buffer, size_t size);
int uartSeeker(void* cookie, off_t* position, int whence);
int uartCleaner(void* cookie);

void navDataToSerialBuffer(NavState* navS);

#endif // SERIALCOM_H
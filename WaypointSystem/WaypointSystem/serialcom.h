#ifndef SERIALCOM_H
#define SERIALCOM_H

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus
#include <stdio.h>
#include <stddef.h>
#include "navtypes.h"
#include "navFunctions.h"
#include "nmeafunctions.h"
#ifdef __cplusplus
}
#endif // __cplusplus

#define UART_BUFFER_LENGTH (128)

#ifdef _WIN32
typedef int ssize_t;
typedef int64_t _off_t;
typedef _off_t off_t;
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
typedef ssize_t cookie_write_function_t(void* __cookie, const char* __buf, size_t __n);
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
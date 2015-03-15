/*
**
** NavSys library
** URL: https://github.com/thisisG
** Author: Geir Istad (geir dot istad at gmail dot com)
** Licence: MIT
**
*/

#ifndef NAV_SYSTEM_CONFIG_H
#define NAV_SYSTEM_CONFIG_H

// Buffer length for UART FIFO buffer structure
// serialcom.h/c
#define UART_BUFFER_LENGTH (128)

// Buffer length for GPS and SERIAL string buffers used for decoding/encoding
// information before being passed to send/receive buffers.
// navtypes.h/c
#define GPS_STR_BFR_LEN (128)
#define SERIAL_STR_BFR_LEN (128)

// Default value for WP distances for when they should be considered out of
// range or within range for being considered arrived at. Units are metres.
// navfunctions.h/c
#define MAX_WP_DISTANCE (50000)
#define ARRIVED_WP_DISTANCE (500)
#define EXCEPTION_MAX_WP_DISTANCE (100000)
#define EXCEPTION_ARRIVED_WP_DISTANCE (1000)

#endif // NAV_SYSTEM_CONFIG_H

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

// Log file name and enable switch
// Uncomment these to enable logging
#define NAV_LOG_ON
#define NAV_LOG_FILE_NAME "nav.log"
#define NAV_LOG_GPS_STRINGS "gps.log"

// Default config filename used to initialise NavWPFileManager structures
// wphandler.h/c
#define DEFAULT_CONFIG_FILE "navsys.cfg"

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

// For NavTestOne
/*
#define MAX_WP_DISTANCE (1100)
#define ARRIVED_WP_DISTANCE (300)
#define EXCEPTION_MAX_WP_DISTANCE (500)
#define EXCEPTION_ARRIVED_WP_DISTANCE (300)
*/
// For NavTestTwo
/*
#define MAX_WP_DISTANCE (1500)
#define ARRIVED_WP_DISTANCE (500)
#define EXCEPTION_MAX_WP_DISTANCE (300)
#define EXCEPTION_ARRIVED_WP_DISTANCE (2000)
*/
// For ProperTest
#define MAX_WP_DISTANCE (100)
#define ARRIVED_WP_DISTANCE (50)
#define EXCEPTION_MAX_WP_DISTANCE (100)
#define EXCEPTION_ARRIVED_WP_DISTANCE (75)


// Sizes for datastructures in packed binary style since sizeof() would return
// an errenouous value.
// wphandler.h/c
#define SIZE_NAV_FILE_HEADER (6)
#define SIZE_NAV_CFG_FILE_HEADER (4*4)
#define SIZE_NAV_FILE_WP_LIST_HEADER (13*2 + 8)
#define SIZE_NAV_DATABLOCK_HEADER (6)
#define SIZE_COORDINATE (13)

#endif // NAV_SYSTEM_CONFIG_H

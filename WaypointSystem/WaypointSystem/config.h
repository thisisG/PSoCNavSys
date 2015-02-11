#ifndef NAV_SYSTEN_CONFIG_H
#define NAV_SYSTEN_CONFIG_H

// Buffer length for UART FIFO buffer structure
// serialcom.h/c
#define UART_BUFFER_LENGTH (11)

// Buffer length for GPS and SERIAL string buffers used for decoding/encoding
// information before being passed to send/receive buffers.
// navtypes.h/c
#define GPS_STR_BFR_LEN (128)
#define SERIAL_STR_BFR_LEN (128)

#endif
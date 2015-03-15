/*
**
** NavSys library
** URL: https://github.com/thisisG
** Author: Geir Istad (geir dot istad at gmail dot com)
** Licence: MIT
**
*/

#ifndef NAVISR_H
#define NAVISR_H

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

#include "navtypes.h"
#include "serialcom.h"
#include "UART_GPS.h"
#include "GPS_TX_ISR.h"

#ifdef __cplusplus
}
#endif // __cplusplus

// Global variables allowing ISRs to access data in found in other scopes.
extern volatile UartBuffer* ptrUartBuff;
extern volatile uint8 rxStringReady;
extern volatile uint8 txStringReady;

// TODO
void gpsISRSetUartBuffer(volatile struct UartBuffer* mainBuffer);

/* gpsRxISR();
input:
    NA
output:
    NA
remarks:
    Generated an ISR used on a received byte over UART from a GPS devices
    attached using the CY_ISR_PROTO() and CY_ISR() macros.
    Takes one (or more if several new bytes) out of the Rx receive buffer and
    store these in the inputBuffer in an instance of a UartBuffer data
    structure.
*/
CY_ISR_PROTO(gpsRxISR);

/* gpsTxISR();
input:
    NA
output:
    NA
remarks:
    Generated an ISR used to fill the Tx buffer for the UART connected to a GPS
    device using the CY_ISR_PROTO() and CY_ISR() macros.
    When there is a string or part of a string waiting for transfer in an
    instance of a UartBuffer the ISR will fill the UART Tx buffer until the
    whole string is sent.
    It will then disable the Tx interrupt until a new is to be sent over UART.
*/
CY_ISR_PROTO(gpsTxISR);

#endif // NAVISR_H

/* [] END OF FILE */

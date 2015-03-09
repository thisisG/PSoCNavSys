/* ========================================
 *
 * Copyright YOUR COMPANY, THE YEAR
 * All Rights Reserved
 * UNPUBLISHED, LICENSED SOFTWARE.
 *
 * CONFIDENTIAL AND PROPRIETARY INFORMATION
 * WHICH IS THE PROPERTY OF your company.
 *
 * ========================================
*/

#ifndef NAVISR_H
#define NAVISR_H

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

#include "./navtypes.h"
#include "./serialcom.h"
#include "UART_GPS.h"
#include "GPS_TX_ISR.h"

#ifdef __cplusplus
}
#endif // __cplusplus

// Global variables allowing ISRs to access data in found in other scopes.
volatile UartBuffer* ptrUartBuff = NULL;
volatile uint8 rxStringReady = 0;
volatile uint8 txStringReady = 0;

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

#endif

/* [] END OF FILE */

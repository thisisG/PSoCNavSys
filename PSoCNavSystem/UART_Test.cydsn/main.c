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
#include <project.h>
#include <stdio.h>
#include <stdlib.h>
#include "serialcom.h"
#include "navisr.h"
#include "GPS_RX_ISR.h"
#include "GPS_TX_ISR.h"
#include "UART_GPS.h"

/* Global variables and pointers */

// From navisr.h/c
extern uint8 rxStringReady;

int main()
{
    // Disable global interrupts during setup
    CyGlobalIntDisable;
    
    // LCD Setup
    LCD_Start();
    LCD_Position(0u, 0u);
    LCD_PrintString("UART Test!");
    LCD_Position(1u, 0u);
    
    // Navsystem setup
    struct NavState myNavState;
    zeroNavState(&myNavState);
    
    /*
    ** UART setup
    */
    // Create UART buffer structure and initialize it
    UartBuffer myUartBuffer;
    initUartBuffer(&myUartBuffer);
    
    // Set the address to the UART buffer in the ISR
    gpsISRSetUartBuffer(&myUartBuffer);
    
    // Enable UART ISR
    UART_GPS_Enable();
    
    
    /*
    ** Setup for custom IO streams for use with the UART buffer
    */
    // Set up cookie functions for custom IO stream
    cookie_io_functions_t uartIOFunctions;
    uartIOFunctions.close = NULL;
    uartIOFunctions.read = uartReader;
    uartIOFunctions.seek = NULL;
    uartIOFunctions.write = uartWriter;
    
    FILE ptrCookie = *(fopencookie(&myUartBuffer, "", uartIOFunctions));
    
    /*
    ** ISR setup
    */
    // Set the address to the ISR UART RX handler and start the ISR handling
    GPS_RX_ISR_StartEx(gpsRxISR);
    GPS_TX_ISR_StartEx(gpsTxISR);
    
    // Enable global interrupts
    CyGlobalIntEnable;
    
    size_t lcdLength = 16;
    int counter = 0;
    char aChar = '\0';
    char uStatus = '\0';
    uint8 directToLCD = 1;
    
    for(;;)
    {
        if (rxStringReady == 1)
        {
             fread(&(myNavState.gpsBuffer.gpsStringBuffer),
                   1, myNavState.gpsBuffer.gpsBufferLength, 
                   &ptrCookie);
            rxStringReady = 0;
        }
        
        aChar = myUartBuffer.inputBuffer[myUartBuffer.inputTail];
        if ((directToLCD != 0) && (aChar != '\0'))
        {
            ++counter;
            if ((counter % lcdLength-1) == 0)
            {
                 LCD_Position(1u, 0u);
                counter = 0;
            }
            LCD_PutChar(aChar);
            UART_GPS_PutChar(aChar);
        }
        
    }
}

/* [] END OF FILE */

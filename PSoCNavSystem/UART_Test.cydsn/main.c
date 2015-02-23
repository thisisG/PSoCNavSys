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
// #include <stdio.h>
// #include <stdlib.h>
// #include <unistd.h> // For read(), write()
#include "serialcom.h"
#include "navisr.h"
#include "GPS_RX_ISR.h"
#include "GPS_TX_ISR.h"
#include "UART_GPS.h"
#include "gpsinterface.h"

/* Global variables and pointers */

// From navisr.h/c
extern volatile uint8 rxStringReady;
extern volatile uint8 txStringReady;

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
    UART_GPS_Start();
    
    
    /*
    ** Setup for custom IO streams for use with the UART buffer
    */
    // For now disabled until a basic UART implementation can be made and tested
    /*
    // Set up cookie functions for custom IO stream
    cookie_io_functions_t uartIOFunctions;
    uartIOFunctions.close = NULL;
    uartIOFunctions.read = &uartReader;
    uartIOFunctions.seek = NULL;
    uartIOFunctions.write = &uartWriter;
    */
    
    // For now disabled until a basic UART implementation can be made and tested
    // FILE ptrCookie = *(fopencookie(&myUartBuffer, "w+", uartIOFunctions));
    
    /*
    ** ISR setup
    */
    // Attach the Rx ISR handler to the UART Rx interrupt.
    // The Rx ISR is triggered on byte received in its initial state.
    GPS_RX_ISR_StartEx(gpsRxISR);
    
    // Attach the Tx ISR handler to the UART Tx interrupt.
    // The Tx ISR is triggered by nothing in its initial state. The interrupts 
    // are switched on when needed (when a string is due for sending).
    GPS_TX_ISR_StartEx(gpsTxISR);
    
    // Enable global interrupts
    CyGlobalIntEnable;
    
    size_t lcdLength = 16;
    int counter = 0;
    char aChar = '\0';
    char uStatus = '\0';
    uint8 directToLCD = 0;
    
    for(;;)
    {
        if (rxStringReady == 1)
        {
            // Load the GPS string into the GPS String buffer in NavState.
            uartReader(&myUartBuffer, &(myNavState.gpsBuffer.gpsStringBuffer)[0],
                       myNavState.gpsBuffer.gpsBufferLength - 1);
            
            // Clear the rxStringReady flag.
            rxStringReady = 0;
            
            // For debugging update the LCD display with the received string.
            LCD_PrintString(myNavState.gpsBuffer.gpsStringBuffer);
            
            // Load the information in the GPS string into NavState, this means
            // updating the current location etc to the data in the GPS string.
            decodeGpsStringInNavState(&myNavState);
            
            // Prepare serial string buffer in NavState with current navdata
            navDataToSerialBuffer(&myNavState);
            
            // Write the serial buffer to the UART buffer
            uartWriter(&myUartBuffer, myNavState.serialBuffer.serialStringBuffer, 
                       strlen(myNavState.serialBuffer.serialStringBuffer));
            
            // Raise a flag for sending the serial data 
            txStringReady = 1;
            
            /*
            ** The following is currently not working. Will try to fix at a 
            ** later stage if time allows.
            
            // fread(&(myNavState.gpsBuffer.gpsStringBuffer),
            //       1, myNavState.gpsBuffer.gpsBufferLength, 
            //       &ptrCookie);
            

            // uartWriter(&myUartBuffer, myNavState.gpsBuffer.gpsStringBuffer, 
            //            strlen(myNavState.gpsBuffer.gpsStringBuffer));
            */
            
        }
        
        if(txStringReady == 1)
        {
            // Set the interrupt mode for the UART Tx to empty FIFO buffer
            uint8 txISRMode = UART_GPS_TX_STS_FIFO_EMPTY;
            UART_GPS_SetTxInterruptMode(txISRMode);
            
            // Trigger the ISR in software to start the sending
            GPS_TX_ISR_SetPending();
            
            // Lower the flag for having a tx string ready, the sending of the 
            // data is now handled by the ISR
            txStringReady = 0;
        }
    }
}

/* [] END OF FILE */

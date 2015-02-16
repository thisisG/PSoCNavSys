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
#include "serialcom.h"
#include "navisr.h"
#include "GPS_RX_ISR.h"
#include "UART_GPS.h"

/* Global variables and pointers */

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
    gpsRxISRSetUartBuffer(&myUartBuffer);
    
    // Enable UART ISR
    UART_GPS_Enable();
    
    // Set up cookie functions for custom IO stream
    cookie_io_functions_t uartIOFunctions;
    uartIOFunctions.close = uartCleaner;
    uartIOFunctions.read = uartReader;
    uartIOFunctions.seek = uartSeeker;
    uartIOFunctions.write = uartWriter;
    
    /*
    ** ISR setup
    */
    // Set the address to the ISR UART RX handler and start the ISR handling
    GPS_RX_ISR_StartEx(gpsRxISR);
    
    // Enable global interrupts
    CyGlobalIntEnable;
    
    size_t lcdLength = 16;
    int counter = 0;
    char aChar = '\0';
    char uStatus = '\0';
    uint8 directToLCD = 1;
    
    for(;;)
    {
        //CyDelay(1000);
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

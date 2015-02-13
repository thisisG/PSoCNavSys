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
#include <stddef.h>
#include "serialcom.h"
#include "GPS_RX_ISR.h"
#include "UART_GPS.h"

/* Global variables and pointers */


int main()
{
    /* Place your initialization/startup code here (e.g. MyInst_Start()) */


    
    // LCD Setup
    LCD_Start();
    LCD_Position(0u, 0u);
    LCD_PrintString("UART Test!");
    
    // Navsystem setup
    struct NavState myNavState;
    zeroNavState(&myNavState);
    
    // UART Setup
    UartBuffer myUartBuffer;
    initUartBuffer(&myUartBuffer);
    
    GPS_RX_ISR_SETBUFF(&myUartBuffer);
    GPS_RX_ISR_Start();
    cookie_io_functions_t uartIOFunctions;
    uartIOFunctions.close = uartCleaner;
    uartIOFunctions.read = uartReader;
    uartIOFunctions.seek = uartSeeker;
    uartIOFunctions.write = uartWriter;
    
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
                 LCD_Position(1,0);
            }
            LCD_PutChar(aChar);
            UART_GPS_PutChar(aChar);
        }
        
    }
}

/* [] END OF FILE */

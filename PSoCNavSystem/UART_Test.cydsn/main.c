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

/*
** Setup here
*/


UartBuffer myUartBuffer;

cookie_io_functions_t uartIOFunctions;

int main()
{
    /* Place your initialization/startup code here (e.g. MyInst_Start()) */

    // Enable global interrupts
    CyGlobalIntEnable;
    
    // LCD Setup
    LCD_Start();
    LCD_Position(0u, 0u);
    LCD_PrintString("UART Test!");
    
    // UART Setup
    UART_GPS_Start();
    
    initUartBuffer(&myUartBuffer);
    
    uartIOFunctions.close = uartCleaner;
    uartIOFunctions.read = uartReader;
    uartIOFunctions.seek = uartSeeker;
    uartIOFunctions.write = uartWriter;
    
    size_t lcdLength = 16;
    int counter = 0;
    char aChar = '\0';
    char uStatus = '\0';

    
    for(;;)
    {
        aChar = UART_GPS_GetChar();
        if (aChar != '\0')
        {
            ++counter;
            if ((counter % lcdLength-1) == 0)
            {
                 LCD_Position(1,0);
            }
            LCD_PutChar(aChar);
        }
        
    }
}

/* [] END OF FILE */

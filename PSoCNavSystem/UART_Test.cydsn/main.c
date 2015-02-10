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

    LCD_Start();
    CyGlobalIntEnable;
    LCD_Position(0u, 0u);
    LCD_PrintString("UART Stat: ");
    
    UART_GPS_Start();
    
    char lcdBuffer[40];
    int counter = 0;
    char aChar = '\0';
    char uStatus = '\0';
    initUartBuffer(&myUartBuffer);
    
    uartIOFunctions.close = uartCleaner;
    uartIOFunctions.read = uartReader;
    uartIOFunctions.seek = uartSeeker;
    uartIOFunctions.write = uartWriter;
    
    /* CyGlobalIntEnable; */ /* Uncomment this line to enable global interrupts. */
    for(;;)
    {
        aChar = UART_GPS_GetChar();
        uStatus = UART_GPS_GetRxBufferSize();
        uStatus = uStatus + 48u;
        if (aChar != '\0')
        {
            LCD_PutChar(aChar);
            CyDelay(5000);
        }
        else
        {
                        LCD_ClearDisplay();
            LCD_Position(0,0);
            LCD_PrintString("UART Rx Buff Size:");
            LCD_Position(1,0);

            LCD_PutChar(uStatus);
        }
        
    }
}

/* [] END OF FILE */

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

UartBuffer myUartBuffer;

cookie_io_functions_t uartIOFunctions;

int main()
{
    /* Place your initialization/startup code here (e.g. MyInst_Start()) */

    initUartBuffer(&myUartBuffer);
    
    uartIOFunctions.close = uartCleaner;
    uartIOFunctions.read = uartReader;
    uartIOFunctions.seek = uartSeeker;
    uartIOFunctions.write = uartWriter;
    
    /* CyGlobalIntEnable; */ /* Uncomment this line to enable global interrupts. */
    for(;;)
    {
        /* Place your application code here. */
    }
}

/* [] END OF FILE */

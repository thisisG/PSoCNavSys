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
#include <FS.h> // For file system for SD card

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
    
    // Initialize file system
    FS_Init();

    
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
    
    // File related variables
    char logFileName[32] = "\\DIR\\stdlog.txt";
    FS_FILE* ptrLogFile;
    char sdVolName[10]; // Char buffer for SD card volume name
    char testString[20] = "Some text."; // Char string to write to file
    uint8 testedSDCard = 0;
    
    for(;;)
    {
        if (testedSDCard == 0)
        {
            LCD_Position(0u, 0u);
            if(0 != FS_GetVolumeName(0u, &sdVolName[0], 9u))
            {
                /* Getting volume name succeeded so prompt it on the LCD */
                LCD_Position(0u, 0u);
                LCD_PrintString("SD card name:");
                LCD_Position(1u, 0u);
                LCD_PrintString(sdVolName);
            }
            else
            {
                /* Operation Failed - indicate this */
                LCD_PrintString("Failed to get");
                LCD_Position(1u, 0u);
                LCD_PrintString("SD card name");
            }    
            testedSDCard = 1;
            CyDelay(2000);
        }
        
        if (testedSDCard == 1)
        {
            LCD_Position(0u, 0u);
            LCD_PrintString("SD card format");
            
            if(0 == FS_FormatSD(sdVolName))
            {
                LCD_Position(1u, 0u);
                LCD_PrintString("Succeeded");
            }
            else
            {
                LCD_Position(1u, 0u);
                LCD_PrintString("Failed");
            }
            testedSDCard++;
            CyDelay(2000);
        }
        
        if (testedSDCard == 2)
        {
            LCD_Position(0u, 0u);
            if(0 == FS_MkDir("Dir"))
            {
                /* Display successful directory creation message */
                LCD_PrintString("\"Dir\" created");
            }
            else
            {
                /* Display failure message */
                LCD_PrintString("Failed to create");
                LCD_Position(1u, 0u);
                LCD_PrintString("directory");
            }
            testedSDCard++;
            CyDelay(2000);
        }
        
        if (testedSDCard == 3)
        {
            ptrLogFile = FS_FOpen(logFileName, "a");
            
            // Check if file opened successfully
            if (ptrLogFile != NULL)
            {
                LCD_Position(0u,0u);
                LCD_PrintString("Opened file!");
                LCD_Position(1, 0);
                LCD_PrintString(logFileName);
                CyDelay(2000);
                
                if (0 != FS_Write(ptrLogFile, testString, strlen(testString)))
                {
                    LCD_Position(0,0);
                    LCD_PrintString("Wrote to file:");
                    LCD_Position(1,0);
                    LCD_PrintString(testString);
                    CyDelay(2000);
                }
                else
                {
                    LCD_Position(0,0);
                    LCD_PrintString("Unable to write");
                    LCD_Position(1, 0);
                    LCD_PrintString(logFileName);
                    CyDelay(2000);
                }
            }
            else
            {
                LCD_Position(0,0);
                LCD_PrintString("Unable to open");
                LCD_Position(1, 0);
                LCD_PrintString(logFileName);
                CyDelay(2000);
            }
            testedSDCard++;
        }

        
        if (rxStringReady == 1)
        {
            // Load the GPS string into the GPS String buffer in NavState.
            uartReader(&myUartBuffer, &(myNavState.gpsBuffer.gpsStringBuffer)[0],
                       myNavState.gpsBuffer.gpsBufferLength - 1);
            
            // Clear the rxStringReady flag.
            rxStringReady = 0;
            
            // For debugging update the LCD display with the received string.
            if (directToLCD == 1)
            {
                LCD_PrintString(myNavState.gpsBuffer.gpsStringBuffer);
            }

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

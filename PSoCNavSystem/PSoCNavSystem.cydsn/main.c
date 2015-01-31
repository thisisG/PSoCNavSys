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

#include "navtypes.h"
#include "navfunctions.h"
#include "./nmea/nmea.h"
#include <math.h>

// coordinateCode;
int main()
{
    // Create an instance of NavState
    struct NavState myNavState;
    /* Place your initialization/startup code here (e.g. MyInst_Start()) */
    zeroNavState(&myNavState);
    /* CyGlobalIntEnable; */ /* Uncomment this line to enable global interrupts. */
    for(;;)
    {
        
    }
}

/* [] END OF FILE */

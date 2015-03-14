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

#ifndef WPHANDLER_H
#define WPHANDLER_H

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

// Depends on Coordinate and NavState structures when writing to files
#include "navtypes.h"

// Depends on having access to the file system functions and definitions as well
// as file types
#include "navfiles.h"
#include <sys/types.h>

#ifdef __cplusplus
}
#endif // __cplusplus

/* STRUCT NavWPFileManager
ADS that contain the filenames and pointers used for managing waypoints via
files.
*/
typedef struct NavWPFileManager
{
    char cfgFileName[20];
    char wpListFileName[20];
    char eWPListFileName[20];
    NAV_FILE* ptrCfgFile;
    NAV_FILE* ptrWPList;
    size_t numberOfExceptionEntries;
    size_t currentExceptionEntry;
} NavWPFileManager;

/* STRUCT NavWPHandler
ADS that contain data regarding the management of waypoints.
*/
typedef struct NavWPHandler
{
    struct NavWPFileManager fileManager;
    struct Coordinate wpGoal;
    size_t offsetFirstWPBlock;
    size_t currentWPCount;
    size_t maxWPCount;
} NavWPHandler;

// TODO
uint8_t WPHandlerOpen(NavWPHandler* wpHandler, char* wpFileName);

// TODO
void WPHandlerGetGoal(NavWPHandler* wpHandler, Coordinate* wpGoal);

// TODO
size_t WPHandlerNextWP(NavWPHandler* wpHandler, Coordinate* nextWP);

// TODO
void WPHandlerSeekWP(NavWPHandler* wpHandler, const size_t wpNumber);

#endif // WPHANDLER_H

/* [] END OF FILE */
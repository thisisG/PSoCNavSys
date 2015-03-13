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

// Depends on having access to the file system functions and definitions
#ifdef __GNUC__
#include "FS.h"
#else // File operations for windows
#include <stdio.h>
#endif // __GNUC__

// Depends on file types
#include "navfiles.h"
#include <sys/types.h>

#ifdef __cplusplus
}
#endif // __cplusplus

// If we are not on the PSoC system we want to make a typedef for FS_FILE such
// that the handler code works with other platforms
#ifndef __GNUC__
typedef FILE FS_FILE;
#endif // __GNUC__

/* STRUCT NavWPFileManager
ADS that contain the filenames and pointers used for managing waypoints via
files.
*/
typedef struct NavWPFileManager
{
    char cfgFileName[20];
    char wpListFileName[20];
    char eWPListFileName[20];
    FS_FILE* ptrCfgFile;
    FS_FILE* ptrWPList;
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
uint8_t wpHandlerOpen(NavWPHandler* wpHandler, char* wpFileName);

// TODO
void WPHandlerGetGoal(NavWPHandler* wpHandler, Coordinate* wpGoal);

// TODO
size_t WPHandlerNextWP(NavWPHandler* wpHandler, Coordinate* nextWP);

// TODO
void WPHandlerSeekWP(NavWPHandler* wpHandler, const size_t wpNumber);

#endif // WPHANDLER_H

/* [] END OF FILE */

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
#ifndef NAVFILES_H
#define NAVFILES_H

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

// Depends on Coordinate and NavState structures when writing to files
#include "navtypes.h"
// Depends on having access to the file system functions
#include "FS.h"

#ifdef __cplusplus
}
#endif // __cplusplus

/* ENUM NavFileTypes
Numbered list describing the type of files the system is aware of and know how
to perform operations with.
The first and last entry should be invalid types which is only used for enabling
iteration through the enumerated types.
*/
typedef enum NavFileType
{
    StartFileType = 0,
    WaypointList,
    ExceptionWaypointList,
    LastFileType
} NavFileType;

/* ENUM NavFileTypes
Numbered list describing the type version of the file we are working with in
case it is needed for further development of the system.
The first and last entry should be invalid types which is only used for enabling
iteration through the enumerated types.
*/
typedef enum NavFileVersion
{
    StartFileVersion = 0,
    FileVersion1,
    LastFileVersion
} NavFileVersion;

/* STRUCT NavFileHeader
ADS that contain information of the type of file we have opened.
Any file which the system intends to read and write to should have one of these
present as the first entry in the file.
The exception are text files which are opened in append mode only, examples of
this would a system log file which are made in pure text and not intended to be
read back by the PSoC.
*/
typedef struct NavFileHeader
{
    NavFileType fileType;
    NavFileVersion fileVersion;
} NavFileHeader;

/* STRUCT NavFileWaypointList
ADS that describes the contents of a list of waypoints contained in the file.
*/
typedef struct NavFileWaypointList
{
	Coordinate startCoordinate;
	Coordinate endCoordinate;
} NavFileWaypointList;

#endif // NAVFILES_H
/* [] END OF FILE */
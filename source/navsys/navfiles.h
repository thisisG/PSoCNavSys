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
#include "./navtypes.h"
// Depends on having access to the file system functions and definitions
#include "FS.h"
#include <sys/types.h>

#ifdef __cplusplus
}
#endif // __cplusplus

/* ENUM NavFileType
Numbered list describing the type of files the system is aware of and know how
to perform operations with.
The numbers are chosen apparently at random, and care should be taken to choose
a new number when adding new types.
*/
typedef enum NavFileType
{
    INVALID_FILE_TYPE = 0,
    WAYPOINT_LIST_FILE = 3245,
    EXCEPTION_WAYPOINT_LIST_FILE = 5723,
    WAYPOINT_CONFIG_FILE = 6573,
} NavFileType;

/* ENUM NavVersion
Numbered list describing the type version of the file we are working with in
case it is needed for further development of the system.
*/
typedef enum NavVersion
{
    INVALID_FILE_VERSION = 0,
    FILE_VERSION_1,
} NavVersion;

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
    NavVersion fileVersion;
    size_t headerBlockSize; // The size of the header block after the file header
} NavFileHeader;

/* ENUM NavDataType
Numbered list describing the type of data blocks the system is aware of and know
how to perform operations on.
The numbers are chosen apparently at random, and care should be taken to choose
a new and unused number when adding new types.
*/
typedef enum NavDataType
{
    INVALID_DATA_TYPE = 0,
    WAYPOINT_DATA = 5123,
    EXCEPTION_WAYPOINT_DATA = 3455
} NavDataType;

/* STRUCT NavDataBlockHeader
ADS that contains information about the block of data following the header.
Any datablock should have one of these in front of them in order to allow for
future expansion of datatypes, and also allow older versions of the software to
more recent versions of stored data.
*/
typedef struct NavDataBlockHeader
{
    NavDataType dataType;
    NavVersion dataVersion;
    size_t dataBlockSize; // The size of the data block after the header
} NavDataBlockHeader;

/* STRUCT NavFileWPListHeader
ADS that describes the contents of a list of waypoints contained in the file.
*/
typedef struct NavFileWPListHeader
{
    Coordinate startCoordinate;
    Coordinate endCoordinate;
    size_t numberOfEntries;
    size_t dataBlockSize;
} NavFileWPListHeader;

#endif // NAVFILES_H

/* [] END OF FILE */

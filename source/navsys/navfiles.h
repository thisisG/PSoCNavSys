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

// Depends on Coordinate and NavState structures when writing to files.
#include "navtypes.h"
#include <sys/types.h>

#ifdef __cplusplus
}
#endif // __cplusplus

// Create typedefs for file pointers in order to create interfaces for
// FS_FOpen()/fopen(), FS_Read()/fread(), FS_FSeek()/fseek() etc.
#ifdef __GNUC__
typedef FS_FILE NAV_FILE;
#else
typedef FILE NAV_FILE;
#endif // __GNUC__

// Define seek parameters for use with NAV_fseek() function calls.
#define NAV_SEEK_SET 0
#define NAV_SEEK_CUR 1
#define NAV_SEEK_END 2

// TODO Description NAV_fopen()
NAV_FILE* NAV_fopen(const char* filename, const char* mode);

// TODO Description NAV_fclose()
int NAV_fclose(NAV_FILE* ptrNavFile);

// TODO Description NAV_fseek()
int NAV_fseek(NAV_FILE* ptrNavFile, const int32_t offset, const int origin);

// TODO Description NAV_fwrite()
size_t NAV_fwrite(const void * ptrData, size_t size, size_t count, NAV_FILE* ptrNavFile);

// TODO Description NAV_fread()
uint32_t NAV_fread(void * ptrData, size_t size, size_t count, NAV_FILE* ptrNavFile);

/* ENUM NavFileType
Numbered list describing the type of files the system is aware of and know how
to perform operations with.
The numbers are chosen apparently at random, and care should be taken to choose
a new number when adding new types.
The INVALID_MAX_FILE_TYPE is defined to force the enum to be the size of 4 B 
since the PSoC will otherwise make the enum of length 1 B.
*/
typedef enum NavFileType
{
    INVALID_FILE_TYPE = 0,
    WAYPOINT_LIST_FILE = 3245,
    EXCEPTION_WAYPOINT_LIST_FILE = 5723,
    WAYPOINT_CONFIG_FILE = 6573,
    INVALID_MAX_FILE_TYPE = 2147483647
} NavFileType;

/* ENUM NavVersion
Numbered list describing the type version of the file we are working with in
case it is needed for further development of the system.
*/
typedef enum NavVersion
{
    INVALID_FILE_VERSION = 0,
    FILE_VERSION_1,
    INVALID_MAX_VERSION = 2147483647
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

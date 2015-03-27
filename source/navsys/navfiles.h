/*
**
** NavSys library
** URL: https://github.com/thisisG
** Author: Geir Istad (geir dot istad at gmail dot com)
** Licence: MIT
**
*/

#ifndef NAVFILES_H
#define NAVFILES_H

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

// Depends on Coordinate and NavState structures when writing to files.
#include "navtypes.h"
// Depends on initialization functions for navtypes
#include "navfunctions.h"
#include <sys/types.h>

// Depends on the emFile library when on the PSoC
#ifdef __GNUC__
#include "FS.h"
#else
#include <stdio.h>
#endif // __GNUC__

#ifdef __cplusplus
}
#endif // __cplusplus

/***********************************************
**
** Defines
**
***********************************************/
// Define seek parameters for use with NAV_fseek() function calls.
#define NAV_SEEK_SET 0
#define NAV_SEEK_CUR 1
#define NAV_SEEK_END 2

/***********************************************
**
** Type declarations
**
***********************************************/

// Create typedefs for file pointers in order to create interfaces for
// FS_FOpen()/fopen(), FS_Read()/fread(), FS_FSeek()/fseek() etc.
#ifdef __GNUC__
typedef FS_FILE NAV_FILE;
#else
typedef FILE NAV_FILE;
#endif // __GNUC__

/* ENUM NavFileType
Numbered list describing the type of files the system is aware of and know how
to perform operations with.
*/
typedef enum NavFileType
{
  INVALID_FILE_TYPE = 0,
  WAYPOINT_LIST_FILE = 1,
  EXCEPTION_WAYPOINT_LIST_FILE = 2,
  CONFIG_FILE = 3
} NavFileType;

/* ENUM NavVersion
Numbered list describing the type version of the file we are working with in
case it is needed for further development of the system.
*/
typedef enum NavVersion
{
  INVALID_NAV_VERSION = 0,
  NAV_VERSION_1
} NavVersion;

/* ENUM NavDataType
Numbered list describing the type of data blocks the system is aware of and know
how to perform operations on.
*/
typedef enum NavDataType
{
  INVALID_DATA_TYPE = 0,
  WAYPOINT_DATA = 1,
  EXCEPTION_WAYPOINT_DATA = 2
} NavDataType;

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
  uint8_t fileType;
  uint8_t fileVersion;
  // The size of the header block after the header.
  // This feature is currently not used, but the space is reserved for future
  // development of the system.
  uint32_t nextHeaderSize;
} NavFileHeader;

/* STRUCT NavConfigFileHeader
ADS containing the number of WP lists and exception WP lists.
The file also contains the currently used list and should be updated to the
correct number when a new list is chosen in order to resume operations in case
of power failure or other types of system resets.
After this header there will be numberOfWPLists + numberOfExeptionWPLists
char[20] arrays containing the filenames of the lists the system should be aware
of.
*/
typedef struct NavConfigFileHeader
{
  uint32_t numberOfWPLists;
  uint32_t currentWPList;
  uint32_t numberOfExeptionWPLists;
  uint32_t currentExeptionWPList;
} NavConfigFileHeader;

/* STRUCT NavFileWPListHeader
ADS that describes the contents of a list of waypoints contained in the file.
*/
typedef struct NavFileWPListHeader
{
  Coordinate startCoordinate;
  Coordinate endCoordinate;
  uint32_t numberOfEntries;
  // The size of the header block after the header.
  // This feature is currently not used, but the space is reserved for future
  // development of the system.
  uint32_t nextHeaderSize;
} NavFileWPListHeader;

/* STRUCT NavDatablockHeader
ADS that contains information about the block of data following the header.
Any datablock should have one of these in front of them in order to allow for
future expansion of datatypes, and also allow older versions of the software to
more recent versions of stored data.
*/
typedef struct NavDatablockHeader
{
  uint8_t dataType;
  uint8_t dataVersion;
  // The size of the data block after the header.
  // This feature is currently not used, but the space is reserved for future
  // development of the system.
  uint32_t nextDataSize;
} NavDatablockHeader;

/***********************************************
** Initialisation function declarations
***********************************************/

// TODO Description initNavFileHeader()
void initNavFileHeader(NavFileHeader* fileHeader);

// TODO Description initNavConfigFileHeader()
void initNavConfigFileHeader(NavConfigFileHeader* ptrCfgFileHeader);

// TODO Description initNavFileWPListHeader()
void initNavFileWPListHeader(NavFileWPListHeader* WPListHeader);

// TODO Description initNavDataBlockHeader()
void initNavDatablockHeader(NavDatablockHeader* dataHeader);

/***********************************************
** Basic file interfaces
***********************************************/

// TODO Description NAV_fopen()
NAV_FILE* NAV_fopen(const char* filename, const char* mode);

// TODO Description NAV_fclose()
int NAV_fclose(NAV_FILE* ptrNavFile);

// TODO Description NAV_fseek()
int NAV_fseek(NAV_FILE* ptrNavFile, const int32_t offset, const int origin);

// TODO Description NAV_fwrite()
size_t NAV_fwrite(const void* ptrData, size_t size, size_t count,
                  NAV_FILE* ptrNavFile);

// TODO Description NAV_fread()
uint32_t NAV_fread(void* ptrData, size_t size, size_t count,
                   NAV_FILE* ptrNavFile);

// TODO Description NAV_ftell()
int32_t NAV_ftell(NAV_FILE *ptrNavFile);

/***********************************************
** Single purpose file functions
***********************************************/

void checkAndCloseNavFile(NAV_FILE* navFile);

void cfgGetFileHeaderCfgHeader(NAV_FILE* cfgFile,
  NavFileHeader* fileHeader,
                               NavConfigFileHeader* cfgHeader);

void moveCharArraysDown(NAV_FILE* cfgFile, const size_t copySize,
                        const size_t startEntryAtEnd);

/***********************************************
** Structure read/write functions
***********************************************/

// TODO Description fwriteCoordinate()
size_t fwriteCoordinate(const Coordinate* ptrCoord, NAV_FILE* ptrNavFile);

// TODO Description freadCoordinate()
size_t freadCoordinate(Coordinate* ptrCoord, NAV_FILE* ptrNavFile);

// TODO Description fwriteNavFileHeader()
size_t fwriteNavFileHeader(const NavFileHeader* ptrFileHeader,
                           NAV_FILE* ptrNavFile);

// TODO Description freadNavFileHeader()
size_t freadNavFileHeader(NavFileHeader* ptrFileHeader, NAV_FILE* ptrNavFile);

// TODO Description fwriteNavConfigFileHeader()
size_t fwriteNavConfigFileHeader(const NavConfigFileHeader* ptrCfgHeader,
                                 NAV_FILE* ptrNavFile);

// TODO Description freadNavConfigFileHeader()
size_t freadNavConfigFileHeader(NavConfigFileHeader* ptrCfgHeader,
                                NAV_FILE* ptrNavFile);

// TODO Description fwriteNavFileWPListHeader()
size_t fwriteNavFileWPListHeader(const NavFileWPListHeader* ptrWPListHeader,
                                 NAV_FILE* ptrNavFile);

// TODO Description freadNavFileWPListHeader()
size_t freadNavFileWPListHeader(NavFileWPListHeader* ptrWPListHeader,
                                NAV_FILE* ptrNavFile);

// TODO Description fwriteNavDataBlockHeader()
size_t fwriteNavDatablockHeader(const NavDatablockHeader* ptrDataHeader,
                                NAV_FILE* ptrNavFile);

// TODO Description freadNavDataBlockHeader()
size_t freadNavDatablockHeader(NavDatablockHeader* ptrDataHeader,
                               NAV_FILE* ptrNavFile);



#endif // NAVFILES_H

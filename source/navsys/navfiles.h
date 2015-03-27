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
** Defines
***********************************************/
// Define seek parameters for use with NAV_fseek() function calls.
#define NAV_SEEK_SET 0
#define NAV_SEEK_CUR 1
#define NAV_SEEK_END 2



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

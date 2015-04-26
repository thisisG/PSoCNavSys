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

 /*
input:
  NavFileHeader*
output:
  NA
remarks:
  Initialised the structure to default values.
*/
void initNavFileHeader(NavFileHeader* fileHeader);

 /*
input:
  NavConfigFileHeader*
output:
  NA
remarks:
  Initialised the structure to default values.
*/
void initNavConfigFileHeader(NavConfigFileHeader* ptrCfgFileHeader);

 /*
input:
  NavFileWPListHeader*
output:
  NA
remarks:
  Initialised the structure to default values.
*/
void initNavFileWPListHeader(NavFileWPListHeader* WPListHeader);

 /*
input:
  NavDatablockHeader*
output:
  NA
remarks:
  Initialised the structure to default values.
*/
void initNavDatablockHeader(NavDatablockHeader* dataHeader);

/***********************************************
** Basic file interfaces
***********************************************/

 /*
input:
  const char* filename with name of file to open
  const char* mode with open mode for file
output:
  NAV_FILE* to opened file
remarks:
  Interface to platform specific fopen() functions.
*/
NAV_FILE* NAV_fopen(const char* filename, const char* mode);

 /*
input:
  NAV_FILE* pointing to file to close
output:
  NA
remarks:
  Interface to platform specific close() functions.
*/
int NAV_fclose(NAV_FILE* ptrNavFile);

 /*
input:
  NAV_FILE* pointing to file to close
  const int32_t offset in bytes to seek
  const int origin where to seek from
output:
  Returns platform specific values. See documentation for each platform.
remarks:
  Interface to platform specific fseek() functions.
*/
int NAV_fseek(NAV_FILE* ptrNavFile, const int32_t offset, const int origin);

 /*
input:
  const void* ptrData where data is read from
  size_t size to write
  size_t count of items of size to write
  NAV_FILE* pointing to file to write to
output:
  size_t of number of items written. Platform specific.
remarks:
  Interface to platform specific fwrite() functions.
*/
size_t NAV_fwrite(const void* ptrData, size_t size, size_t count,
                  NAV_FILE* ptrNavFile);

 /*
input:
  void* ptrData where data is written to
  size_t size to read
  size_t count of items of size to read
  NAV_FILE* pointing to file to read from
output:
  uint32_t of number of items read. Platform specific.
remarks:
  Interface to platform specific fread() functions.
*/
uint32_t NAV_fread(void* ptrData, size_t size, size_t count,
                   NAV_FILE* ptrNavFile);

 /*
input:
  NAV_FILE* pointing to file to get position of
output:
  int32_t containing current position in file.
remarks:
  Interface to platform specific ftell() functions.
*/
int32_t NAV_ftell(NAV_FILE* ptrNavFile);

/***********************************************
** Single purpose file functions
***********************************************/

 /*
input:
  NAV_FILE* pointing to file to check and close
output:
  NA
remarks:
  Checks if file pointer is NULL, if not close the file and set ptr to NULL.
*/
void checkAndCloseNavFile(NAV_FILE* navFile);

/***********************************************
** Structure read/write functions
***********************************************/

 /*
input:
  const Coordinate* ptrCoord to read from
  NAV_FILE* pointing to file to write to
output:
  size_t number of items written
remarks:
  Serialisation and writing of Coordinate data to file.
*/
size_t fwriteCoordinate(const Coordinate* ptrCoord, NAV_FILE* ptrNavFile);

 /*
input:
  Coordinate* ptrCoord to write to
  NAV_FILE* pointing to file to read from
output:
  size_t number of items read
remarks:
  De-serialisation and reading of Coordinate data to structure.
*/
size_t freadCoordinate(Coordinate* ptrCoord, NAV_FILE* ptrNavFile);

 /*
input:
  const NavFileHeader* ptrFileHeader to read from
  NAV_FILE* pointing to file to write to
output:
  size_t number of items written
remarks:
  Serialisation and writing of NavFileHeader data to file.
*/
size_t fwriteNavFileHeader(const NavFileHeader* ptrFileHeader,
                           NAV_FILE* ptrNavFile);

 /*
input:
  NavFileHeader* ptrFileHeader to write to
  NAV_FILE* pointing to file to read from
output:
  size_t number of items read
remarks:
  De-serialisation and read of Coordinate data to file.
*/
size_t freadNavFileHeader(NavFileHeader* ptrFileHeader, NAV_FILE* ptrNavFile);

 /*
input:
  const NavConfigFileHeader* ptrCfgHeader to read from
  NAV_FILE* ptrNavFile to write data to
output:
  size_t number of items written
remarks:
  Serialisation and writing of NavConfigFileHeader data to file.
*/
size_t fwriteNavConfigFileHeader(const NavConfigFileHeader* ptrCfgHeader,
                                 NAV_FILE* ptrNavFile);

 /*
input:
  NavConfigFileHeader* ptrCfgHeader to write to
  NAV_FILE* ptrNavFile for file to read data from
output:
  size_t number of items read
remarks:
  De-serialisation and reading of NavConfigFileHeader data from file.
*/
size_t freadNavConfigFileHeader(NavConfigFileHeader* ptrCfgHeader,
                                NAV_FILE* ptrNavFile);

 /*
input:
  const NavFileWPListHeader* to read data from
  NAV_FILE* for file to write data to
output:
  size_t number of items written
remarks:
  Serialisation and writing of NavFileWPListHeader data to file
*/
size_t fwriteNavFileWPListHeader(const NavFileWPListHeader* ptrWPListHeader,
                                 NAV_FILE* ptrNavFile);

 /*
input:
  NavFileWPListHeader* for writing data to
  NAV_FILE* for reading data from
output:
  size_t number of items read
remarks:
  De-serialisation and reading of NavFileWPListHeader data from file
*/
size_t freadNavFileWPListHeader(NavFileWPListHeader* ptrWPListHeader,
                                NAV_FILE* ptrNavFile);

 /*
input:
  const NavDatablockHeader* to read data from
  NAV_FILE* to write data to
output:
  size_t number if items written
remarks:
  Serialisation and writing of NavDatablockHeader data to file
*/
size_t fwriteNavDatablockHeader(const NavDatablockHeader* ptrDataHeader,
                                NAV_FILE* ptrNavFile);

 /*
input:
  NavDatablockHeader* to write data to
  NAV_FILE* to read data from
output:
  size_t number of items read
remarks:
  De-serialisation and reading of NavDatablockHeader from file
*/
size_t freadNavDatablockHeader(NavDatablockHeader* ptrDataHeader,
                               NAV_FILE* ptrNavFile);

#endif // NAVFILES_H

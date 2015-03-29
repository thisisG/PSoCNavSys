/*
**
** NavSys library
** URL: https://github.com/thisisG
** Author: Geir Istad (geir dot istad at gmail dot com)
** Licence: MIT
**
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
#include "navfunctions.h"

// Depends on standard types and string operations
#include <stdio.h>
#include <string.h>
#include <sys/types.h>

#ifdef __cplusplus
}
#endif // __cplusplus

/***********************************************
** Initialisation functions
***********************************************/

void initNavWPFileManager(NavWPFileManager* WPFileManager);

void initNavWPHandler(NavWPHandler* WPHandler);

/***********************************************
** WPHandler functions
***********************************************/

// TODO Description WPHandlerOpen()
uint8_t WPHandlerOpen(NavWPHandler* wpHandler, const char* wpFileName);

// TODO Description WPHandlerGetGoal()
void WPHandlerGetGoal(NavWPHandler* wpHandler, Coordinate* wpGoal);

// TODO Description WPHandlerNextWP()
int32_t WPHandlerNextWP(NavWPHandler* wpHandler, Coordinate* nextWP);

// TODO Description WPHandlerSeekWP()
void WPHandlerSeekWP(NavWPHandler* wpHandler, const size_t wpNumber);

/***********************************************
** Helper functions
***********************************************/

void cfgGetFileHeaderCfgHeader(NAV_FILE* cfgFile, NavFileHeader* fileHeader,
                               NavConfigFileHeader* cfgHeader);

void moveCharArraysDown(NAV_FILE* cfgFile, const size_t copySize,
                        const size_t startEntryAtEnd);

void getWPListName(NAV_FILE* cfgFile, const size_t listNumber, char listName[20]);

void getExceptionWPListName(NAV_FILE* cfgFile, const size_t listNumber,
                            char* listName);

/***********************************************
** File generation functions
***********************************************/

size_t generateWPListFile(const char* fileName,
                          const NavDatablockHeader* blockHeaderArray,
                          const Coordinate* coordArray,
                          const size_t arrayLength);

uint8_t makeTemplateCfgFile(const char* fileName, NavVersion sysVersion);

uint8_t makeTemplateWPListFile(const char* fileName, NavVersion sysVersion,
                               NavFileType fileType);

uint8_t appendCoordToWPListFile(const char* fileName, const Coordinate* coord,
                                const NavDataType dataType);

uint8_t addRegularWPListFileToCfgFile(const char* cfgFileName,
                                      const char* WPFileName);

uint8_t addExeptionWPListFileToCfgFile(const char* cfgFileName,
                                      const char* EWPFileName);

uint8_t addWPListToCfgFile(const char* cfgFileName, const char* listFileName,
                           const NavFileType fileType);

#endif // WPHANDLER_H

/* [] END OF FILE */

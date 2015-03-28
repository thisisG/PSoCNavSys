/*
**
** NavSys library
** URL: https://github.com/thisisG
** Author: Geir Istad (geir dot istad at gmail dot com)
** Licence: MIT
**
*/

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus
#include "wphandler.h"

#ifdef __cplusplus
}
#endif // __cplusplus

/***********************************************
** Initialisation functions
***********************************************/

void initNavWPFileManager(NavWPFileManager* WPFileManager)
{
  strncpy(WPFileManager->cfgFileName, DEFAULT_CONFIG_FILE,
          strlen(DEFAULT_CONFIG_FILE));
  strncpy(WPFileManager->wpListFileName, "", strlen(""));
  strncpy(WPFileManager->eWPListFileName, "", strlen(""));
  WPFileManager->ptrCfgFile = NULL;
  WPFileManager->ptrWPList = NULL;
  WPFileManager->numberOfExceptionEntries = 0;
  WPFileManager->currentExceptionEntry = 0;
}

void initNavWPHandler(NavWPHandler* WPHandler)
{
  initNavWPFileManager(&(WPHandler->fileManager));
  initCoordinate(&(WPHandler->wpGoal));
  WPHandler->offsetFirstWPBlock = 0;
  WPHandler->currentWPCount = 0;
  WPHandler->maxWPCount = 0;
}

/***********************************************
** WPHandler functions
***********************************************/

uint8_t WPHandlerOpen(NavWPHandler* wpHandler, const char* wpFileName)
{
  uint8_t statusFileOpen = 0;

  // Check if WP file is currently open, if it is close it
  checkAndCloseNavFile(wpHandler->fileManager.ptrWPList);

  wpHandler->fileManager.ptrWPList = NAV_fopen(wpFileName, "rb");

  if (wpHandler->fileManager.ptrWPList != 0)
  {
    // Able to open the file, update the status and WPBlock offset
    statusFileOpen = 1;
    wpHandler->offsetFirstWPBlock = 0;

    // Read the file header and update the offsetFirstWPBlock.
    NavFileHeader fileHeader;
    initNavFileHeader(&fileHeader);

    wpHandler->offsetFirstWPBlock += SIZE_NAV_FILE_HEADER;
    freadNavFileHeader(&fileHeader, wpHandler->fileManager.ptrWPList);

    // Read the WP List header and update the offsetFirstWPBlock.
    NavFileWPListHeader WPListHeader;
    initNavFileWPListHeader(&WPListHeader);

    wpHandler->offsetFirstWPBlock += SIZE_NAV_FILE_WP_LIST_HEADER;
    freadNavFileWPListHeader(&WPListHeader, wpHandler->fileManager.ptrWPList);

    // Set the wpGoal in the wpHandler
    wpHandler->wpGoal = WPListHeader.endCoordinate;

    // Set the current wp counter to 0 to show that we are at the start of the
    // list and that the WP handler is ready for the first call to get the next
    // WP in the list via the WPHandlerNextWP() function.
    wpHandler->currentWPCount = 0;

    // Set the maximum wp count to number of entries in list.
    wpHandler->maxWPCount = WPListHeader.numberOfEntries;
  }

  return statusFileOpen;
}

void WPHandlerGetGoal(NavWPHandler* wpHandler, Coordinate* wpGoal)
{
  (*wpGoal) = wpHandler->wpGoal;
}

int32_t WPHandlerNextWP(NavWPHandler* wpHandler, Coordinate* nextWP)
{
  // Set the return value to -1 by default.
  // The return value should be -1 if we have no more waypoints in the list, if
  // not it should be the number of the WP we are currently at.
  int32_t returnCount = -1;

  // Store local copies of variables for clarity
  size_t WPCount = wpHandler->currentWPCount;
  size_t WPMaxCount = wpHandler->maxWPCount;

  // Check if there are more wps left in the list
  // WPCount goes from 0 to N-1, WPMaxCount is N, when N-1 + 1 == N we are out
  // of WPs to use.
  if ((WPCount + 1) <= WPMaxCount)
  {
    // The file should already be in a position for reading the next WP.
    // Read the data block header.
    NavDatablockHeader dataHeader;
    initNavDatablockHeader(&dataHeader);
    freadNavDatablockHeader(&dataHeader, wpHandler->fileManager.ptrWPList);

    // Read the coordinate.
    Coordinate coord;
    initCoordinate(&coord);
    freadCoordinate(&coord, wpHandler->fileManager.ptrWPList);

    WPCount++;
    (*nextWP) = coord;
    returnCount = WPCount;

    // Update the external variables.
    wpHandler->currentWPCount = WPCount;
  }
  else
  {
    // Do nothing, return value already -1 to show there are no more waypoints.
  }
  return returnCount;
}

void WPHandlerSeekWP(NavWPHandler* wpHandler, const size_t wpNumber)
{
  // We assume that we start at the origin and we need to find the offset for
  // wpNumber of waypoints we want to seek to.
  // The total offset is then calculated from the offset stored in
  // wpHandler->offsetFirstWPBlock plus
  // wpNumber*(sizeof(NavDatablockHeader) + sizeof(Coordinate))
  size_t startOffset = wpHandler->offsetFirstWPBlock;

  size_t wpDataOffset = wpNumber
      * (SIZE_NAV_DATABLOCK_HEADER + SIZE_COORDINATE);

  size_t totalOffset = startOffset + wpDataOffset;

  NAV_fseek(wpHandler->fileManager.ptrWPList, totalOffset, NAV_SEEK_SET);
  wpHandler->currentWPCount = wpNumber;
}

/***********************************************
** Helper functions
***********************************************/

void cfgGetFileHeaderCfgHeader(NAV_FILE* cfgFile, NavFileHeader* fileHeader,
                               NavConfigFileHeader* cfgHeader)
{
  freadNavFileHeader(fileHeader, cfgFile);
  freadNavConfigFileHeader(cfgHeader, cfgFile);
}

// Test
// VS2013: OK
// PSoC: OK
void moveCharArraysDown(NAV_FILE* cfgFile, const size_t copySize,
                        const size_t startEntryAtEnd)
{
  size_t leftToCopy = copySize;
  char charBuffer[20];
  int32_t bytesTwoCharArrays = 2 * sizeof(char[20]);
  NAV_fseek(cfgFile, startEntryAtEnd, NAV_SEEK_SET);
  // Copy last entry to next position in file, then second last entry to the
  // previous position of the last entry etc until copySize = 0.
  while (leftToCopy > 0)
  {
    NAV_fread(charBuffer, sizeof(char[20]), 1, cfgFile);
    NAV_fwrite(charBuffer, sizeof(char[20]), 1, cfgFile);
    NAV_fseek(cfgFile, -bytesTwoCharArrays, NAV_SEEK_CUR);
    leftToCopy -= sizeof(char[20]);
  }
}

void getWPListName(NAV_FILE* cfgFile, const size_t listNumber, char* listName)
{
  // Calculate offset and seek to position
  size_t baseOffset = SIZE_NAV_FILE_HEADER + SIZE_NAV_FILE_WP_LIST_HEADER;
  size_t totalOffset = baseOffset + (listNumber * sizeof(char[20]));
  NAV_fseek(cfgFile, totalOffset, NAV_SEEK_SET);

  // Read the list name directly into listName
  NAV_fread(listName, sizeof(char[20]), 1, cfgFile);
}

void getExceptionWPListName(NAV_FILE* cfgFile, const size_t listNumber,
                            char* listName)
{
  // See to the cfg header and read the cfg header
  NAV_fseek(cfgFile, SIZE_NAV_FILE_HEADER, NAV_SEEK_SET);
  NavConfigFileHeader cfgHeader;
  initNavConfigFileHeader(&cfgHeader);
  freadNavConfigFileHeader(&cfgHeader, cfgFile);

  // Find offset, seek to it and read the exception list name
  size_t regularWPListCount = cfgHeader.numberOfWPLists;
  size_t offset = (regularWPListCount + listNumber) * sizeof(char[20]);
  NAV_fseek(cfgFile, offset, NAV_SEEK_CUR);
  NAV_fread(listName, sizeof(char[20]), 1, cfgFile);
}

/***********************************************
** File generation functions
***********************************************/

size_t generateWPListFile(const char* fileName,
                          const NavDatablockHeader* blockHeaderArray,
                          const Coordinate* coordArray,
                          const size_t arrayLength)
{
  size_t dataItemsWritten = 0;
  NAV_FILE* navFile = NAV_fopen(fileName, "wb");

  if (navFile != NULL)
  {
    NavFileHeader fileHeader;
    initNavFileHeader(&fileHeader);

    fileHeader.fileType = WAYPOINT_LIST_FILE;
    fileHeader.fileVersion = NAV_VERSION_1;
    fileHeader.nextHeaderSize = 0; // Currently not implemented.

    NavFileWPListHeader listHeader;
    initNavFileWPListHeader(&listHeader);

    listHeader.numberOfEntries = arrayLength;
    listHeader.startCoordinate = coordArray[0];
    listHeader.endCoordinate = coordArray[arrayLength - 1];
    listHeader.nextHeaderSize = 0; // Currently not implemented.

    fwriteNavFileHeader(&fileHeader, navFile);

    fwriteNavFileWPListHeader(&listHeader, navFile);

    size_t i = 0;
    for (i = 0; i < arrayLength; i++)
    {
      dataItemsWritten
          += fwriteNavDatablockHeader(&(blockHeaderArray[i]), navFile);
      dataItemsWritten += fwriteCoordinate(&(coordArray[i]), navFile);
    }

    NAV_fclose(navFile);
    navFile = NULL;
  }
  else
  {
    // TODO LOG ERROR
  }
  return dataItemsWritten;
}

uint8_t makeTemplateCfgFile(const char* fileName, NavVersion sysVersion)
{
  uint8_t success = 0;
  NAV_FILE* cfgFile = NAV_fopen(fileName, "wb");

  if (cfgFile != NULL)
  {
    success = 1;
    NavFileHeader fileHeader;
    initNavFileHeader(&fileHeader);
    fileHeader.fileType = CONFIG_FILE;
    fileHeader.fileVersion = sysVersion;
    fwriteNavFileHeader(&fileHeader, cfgFile);

    NavConfigFileHeader cfgHeader;
    initNavConfigFileHeader(&cfgHeader);
    fwriteNavConfigFileHeader(&cfgHeader, cfgFile);

    NAV_fclose(cfgFile);
  }
  else
  {
    // Do nothing, ptr was NULL and return value is already 0
  }

  return success;
}

uint8_t makeTemplateWPListFile(const char* fileName,
                               const NavVersion sysVersion,
                               const NavFileType fileType)
{
  uint8_t success = 0;

  NAV_FILE* wpFile = NAV_fopen(fileName, "wb");

  if (wpFile != NULL)
  {
    success = 1;
    NavFileHeader fileHeader;
    initNavFileHeader(&fileHeader);
    fileHeader.fileType = fileType;
    fileHeader.fileVersion = sysVersion;
    fileHeader.nextHeaderSize = 0; // Not implemented yet
    fwriteNavFileHeader(&fileHeader, wpFile);

    NavFileWPListHeader wpListHeader;
    initNavFileWPListHeader(&wpListHeader);
    // All parameters are zero since there are no entries yet.
    fwriteNavFileWPListHeader(&wpListHeader, wpFile);

    NAV_fclose(wpFile);
  }
  else
  {
    // Do nothing, ptr was NULL and return value is already 0
  }

  return success;
}

uint8_t appendCoordToWPListFile(const char* fileName, const Coordinate* coord,
                                const NavDataType dataType)
{
  uint8_t success = 0;

  // Open for both read and write
  NAV_FILE* wpFile = NAV_fopen(fileName, "r+b");

  if (wpFile != NULL)
  {
    success = 1;

    NavFileHeader fileHeader;
    initNavFileHeader(&fileHeader);
    freadNavFileHeader(&fileHeader, wpFile);

    NavFileWPListHeader wpFileHeader;
    initNavFileWPListHeader(&wpFileHeader);

    // Check if this is the first WP added, if so add it to the data structure
    // as the starting point
    if (wpFileHeader.numberOfEntries == 0)
    {
      wpFileHeader.startCoordinate = (*coord);
    }
    // Assign the added WP as the goal in the data structure
    wpFileHeader.endCoordinate = (*coord);

    // Store the current WP count for inserting new coordinate later
    uint32_t oldWPCount = wpFileHeader.numberOfEntries;

    // Update the WP count
    wpFileHeader.numberOfEntries += 1;
    // Seek back and update the wpHeader stored in file
    NAV_fseek(wpFile, -SIZE_NAV_FILE_WP_LIST_HEADER, NAV_SEEK_CUR);
    fwriteNavFileWPListHeader(&wpFileHeader, wpFile);

    // Seek to the insert position for the coordinate we are after
    int32_t seekPosition = oldWPCount
        * (SIZE_NAV_DATABLOCK_HEADER + SIZE_COORDINATE);
    NAV_fseek(wpFile, seekPosition, NAV_SEEK_CUR);

    // Write a datablockheader and coordinate
    NavDatablockHeader dataHeader;
    initNavDatablockHeader(&dataHeader);
    dataHeader.dataVersion = fileHeader.fileVersion;
    dataHeader.dataType = dataType;
    dataHeader.nextDataSize = 0; // Yet to be implemented
    fwriteNavDatablockHeader(&dataHeader, wpFile);
    fwriteCoordinate(coord, wpFile);

    NAV_fclose(wpFile);
  }
  else
  {
    // Do nothing, ptr was NULL and return value is already 0
  }

  return success;
}

uint8_t addRegularWPListFileToCfgFile(const char* cfgFileName,
                                      const char* WPFileName)
{
  uint8_t operationSuccess = 0;

  // Open the cfg file for read and write mode
  NAV_FILE* cfgFile = NAV_fopen(cfgFileName, "r+b");

  if (cfgFile != 0)
  {
    operationSuccess = 1;
    // Get the count of items stored in the list
    NavFileHeader cfgFileHeader;
    initNavFileHeader(&cfgFileHeader);
    NavConfigFileHeader cfgHeader;
    initNavConfigFileHeader(&cfgHeader);
    cfgGetFileHeaderCfgHeader(cfgFile, &cfgFileHeader, &cfgHeader);

    size_t WPListCount = cfgHeader.numberOfWPLists;
    size_t newWPListCount = WPListCount + 1;
    size_t ExceptionWPListCount = cfgHeader.numberOfExeptionWPLists;

    // Update the header to the new WPListcount and write it to file
    cfgHeader.numberOfWPLists = newWPListCount;
    NAV_fseek(cfgFile, -SIZE_NAV_CFG_FILE_HEADER, NAV_SEEK_CUR);
    fwriteNavConfigFileHeader(&cfgHeader, cfgFile);

    // Make room for the new char[20] array in the cfg file.
    // Find the position we want to insert the new char[20]array
    size_t baseOffset = SIZE_NAV_FILE_HEADER + SIZE_NAV_CFG_FILE_HEADER;
    // The insert position is after the base offset and after the last char[20]
    // array of WPList filenames, before ExceptionWPList names begin.
    size_t insertPosition = baseOffset + WPListCount * sizeof(char[20]);

    // Find the last character array position currently in the in the file,
    // taking into account that the file might be longer than the number of
    // character arrays and headers in it, so avoiding using sizeof() for this.
    size_t lastArrayStartPosition = 0;
    if ((WPListCount + ExceptionWPListCount) == 0)
    {
      lastArrayStartPosition = baseOffset;
    }
    else
    {
      lastArrayStartPosition = baseOffset
          + (WPListCount + ExceptionWPListCount - 1) * sizeof(char[20]);
    }

    // Find the total size of what we want to copy
    size_t copySize = insertPosition - lastArrayStartPosition;

    // Copy each location sizeof(char[20]) bytes down in the file, starting at
    // the end
    moveCharArraysDown(cfgFile, copySize, lastArrayStartPosition);

    // Go to insert position and insert the new char array.
    NAV_fseek(cfgFile, insertPosition, NAV_SEEK_SET);
    NAV_fwrite(WPFileName, sizeof(char[20]), 1, cfgFile);

    // Close the file.
    NAV_fclose(cfgFile);
  }

  return operationSuccess;
}

uint8_t addExeptionWPListFileToCfgFile(const char* cfgFileName,
                                       const char* EWPFileName)
{
  uint8_t success = 0;

  NAV_FILE* cfgFile = NAV_fopen(cfgFileName, "r+b");
  if (cfgFile != NULL)
  {
    success = 1;

    // Get the count of items stored in the list
    NavFileHeader cfgFileHeader;
    initNavFileHeader(&cfgFileHeader);
    NavConfigFileHeader cfgHeader;
    initNavConfigFileHeader(&cfgHeader);
    cfgGetFileHeaderCfgHeader(cfgFile, &cfgFileHeader, &cfgHeader);

    size_t ExceptionWPListCount = cfgHeader.numberOfExeptionWPLists;
    size_t newExWPListCount = ExceptionWPListCount + 1;
    size_t WPListCount = cfgHeader.numberOfWPLists;

    // Update the header to the new newExWPListCount and write it to file
    cfgHeader.numberOfExeptionWPLists = newExWPListCount;
    NAV_fseek(cfgFile, -SIZE_NAV_CFG_FILE_HEADER, NAV_SEEK_CUR);
    fwriteNavConfigFileHeader(&cfgHeader, cfgFile);

    // Add the new char[20] array with new exception list to the end of the
    // current list of character arrays.
    size_t totalCount = WPListCount + ExceptionWPListCount;
    size_t insertPosition = 0;
    // Check if it is the first list added, if so the offset from current
    // position is 0
    if (totalCount == 0)
    {
      insertPosition = 0;
    }
    else
    {
      insertPosition = (totalCount - 1) * sizeof(char[20]);
    }

    // Seek to the insertposition and write the char array
    NAV_fseek(cfgFile, insertPosition, NAV_SEEK_CUR);
    NAV_fwrite(EWPFileName, sizeof(char[20]), 1, cfgFile);

    // Close the file
    NAV_fclose(cfgFile);
  }
  else
  {
    // For completeness. Returnvalue is already 0.
  }

  return success;
}

uint8_t addWPListToCfgFile(const char* cfgFileName, const char* listFileName,
                           const NavFileType fileType)
{
  uint8_t success = 0;
  if (fileType == WAYPOINT_LIST_FILE)
  {
    success = addRegularWPListFileToCfgFile(cfgFileName, listFileName);
  }
  else if (fileType == EXCEPTION_WAYPOINT_LIST_FILE)
  {
    success = addExeptionWPListFileToCfgFile(cfgFileName, listFileName);
  }
  else
  {
    // Case for completion, return value already 0.
  }
  return success;
}
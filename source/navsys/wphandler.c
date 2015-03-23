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
  strncpy(WPFileManager->cfgFileName, "", strlen(""));
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
  zeroCoordinate(&(WPHandler->wpGoal));
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

  // If file pointer is not NULL, close currently open file.
  if (wpHandler->fileManager.ptrWPList != NULL)
  {
    NAV_fclose(wpHandler->fileManager.ptrWPList);
    wpHandler->fileManager.ptrWPList = NULL;
  }

  // Set the file pointer to zero
  wpHandler->fileManager.ptrWPList = 0;

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
    // list.
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

size_t WPHandlerNextWP(NavWPHandler* wpHandler, Coordinate* nextWP)
{
  // Set the return value to 0 by default.
  // The return value should be 0 if we have no more waypoints in the list, if
  // not it should be the number of the WP we are currently at.
  size_t returnCount = 0;

  // Store local copies of variables for clarity
  size_t WPCount = wpHandler->currentWPCount;
  size_t WPMaxCount = wpHandler->maxWPCount;

  // Check if there are more wps left in the list
  if ((WPCount + 1) <= WPMaxCount)
  {
    // The file should already be in a position for reading the next WP.
    // Read the data block header.
    NavDatablockHeader dataHeader;
    initNavDatablockHeader(&dataHeader);
    freadNavDatablockHeader(&dataHeader, wpHandler->fileManager.ptrWPList);

    // Read the coordinate.
    Coordinate coord;
    zeroCoordinate(&coord);
    freadCoordinate(&coord, wpHandler->fileManager.ptrWPList);

    (*nextWP) = coord;
    WPCount++;
    returnCount = WPCount;
  }
  else
  {
    // Do nothing, return value already 0.
  }

  // Update the external variables.
  wpHandler->currentWPCount = WPCount;
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
    // LOG ERROR
  }
  return dataItemsWritten;
}

/* [] END OF FILE */

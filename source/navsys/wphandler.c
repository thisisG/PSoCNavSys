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

uint8_t WPHandlerOpen(NavWPHandler* wpHandler, char* wpFileName)
{
  uint8_t statusFileOpen = 0;

  // Close any currently open file assigned to the pointer.
  NAV_fclose(wpHandler->fileManager.ptrWPList);

  // Set the file pointer to zero
  wpHandler->fileManager.ptrWPList = 0;

  wpHandler->fileManager.ptrWPList = NAV_fopen(wpFileName, "rb");

  if (wpHandler->fileManager.ptrWPList != 0)
  {
    // Able to open the file, update the status.
    statusFileOpen = 1;

    // Read the file header and update the offsetFirstWPBlock.
    NavFileHeader fileHeader;
    initNavFileHeader(&fileHeader);

    NAV_fread(&fileHeader, 1, sizeof(fileHeader),
              wpHandler->fileManager.ptrWPList);

    wpHandler->offsetFirstWPBlock = sizeof(fileHeader);

    // Read the WP List header and update the offsetFirstWPBlock.
    NavFileWPListHeader WPListHeader;
    initNavFileWPListHeader(&WPListHeader);

    NAV_fread(&WPListHeader, 1,
      fileHeader.nextHeaderSize, wpHandler->fileManager.ptrWPList);

    wpHandler->offsetFirstWPBlock += fileHeader.nextHeaderSize;

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

    NAV_fread(&dataHeader, sizeof(dataHeader), 1,
              wpHandler->fileManager.ptrWPList);

    // Read the coordinate.
    Coordinate coord;
    zeroCoordinate(&coord);

    NAV_fread(&coord, sizeof(coord), 1, wpHandler->fileManager.ptrWPList);

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
      * (sizeof(NavDatablockHeader) + sizeof(Coordinate));

  size_t totalOffset = startOffset + wpDataOffset;

  NAV_fseek(wpHandler->fileManager.ptrWPList, totalOffset, NAV_SEEK_SET);
}

/* [] END OF FILE */

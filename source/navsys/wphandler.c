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

  // Check if WP file is currently open, if it is close it
  checkAndCloseNavFile(wpHandler->fileManager.ptrWPList);

  // Open the WP list for reading
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

    // Set the current wp counter to -1 to show that we are at the start of the
    // list and that the WP handler is ready for the first call to get the next
    // WP in the list via the WPHandlerNextWP() function.
    wpHandler->currentWPCount = -1;

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
    // TODO LOG ERROR
  }

  return dataItemsWritten;
}

size_t makeTemplateCfgFile(const char* fileName, NavVersion sysVersion)
{
  size_t itemsWritten = 0;
  NAV_FILE *cfgFile = NAV_fopen(fileName, "wb");

  NavFileHeader fileHeader;
  initNavFileHeader(&fileHeader);
  fileHeader.fileType = CONFIG_FILE;
  fileHeader.fileVersion = sysVersion;
  itemsWritten += fwriteNavFileHeader(&fileHeader, cfgFile);

  NavConfigFileHeader cfgHeader;
  initNavConfigFileHeader(&cfgHeader);
  itemsWritten += fwriteNavConfigFileHeader(&cfgHeader, cfgFile);

  NAV_fclose(cfgFile);

  return itemsWritten;
}

uint8_t addWPListFileToCfgFile(const char* cfgFileName, const char* WPFileName)
{
  uint8_t operationSuccess = 0;

  // Open the cfg file for read and write mode
  NAV_FILE *cfgFile = NAV_fopen(cfgFileName, "r+b");

  if (cfgFile != 0)
  {
    operationSuccess = 1;
    // Get the count of items stored in the list
    NavFileHeader cfgFileHeader;
    initNavFileHeader(&cfgFileHeader);
    freadNavFileHeader(&cfgFileHeader, cfgFile);
    NavConfigFileHeader cfgHeader;
    initNavConfigFileHeader(&cfgHeader);
    freadNavConfigFileHeader(&cfgHeader, cfgFile);

    size_t WPListCount = cfgHeader.numberOfWPLists;
    size_t newWPListCount = WPListCount + 1;
    size_t ExceptionWPListCount = cfgHeader.numberOfExeptionWPLists;

    // Update the header to the new WPListcount and write it to file
    cfgHeader.numberOfWPLists = newWPListCount;
    NAV_fseek(cfgFile, -SIZE_NAV_CFG_FILE_HEADER, NAV_SEEK_CUR);
    fwriteNavConfigFileHeader(&cfgHeader, cfgFile);

    // Make room for the new char[20] array  in the cfg file.
    // Find the position we want to insert the new char[20]array
    size_t baseOffset = SIZE_NAV_FILE_HEADER + SIZE_NAV_CFG_FILE_HEADER;
    // The insert position is after the base offset and after the last char[20]
    // array of WPList filenames, before ExceptionWPList names begin.
    size_t insertPosition = baseOffset + WPListCount*sizeof(char[20]);

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
    size_t copySize = lastArrayStartPosition - insertPosition;

    // Copy each location sizeof(char[20]) bytes down in the file, starting at
    // the end
    NAV_fseek(cfgFile, lastArrayStartPosition, NAV_SEEK_SET);
    char charBuffer[20];
    while (copySize > 0)
    {
      NAV_fread(charBuffer, sizeof(char[20]), 1, cfgFile);
      NAV_fwrite(charBuffer, sizeof(char[20]), 1, cfgFile);
      NAV_fseek(cfgFile, ((-2) * (sizeof(char[20]))), NAV_SEEK_CUR);
      copySize -= sizeof(char[20]);
    }

    // Go to seek position and insert the new char array.
    NAV_fseek(cfgFile, insertPosition, NAV_SEEK_SET);
    NAV_fwrite(WPFileName, sizeof(char[20]), 1, cfgFile);

    // Close the file.
    NAV_fclose(cfgFile);
  }

  return operationSuccess;
}

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

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

#include "wphandler.h"

#ifdef __cplusplus
}
#endif // __cplusplus

uint8_t WPHandlerOpen(NavWPHandler* wpHandler, char* wpFileName)
{
    uint8_t statusFileOpen = 0;

    // Close any currently open file assigned to the pointer
    NAV_fclose(wpHandler->fileManager.ptrWPList);


    // Set the file pointer to zero
    wpHandler->fileManager.ptrWPList = 0;

    wpHandler->fileManager.ptrWPList = NAV_fopen(wpFileName, "rb");

    if (wpHandler->fileManager.ptrWPList != 0)
    {
        // Able to open the file, update the status
        statusFileOpen = 1;

        // Read the file header and update the offsetFirstWPBlock
        NavFileHeader fileHeader = { 0, 0, 0 };
#ifdef __GNUC__
        FS_Read(wpHandler->fileManager.ptrWPList, &fileHeader,
                sizeof(fileHeader));
#else // File operations for windows

#endif // __GNUC__

        wpHandler->offsetFirstWPBlock = sizeof(fileHeader);

        // Read the WP List header and update the offsetFirstWPBlock
        NavFileWPListHeader WPListHeader = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
#ifdef __GNUC__
        FS_Read(wpHandler->fileManager.ptrWPList, &WPListHeader,
                fileHeader.headerBlockSize);
#else // File operations for windows

#endif // __GNUC__

        wpHandler->offsetFirstWPBlock += fileHeader.headerBlockSize;

        // Set the wpGoal in the wpHandler
        wpHandler->wpGoal = WPListHeader.endCoordinate;

        // Set the current wp counter to 0 to show that we are at the start of
        // the list
        wpHandler->currentWPCount = 0;

        // Set the maximum wp count to number of entries in list
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
    // Set the return value to 0 by default
    // The return value should be 0 if we have no more waypoints in the list, if
    // not it should be the number of the WP we are currently at.
    size_t returnCount = 0;

    // Store local copies of variables for clarity
    size_t WPCount = wpHandler->currentWPCount;
    size_t WPMaxCount = wpHandler->maxWPCount;

    // Check if there are more wps left in the list
    if ((WPCount + 1) <= WPMaxCount)
    {
        // The file should already be in a position for reading the first WP
        // Read the data block header
        NavDataBlockHeader dataHeader;
#ifdef __GNUC__
        FS_Read(wpHandler->fileManager.ptrWPList, &dataHeader,
                sizeof(dataHeader));
#else  // File operations for windows
        fread(&dataHeader, sizeof(dataHeader), 1,
              wpHandler->fileManager.ptrWPList);
#endif // __GNUC__

        // Read the coordinate
        Coordinate coord;
#ifdef __GNUC__
        FS_Read(wpHandler->fileManager.ptrWPList, &coord,
                dataHeader.dataBlockSize);
#else  // File operations for windows
        fread(&coord, sizeof(coord), 1, wpHandler->fileManager.ptrWPList);
#endif // __GNUC__

        (*nextWP) = coord;
        WPCount++;
        returnCount = WPCount;
    }
    else
    {
        // Do nothing, return value already 0.
    }

    // Update the external variables
    wpHandler->currentWPCount = WPCount;
    return returnCount;
}

void WPHandlerSeekWP(NavWPHandler* wpHandler, const size_t wpNumber)
{
    // We assume that we start at the origin and we need to find the offset for
    // wpNumber of waypoints we want to seek to.
    // The total offset is then calculated from the offset stored in
    // wpHandler->offsetFirstWPBlock plus
    // wpNumber*(sizeof(NavFileHeader) + sizeof(Coordinate))
    size_t startOffset = wpHandler->offsetFirstWPBlock;

    size_t wpDataOffset = wpNumber
        * (sizeof(NavDataBlockHeader) + sizeof(Coordinate));

    size_t totalOffset = startOffset + wpDataOffset;

#ifdef __GNUC__
    FS_FSeek(wpHandler->fileManager.ptrWPList, FS_SEEK_SET, totalOffset);
#else  // File operations for windows
    fseek(wpHandler->fileManager.ptrWPList, totalOffset, SEEK_SET);
#endif // __GNUC__
}

/* [] END OF FILE */

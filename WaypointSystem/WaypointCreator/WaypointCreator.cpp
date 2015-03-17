// WaypointCreator.cpp : Defines the entry point for the console application.
//

#include <iostream>
#include <iomanip>
#include <stdio.h>
#include <tchar.h>

#ifdef __cplusplus
extern "C" {
#endif
#include <stdio.h>
#include "navsys\wphandler.h"
#include "navsys\navTypes.h"
#include "navsys\navsys.h"

#ifdef __cplusplus
}
#endif
#include "navmanager\gpsemulator.h"

NAV_FILE* somefile;
char filename[20] = "wplist1.wp";

int _tmain(int argc, _TCHAR* argv[])
{
  NavWPHandler navHandler;

  strncpy(navHandler.fileManager.cfgFileName, "sometext", strlen("sometext"));

  somefile = NAV_fopen(filename, "wb");

  size_t totalSize = 0;

  NavFileHeader fileHeader;
  NavFileWPListHeader WPListHeader;

  fileHeader.fileType = WAYPOINT_LIST_FILE;
  fileHeader.fileVersion = (NavVersion)255;
  fileHeader.nextHeaderSize = sizeof(WPListHeader);

  std::cout << "sizeof(WPListHeader): " << sizeof(WPListHeader) << std::endl;

  Coordinate coordArray[1];

  for (size_t i = 0; i < (sizeof(coordArray) / sizeof(coordArray[0])); i++)
  {
    zeroCoordinate(&coordArray[i]);
    // coordArray[i].dLatitude = 0xFFFF;
    coordArray[i].dLatitude = 1;
    coordArray[i].dLongitude = 2 * i;
    coordArray[i].mLatitude = 0;
    coordArray[i].mLongitude = 0;
    coordArray[i].priority = 20 * i;
  }

  WPListHeader.startCoordinate = coordArray[0];
  WPListHeader.endCoordinate = coordArray[0];
  WPListHeader.numberOfEntries = (sizeof(coordArray) / sizeof(coordArray[0]));
  WPListHeader.nextHeaderSize = sizeof(coordArray[0]);

  somefile = fopen(filename, "wb");

  fwrite(&fileHeader, sizeof(fileHeader), 1, somefile);
  totalSize += sizeof(fileHeader);
  std::cout << "totalSize after fileHeader: " << totalSize << std::endl;

  fwrite(&WPListHeader, sizeof(WPListHeader), 1, somefile);
  totalSize += sizeof(WPListHeader);
  std::cout << "totalSize after NavFileWPListHeader: " << totalSize
            << std::endl;

  for (size_t i = 0; i < (sizeof(coordArray) / sizeof(coordArray[0])); i++)
  {
    fwrite(&coordArray[i], sizeof(Coordinate), 1, somefile);
    std::cout << "Wrote:" << std::endl;
    printCoordData(&coordArray[i]);
    totalSize += sizeof(Coordinate);
  }
  std::cout << "Expected file size: " << totalSize << std::endl;
  fclose(somefile);

  std::cout << "sizeof(fileheader.fileVersion): "
            << sizeof(fileHeader.fileVersion) << std::endl;

  return 0;
}

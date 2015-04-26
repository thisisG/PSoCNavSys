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

#include "navfiles.h"

#ifdef __cplusplus
}
#endif // __cplusplus

/***********************************************
** Initialisation function implementations
***********************************************/

void initNavFileHeader(NavFileHeader* ptrFileHeader)
{
  ptrFileHeader->fileType = INVALID_FILE_TYPE;
  ptrFileHeader->fileVersion = INVALID_NAV_VERSION;
  ptrFileHeader->nextHeaderSize = 0;
}

void initNavConfigFileHeader(NavConfigFileHeader* ptrCfgFileHeader)
{
  ptrCfgFileHeader->numberOfWPLists = 0;
  ptrCfgFileHeader->currentWPList = 1;
  ptrCfgFileHeader->numberOfExeptionWPLists = 0;
  ptrCfgFileHeader->currentExeptionWPList = 1;
}

void initNavFileWPListHeader(NavFileWPListHeader* ptrWPListHeader)
{
  initCoordinate(&(ptrWPListHeader->startCoordinate));
  initCoordinate(&(ptrWPListHeader->endCoordinate));
  ptrWPListHeader->numberOfEntries = 0;
  ptrWPListHeader->nextHeaderSize = 0;
}

void initNavDatablockHeader(NavDatablockHeader* ptrDataHeader)
{
  ptrDataHeader->dataType = INVALID_DATA_TYPE;
  ptrDataHeader->dataVersion = INVALID_NAV_VERSION;
  ptrDataHeader->nextDataSize = 0;
}

/***********************************************
** Basic file interfaces
***********************************************/

NAV_FILE* NAV_fopen(const char* filename, const char* mode)
{
#ifdef __GNUC__
  return FS_FOpen(filename, mode);
#else
  return fopen(filename, mode);
#endif // __GNUC__
}

int NAV_fclose(NAV_FILE* ptrNavFile)
{
#ifdef __GNUC__
  return FS_FClose(ptrNavFile);
#else
  return fclose(ptrNavFile);
#endif // __GNUC__
}

int NAV_fseek(NAV_FILE* ptrNavFile, const int32_t offset, const int origin)
{
#ifdef __GNUC__
  return FS_FSeek(ptrNavFile, offset, origin);
#else
  return fseek(ptrNavFile, offset, origin);
#endif // __GNUC__
}

size_t NAV_fwrite(const void* ptrData, size_t size, size_t count,
                  NAV_FILE* ptrNavFile)
{
#ifdef __GNUC__
  return FS_FWrite(ptrData, size, count, ptrNavFile);
#else
  return fwrite(ptrData, size, count, ptrNavFile);
#endif // __GNUC__
}

uint32_t NAV_fread(void* ptrData, size_t size, size_t count,
                   NAV_FILE* ptrNavFile)
{
#ifdef __GNUC__
  return FS_FRead(ptrData, size, count, ptrNavFile);
#else
  return fread(ptrData, size, count, ptrNavFile);
#endif // __GNUC__
}

int32_t NAV_ftell(NAV_FILE* ptrNavFile)
{
#ifdef __GNUC__
  return FS_FTell(ptrNavFile);
#else
  return ftell(ptrNavFile);
#endif // __GNUC__
}

/***********************************************
** Single purpose file functions
***********************************************/

void checkAndCloseNavFile(NAV_FILE* navFile)
{
  if (navFile != NULL)
  {
    NAV_fclose(navFile);
    navFile = NULL;
  }
}

/***********************************************
** Structure read/write functions
***********************************************/

size_t fwriteCoordinate(const Coordinate* ptrCoord, NAV_FILE* ptrNavFile)
{
  size_t itemsWritten = 0;

  itemsWritten += NAV_fwrite(&(ptrCoord->dLongitude),
                             sizeof((ptrCoord->dLongitude)), 1, ptrNavFile);

  itemsWritten += NAV_fwrite(&(ptrCoord->mLongitude),
                             sizeof((ptrCoord->mLongitude)), 1, ptrNavFile);

  itemsWritten += NAV_fwrite(&(ptrCoord->dLatitude),
                             sizeof((ptrCoord->dLatitude)), 1, ptrNavFile);

  itemsWritten += NAV_fwrite(&(ptrCoord->mLatitude),
                             sizeof((ptrCoord->mLatitude)), 1, ptrNavFile);

  itemsWritten += NAV_fwrite(&(ptrCoord->priority),
                             sizeof((ptrCoord->priority)), 1, ptrNavFile);

  return itemsWritten;
}

size_t freadCoordinate(Coordinate* ptrCoord, NAV_FILE* ptrNavFile)
{
  size_t itemsRead = 0;

  itemsRead += NAV_fread(&(ptrCoord->dLongitude),
                         sizeof((ptrCoord->dLongitude)), 1, ptrNavFile);

  itemsRead += NAV_fread(&(ptrCoord->mLongitude),
                         sizeof((ptrCoord->mLongitude)), 1, ptrNavFile);

  itemsRead += NAV_fread(&(ptrCoord->dLatitude), sizeof((ptrCoord->dLatitude)),
                         1, ptrNavFile);

  itemsRead += NAV_fread(&(ptrCoord->mLatitude), sizeof((ptrCoord->mLatitude)),
                         1, ptrNavFile);

  itemsRead += NAV_fread(&(ptrCoord->priority), sizeof((ptrCoord->priority)), 1,
                         ptrNavFile);

  return itemsRead;
}

size_t fwriteNavFileHeader(const NavFileHeader* ptrFileHeader,
                           NAV_FILE* ptrNavFile)
{
  size_t itemsWritten = 0;

  itemsWritten += NAV_fwrite(&(ptrFileHeader->fileType),
                             sizeof((ptrFileHeader->fileType)), 1, ptrNavFile);

  itemsWritten
      += NAV_fwrite(&(ptrFileHeader->fileVersion),
                    sizeof((ptrFileHeader->fileVersion)), 1, ptrNavFile);

  itemsWritten
      += NAV_fwrite(&(ptrFileHeader->nextHeaderSize),
                    sizeof((ptrFileHeader->nextHeaderSize)), 1, ptrNavFile);

  return itemsWritten;
}

size_t freadNavFileHeader(NavFileHeader* ptrFileHeader, NAV_FILE* ptrNavFile)
{
  size_t itemsRead = 0;

  itemsRead += NAV_fread(&(ptrFileHeader->fileType),
                         sizeof((ptrFileHeader->fileType)), 1, ptrNavFile);

  itemsRead += NAV_fread(&(ptrFileHeader->fileVersion),
                         sizeof((ptrFileHeader->fileVersion)), 1, ptrNavFile);

  itemsRead
      += NAV_fread(&(ptrFileHeader->nextHeaderSize),
                   sizeof((ptrFileHeader->nextHeaderSize)), 1, ptrNavFile);

  return itemsRead;
}

size_t fwriteNavConfigFileHeader(const NavConfigFileHeader* ptrCfgHeader,
                                 NAV_FILE* ptrNavFile)
{
  size_t itemsWritten = 0;

  itemsWritten
    += NAV_fwrite(&(ptrCfgHeader->numberOfWPLists),
    sizeof(ptrCfgHeader->numberOfWPLists), 1, ptrNavFile);

  itemsWritten
    += NAV_fwrite(&(ptrCfgHeader->currentWPList),
    sizeof(ptrCfgHeader->currentWPList), 1, ptrNavFile);

  itemsWritten += NAV_fwrite(&(ptrCfgHeader->numberOfExeptionWPLists),
    sizeof(ptrCfgHeader->numberOfExeptionWPLists), 1,
    ptrNavFile);

  itemsWritten
    += NAV_fwrite(&(ptrCfgHeader->currentExeptionWPList),
    sizeof(ptrCfgHeader->currentExeptionWPList), 1, ptrNavFile);

  return itemsWritten;
}

size_t freadNavConfigFileHeader(NavConfigFileHeader *ptrCfgHeader,
                                NAV_FILE *ptrNavFile)
{
  size_t itemsRead = 0;

  itemsRead += NAV_fread(&(ptrCfgHeader->numberOfWPLists),
    sizeof(ptrCfgHeader->numberOfWPLists), 1, ptrNavFile);

  itemsRead += NAV_fread(&(ptrCfgHeader->currentWPList),
    sizeof(ptrCfgHeader->currentWPList), 1, ptrNavFile);

  itemsRead += NAV_fread(&(ptrCfgHeader->numberOfExeptionWPLists),
    sizeof(ptrCfgHeader->numberOfExeptionWPLists), 1,
    ptrNavFile);

  itemsRead
    += NAV_fread(&(ptrCfgHeader->currentExeptionWPList),
    sizeof(ptrCfgHeader->currentExeptionWPList), 1, ptrNavFile);

  return itemsRead;
}

size_t fwriteNavFileWPListHeader(const NavFileWPListHeader* ptrWPListHeader,
                                 NAV_FILE* ptrNavFile)
{
  size_t itemsWritten = 0;
  itemsWritten
      += fwriteCoordinate(&(ptrWPListHeader->startCoordinate), ptrNavFile);

  itemsWritten
      += fwriteCoordinate(&(ptrWPListHeader->endCoordinate), ptrNavFile);

  itemsWritten
      += NAV_fwrite(&(ptrWPListHeader->numberOfEntries),
                    sizeof(ptrWPListHeader->numberOfEntries), 1, ptrNavFile);

  itemsWritten
      += NAV_fwrite(&(ptrWPListHeader->nextHeaderSize),
                    sizeof(ptrWPListHeader->nextHeaderSize), 1, ptrNavFile);

  return itemsWritten;
}

size_t freadNavFileWPListHeader(NavFileWPListHeader* ptrWPListHeader,
                                NAV_FILE* ptrNavFile)
{
  size_t itemsRead = 0;

  itemsRead += freadCoordinate(&(ptrWPListHeader->startCoordinate), ptrNavFile);

  itemsRead += freadCoordinate(&(ptrWPListHeader->endCoordinate), ptrNavFile);

  itemsRead
      += NAV_fread(&(ptrWPListHeader->numberOfEntries),
                   sizeof(ptrWPListHeader->numberOfEntries), 1, ptrNavFile);
  itemsRead
      += NAV_fread(&(ptrWPListHeader->nextHeaderSize),
                   sizeof(ptrWPListHeader->nextHeaderSize), 1, ptrNavFile);

  return itemsRead;
}

size_t fwriteNavDatablockHeader(const NavDatablockHeader* ptrDataHeader,
                                NAV_FILE* ptrNavFile)
{
  size_t itemsWritten = 0;

  itemsWritten += NAV_fwrite(&(ptrDataHeader->dataType),
                             sizeof(ptrDataHeader->dataType), 1, ptrNavFile);

  itemsWritten += NAV_fwrite(&(ptrDataHeader->dataVersion),
                             sizeof(ptrDataHeader->dataVersion), 1, ptrNavFile);

  itemsWritten
      += NAV_fwrite(&(ptrDataHeader->nextDataSize),
                    sizeof(ptrDataHeader->nextDataSize), 1, ptrNavFile);

  return itemsWritten;
}

size_t freadNavDatablockHeader(NavDatablockHeader* ptrDataHeader,
                               NAV_FILE* ptrNavFile)
{
  size_t itemsRead = 0;

  itemsRead += NAV_fread(&(ptrDataHeader->dataType),
                         sizeof(ptrDataHeader->dataType), 1, ptrNavFile);

  itemsRead += NAV_fread(&(ptrDataHeader->dataVersion),
                         sizeof(ptrDataHeader->dataVersion), 1, ptrNavFile);

  itemsRead += NAV_fread(&(ptrDataHeader->nextDataSize),
                         sizeof(ptrDataHeader->nextDataSize), 1, ptrNavFile);

  return itemsRead;
}

/* [] END OF FILE */

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
**
** Structure initialisation functions
**
***********************************************/

// TODO Test
void initNavFileHeader(NavFileHeader* fileHeader)
{
  fileHeader->fileType = INVALID_FILE_TYPE;
  fileHeader->fileVersion = INVALID_NAV_VERSION;
  fileHeader->nextHeaderSize = 0;
}

// TODO Test
void initNavFileWPListHeader(NavFileWPListHeader* WPListHeader)
{
  zeroCoordinate(&(WPListHeader->startCoordinate));
  zeroCoordinate(&(WPListHeader->endCoordinate));
  WPListHeader->numberOfEntries = 0;
  WPListHeader->nextHeaderSize = 0;
}

// TODO Test
void initNavDataBlockHeader(NavDataBlockHeader* dataHeader)
{
  dataHeader->dataType = INVALID_DATA_TYPE;
  dataHeader->dataVersion = INVALID_NAV_VERSION;
  dataHeader->nextDataSize = 0;
}

/***********************************************
**
** Basic file interfaces
**
***********************************************/

// TODO Test
// PC: OK
// PSoC:
NAV_FILE* NAV_fopen(const char* filename, const char* mode)
{
#ifdef __GNUC__
  return FS_FOpen(filename, mode);
#else
  return fopen(filename, mode);
#endif // __GNUC__
}

// TODO Test
int NAV_fclose(NAV_FILE* ptrNavFile)
{
#ifdef __GNUC__
  return FS_FClose(ptrNavFile);
#else
  return fclose(ptrNavFile);
#endif // __GNUC__
}

// TODO Test
int NAV_fseek(NAV_FILE* ptrNavFile, const int32_t offset, const int origin)
{
#ifdef __GNUC__
  return FS_Fseek(ptrNavFile, offset, origin);
#else
  return fseek(ptrNavFile, offset, origin);
#endif // __GNUC__
}

// TODO Test
size_t NAV_fwrite(const void* ptrData, size_t size, size_t count,
                  NAV_FILE* ptrNavFile)
{
#ifdef __GNUC__
  return FS_FWrite(ptrData, size, count, ptrNavFile);
#else
  return fwrite(ptrData, size, count, ptrNavFile);
#endif // __GNUC__
}

// TODO
size_t NAV_fread(void* ptrData, size_t size, size_t count, NAV_FILE* ptrNavFile)
{
#ifdef __GNUC__
  return FS_Fread(prtData, size, count, ptrNavFile);
#else
  return fread(ptrData, size, count, ptrNavFile);
#endif // __GNUC__
}

/***********************************************
**
** Structure read/write functions
**
***********************************************/

// TODO Test
size_t fwriteCoordinate(const Coordinate* ptrCoord, NAV_FILE* ptrNavFile)
{
  size_t bytesWritten = 0;

  bytesWritten += NAV_fwrite(&(ptrCoord->dLongitude),
                             sizeof((ptrCoord->dLongitude)), 1, ptrNavFile);

  bytesWritten += NAV_fwrite(&(ptrCoord->mLongitude),
                             sizeof((ptrCoord->mLongitude)), 1, ptrNavFile);

  bytesWritten += NAV_fwrite(&(ptrCoord->dLatitude),
                             sizeof((ptrCoord->dLatitude)), 1, ptrNavFile);

  bytesWritten += NAV_fwrite(&(ptrCoord->mLatitude),
                             sizeof((ptrCoord->mLatitude)), 1, ptrNavFile);

  bytesWritten += NAV_fwrite(&(ptrCoord->priority),
                             sizeof((ptrCoord->priority)), 1, ptrNavFile);

  return bytesWritten;
}

// TODO Test
size_t freadCoordinate(Coordinate* ptrCoord, NAV_FILE* ptrNavFile)
{
  size_t bytesRead = 0;

  bytesRead += NAV_fread(&(ptrCoord->priority), sizeof((ptrCoord->priority)), 1,
                         ptrNavFile);

  bytesRead += NAV_fread(&(ptrCoord->mLatitude), sizeof((ptrCoord->mLatitude)),
                         1, ptrNavFile);

  bytesRead += NAV_fread(&(ptrCoord->dLatitude), sizeof((ptrCoord->dLatitude)),
                         1, ptrNavFile);

  bytesRead += NAV_fread(&(ptrCoord->mLongitude),
                         sizeof((ptrCoord->mLongitude)), 1, ptrNavFile);

  bytesRead += NAV_fread(&(ptrCoord->dLongitude),
                         sizeof((ptrCoord->dLongitude)), 1, ptrNavFile);

  return bytesRead;
}

// TODO Test
size_t fwriteNavFileHeader(const NavFileHeader* ptrFileHeader,
                           NAV_FILE* ptrNavFile)
{
  size_t bytesWritten = 0;

  bytesWritten += NAV_fwrite(&(ptrFileHeader->fileType),
                             sizeof((ptrFileHeader->fileType)), 1, ptrNavFile);

  bytesWritten
      += NAV_fwrite(&(ptrFileHeader->fileVersion),
                    sizeof((ptrFileHeader->fileVersion)), 1, ptrNavFile);

  bytesWritten
      += NAV_fwrite(&(ptrFileHeader->nextHeaderSize),
                    sizeof((ptrFileHeader->nextHeaderSize)), 1, ptrNavFile);

  return bytesWritten;
}

// TODO Test
size_t freadNavFileHeader(NavFileHeader* ptrFileHeader, NAV_FILE* ptrNavFile)
{
  size_t bytesRead = 0;

  bytesRead
      += NAV_fread(&(ptrFileHeader->nextHeaderSize),
                   sizeof((ptrFileHeader->nextHeaderSize)), 1, ptrNavFile);

  bytesRead += NAV_fread(&(ptrFileHeader->fileVersion),
                         sizeof((ptrFileHeader->fileVersion)), 1, ptrNavFile);

  bytesRead += NAV_fread(&(ptrFileHeader->fileType),
                         sizeof((ptrFileHeader->fileType)), 1, ptrNavFile);

  return bytesRead;
}

// TODO Implement
size_t fwriteNavFileWPListHeader(const NavFileWPListHeader* ptrWPListHeader,
  NAV_FILE* ptrNavFile);

// TODO Implement
size_t freadNavFileWPListHeader(NavFileWPListHeader* ptrWPListHeader,
  NAV_FILE* ptrNavFile);

/* [] END OF FILE */

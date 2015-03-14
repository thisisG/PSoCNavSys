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

#include "navfiles.h"

#ifdef __cplusplus
}
#endif // __cplusplus

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
size_t NAV_fwrite(const void * ptrData, size_t size, size_t count, NAV_FILE* ptrNavFile)
{
#ifdef __GNUC__
    return FS_FWrite(ptrData, size, count, ptrNavFile);
#else
    return fwrite(ptrData, size, count, ptrNavFile);
#endif // __GNUC__
}

// TODO 
uint32_t NAV_fread(void * ptrData, size_t size, size_t count, NAV_FILE* ptrNavFile)
{
#ifdef __GNUC__
    return FS_Fread(prtData, size, count, ptrNavFile);
#else
    return fread(ptrData, size, count, ptrNavFile);
#endif // __GNUC__
}

#ifdef __GNUC__

#else

#endif // __GNUC__

/* [] END OF FILE */

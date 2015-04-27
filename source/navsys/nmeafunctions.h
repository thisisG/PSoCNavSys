/*
**
** NavSys library
** URL: https://github.com/thisisG
** Author: Geir Istad (geir dot istad at gmail dot com)
** Licence: MIT
**
*/

#ifndef NMEAFUNCTIONS_H
#define NMEAFUNCTIONS_H

#ifdef __cplusplus
extern "C" {
#endif

#include <math.h>
#include "navfunctions.h"
#include "navtypes.h"
#include "../nmea/nmea.h"

#ifdef __cplusplus
}
#endif

/*
input:
  const signed16Degree with signed degrees
  const signed32Degree with arcminutes in format MMmmmm
output:
  floatDegree with nmeaFloat in form DDDMM.mmmm
remarks:
  NMEA specific way of presenting floating degrees.
*/
floatDegree nmeaFloatFromLongDegree(const signed16Degree degree,
                                    const signed32Degree minutes);

/*
input:
  const struct Coordinate* with data we want
output:
  floatDegree NMEA degrees of latitude in Coordinate in form DDDMM.mmmm
remarks:
  NMEA specific way of presenting floating degrees.
*/
floatDegree nmeaLatitudeFromCoord(const struct Coordinate* thisCoord);

/*
input:
  const struct Coordinate* with data we want
output:
  floatDegree NMEA degrees of longitude in Coordinate in form DDDMM.mmmm
remarks:
  NMEA specific way of presenting floating degrees.
*/
floatDegree nmeaLongitudeFromCoord(const struct Coordinate* thisCoord);

/*
input:
  const floatDegree nmeaDeg in degrees in the form DDDMM.mmmm
output:
  floatDegree with degrees in from DDD.dddd
remarks:
  Conversion from NMEA speficit way of presenting degrees to a "normal" way with
  format DDD.dddd
*/
floatDegree floatFromNmeaDegree(const floatDegree nmeaDeg);

/*
input:
  const floatDegree NMEA degree in format DDDMM.mmmmm
output:
  signed16Degree* with degrees as DDD
  signed32Degree* with minutes in format MMmmmm
remarks:
  Converts from NMEA type float to integers for storage in Coordinate structures
*/
void longDegreeFromNmeaFloat(const floatDegree nmeaDeg,
                             signed16Degree* ptrDegree,
                             signed32Degree* ptrMinutes);

/*
input:
  nmeaINFO* containing the data we want to access
output:
  signed16Degree* with degrees as DDD
  signed32Degree* with minutes in format MMmmmm
remarks:
  Extracts the latitude from an nmeaINFO data structure to an integer format for
  storage in Coordinate data structures.
*/
void longLatitudeFromNmeaInfo(nmeaINFO* nmeaInfo, signed16Degree* ptrDegree,
                              signed32Degree* ptrMinutes);

/*
input:
  nmeaINFO* containing the data we want to access
output:
  signed16Degree* with degrees as DDD
  signed32Degree* with minutes in format MMmmmm
remarks:
  Extracts the longitude from an nmeaINFO data structure to an integer format
  for storage in Coordinate data structures.
*/
void longLongitudeFromNmeaInfo(nmeaINFO* nmeaInfo, signed16Degree* ptrDegree,
                               signed32Degree* ptrMinutes);

/*
input:
  nmeaINFO* which contains the data we want to access
  Coordinate* where we want to store the extracted data
output:
  (coord->dLatitude)
  (coord->mLatitude)
  (coord->dLongitude)
  (coord->mLongitude)
remarks:
  Extracts longitude and latitude information from nmeaINFO data structure to a
  Coordinate structure where it is stored as pairs of integers for latitude and
  longitude in the format DDD, MMmmmm.
*/
void nmeaInfoToCoord(nmeaINFO* nmeaInfo, Coordinate* coord);

/*
input:
  const nmeaINFO* containing GPS time received
output:
  SystemTime* where we want to store the GPS time
  time->Year
  time->Month
  time->DayOfMonth
  time->Hour
  time->Min
  time->Sec
remarks:
  The time is in UTC time, adjustments to time zone would need to follow if
  necessary for how the system is used.
*/
void nmeaTimeInfoToSysTime(const nmeaINFO* nmeaInfo, SystemTime* time);
#endif

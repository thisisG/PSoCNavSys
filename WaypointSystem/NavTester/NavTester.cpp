// NavTester.cpp : Defines the entry point for the console application.
//


#include <iostream>
#include <fstream>
#include "navmanager\gpsemulator.h"

#ifdef __cplusplus
extern "C" {
#endif
#include <stdio.h>
#include "navsys\navtypes.h"
#include "navsys\navfunctions.h"
#include "navsys\serialcom.h"
#include "navsys\gpsinterface.h"
#include "navsystest\navfilestests.h"
#ifdef __cplusplus
}
#endif

int main()
{
  testfCoordinate();
  testfNavFileHeader();
  testfWPListHeader();
  testfNavDatablockHeader();
  return 0;
}

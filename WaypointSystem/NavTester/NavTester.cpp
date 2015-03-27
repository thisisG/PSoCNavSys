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
#include "navsystest\wphandlertests.h"
#include "navsystest\navmathtests.h"
#ifdef __cplusplus
}
#endif

int main()
{
  // Navfile tests
  testfCoordinate();
  testfNavFileHeader();
  testfWPListHeader();
  testfNavDatablockHeader();

  // WP handler tests
  testWPHandler();
  testmakeTemplateCfgFile();

  const char cfgFile[20] = "cfgtmplt.tst";
  const char wpList[20] = "list.tst";
  addWPListFileToCfgFile(cfgFile, wpList);

  // Navmath tests
  testNavMath();

  return 0;
}

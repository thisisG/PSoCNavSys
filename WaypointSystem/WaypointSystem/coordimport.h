#ifndef COORDIMPORT_H
#define COORDIMPORT_H

#ifdef __cplusplus
extern "C" {
#endif
#include "navtypes.h"
#ifdef __cplusplus
}
#endif

#include <iostream>

class CoordImport {
public:
    CoordImport();
    ~CoordImport();
    void setCsvFile();
    void importCsvToBuffer();
    void putNextCoordinateToNavState(struct Navstate* myNavState);
    void putNextWpToNavState(struct NavState* myNavState);

private:
    int entriesCoordBuffer;
    int entriesWpBuffer;
    struct Coordinate* ptrCoordBuffer;
    struct Coordinate* ptrWpBuffer;
};

class CsvLine {
public:
    CsvLine();
    ~CsvLine();
    std::string Type;
};

#endif
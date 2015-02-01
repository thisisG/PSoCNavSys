#ifndef SERIALCOM_H
#define SERIALCOM_H

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus
#include <stdio.h>
#include "navtypes.h"
#include "navFunctions.h"
#ifdef __cplusplus
}
#endif // __cplusplus

void navDataToSerialBuffer(NavState* navS);

#endif // SERIALCOM_H
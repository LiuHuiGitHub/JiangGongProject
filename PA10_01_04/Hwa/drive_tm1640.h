#ifndef __DRIVE_TM1640_H__
#define __DRIVE_TM1640_H__

#define TM1640_NUM      2

#include "typedef.h"

void drv_tm1640Init(void);
void drv_tm1640Update1(UINT8 *pData);
void drv_tm1640Update2(UINT8 *pData);
void drv_tm1640Display(UINT8 *pData);

#endif

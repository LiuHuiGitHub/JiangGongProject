#ifndef __DRIVE_LED_H__
#define __DRIVE_LED_H__

#include "typedef.h"

extern data UINT8 u8_ledDisBuff[4];
extern data UINT8 u8_ledDot;

void drv_ledInit(void);
void drv_ledHandler1ms(void);

#endif

#ifndef __APP_TIME_H__
#define __APP_TIME_H__

#include "typedef.h"

#define TIME_SAVE_SECTOR                2
#define ALL_CHANNEL                     10

extern UINT16 u16_DisplayTime[ALL_CHANNEL];
extern UINT8 DisplayBuff[32];

void app_timeInit(void);
void app_timeClear(UINT8 channel);
void app_timeLoad(void);
void app_timeSaveTime(void);
void app_timeDisplay100ms(void);
void app_timePower1min(void);
void app_timeAddMoney(UINT8 channel, UINT16 time);
void app_timeCheckHandler1s(void);


#endif

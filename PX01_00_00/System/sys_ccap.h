#ifndef __SYS_CCAP_H__
#define __SYS_CCAP_H__


#include "typedef.h"


extern data UINT32 u32_PulseWidth;
extern data UINT32 u32_PulseCycle;                       //存储信号的时间长度(count1 - count0)


void sys_ccapInit(void);
void sys_ccapEnable(void);
void sys_ccapDisable(void);


void sys_TrigPulseInit(void);
void sys_trigPulseWidth(UINT16 width);

#endif
#ifndef __DRIVE_SCR_H__
#define __DRIVE_SCR_H__

#include "typedef.h"

typedef enum
{
	SCR_OFF,
	SCR_VVVF,
	SCR_FULL,
	SCR_VVVF_OFF,
	SCR_TEST_OFF,
	SCR_TEST_FULL,
}SCR_STATE;

void drv_scrInit(void);
void drv_scrHandler2ms(void);
void drv_scrRequest(SCR_STATE state);
void drv_ledBreathing(void);

#endif
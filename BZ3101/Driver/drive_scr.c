#include "iostm8s103f3.h"
#include "drive_scr.h"
#include "sys_pwm.h"

SCR_STATE e_scrState = SCR_OFF;
static UINT16 u16_vvvfScrCounter = 0;

#define OUTPUT_SCR0		PA_ODR_ODR3
#define OUTPUT_SCR1		PA_ODR_ODR2

#define SCR_OUT_ON		1
#define SCR_OUT_OFF		0
#define LED_ON			0
#define LED_OFF			1

void drv_scrInit(void)
{
	OUTPUT_SCR0 = SCR_OUT_OFF;
	OUTPUT_SCR1 = SCR_OUT_OFF;
	sys_pwm2Channel3Set(0x00);
}

#define TTTT			100
UINT8 ttt = TTTT;

void drv_ledBreathing(void)
{
	static UINT8 dir = 0;
	static UINT8 counter = 0;
	static UINT8 count = 0;
	UINT8 cccc = 20;
	if(counter < TTTT)
	{
		counter++;
	}
	else
	{
		counter = 0;
	}
//	if(ttt>95)
//	{
//		cccc = 100;
//	}
//	else if(ttt>75)
//	{
//		cccc = 80;
//	}
//	else if(ttt>45)
//	{
//		cccc = 50;
//	}
//	else
//	{
//		cccc = 35;
//	}
	if(++count >= cccc)
	{
		count = 0;
		if(dir == 0)
		{
			if(--ttt == 0)
			{
				dir = 1;
			}
		}
		else
		{
			if(++ttt >= TTTT)
			{
				dir = 0;
			}
		}
	}
	sys_pwm2Channel3Set(ttt);
}

void drv_scrHandler1ms(void)
{
	if(e_scrState == SCR_FULL)
	{
		OUTPUT_SCR0 = SCR_OUT_ON;
		OUTPUT_SCR1 = SCR_OUT_ON;
		sys_pwm2Channel3Set(0);
	}
	else if(e_scrState == SCR_VVVF)
	{
		OUTPUT_SCR0 = SCR_OUT_ON;
		u16_vvvfScrCounter++;
		if(u16_vvvfScrCounter >= 500)
		{
			u16_vvvfScrCounter = 0;
			OUTPUT_SCR1 = SCR_OUT_ON;
		}
		else if(u16_vvvfScrCounter >= 400)
		{
			OUTPUT_SCR1 = SCR_OUT_OFF;
		}
		drv_ledBreathing();
	}
	else if(e_scrState == SCR_VVVF_OFF)
	{
		OUTPUT_SCR0 = SCR_OUT_ON;
		OUTPUT_SCR1 = SCR_OUT_OFF;
		drv_ledBreathing();
	}
	else
	{
		OUTPUT_SCR0 = SCR_OUT_OFF;
		OUTPUT_SCR1 = SCR_OUT_OFF;
		sys_pwm2Channel3Set(0);
	}
}

void drv_scrRequest(SCR_STATE state)
{
	e_scrState = state;
}

#include <STC15F2K60S2.H>
#include "drive_buzzer.h"

sbit Buzzer = P1^2;

UINT8 u8_buzzerNumberCount = 0;
UINT8 u8_buzzerCount = 0;
#define BUZZER_ON_TIME         5
#define BUZZER_OFF_TIME        10

void drv_buzzerInit(void)
{
    P1M1 &= ~0x04;      //ÍÆÍìÊä³ö
    P1M0 |= 0x04;
    Buzzer = 0;
}

void drv_buzzerHandler10ms(void)
{
    if(u8_buzzerCount)
    {
        u8_buzzerCount--;
        return;
    }
    if(Buzzer)
	{
		Buzzer = 0;
        u8_buzzerCount = BUZZER_OFF_TIME;
	}
	else if(u8_buzzerNumberCount)
	{
		Buzzer = 1;
        u8_buzzerCount = BUZZER_ON_TIME;
		u8_buzzerNumberCount--;
	}
}

void drv_buzzerNumber(UINT8 num)
{
    u8_buzzerNumberCount = num;
}
#include "STC15F2K60S2.h"
#include "drive_relay.h"
#include "sys_delay.h"

sbit relay0 = P1 ^ 0;
sbit relay1 = P1 ^ 0;

void drv_relayInit(void)
{
    relay0 = 0;
    P1M1 &= ~0x01;      //�������
    P1M0 |= 0x01;
}

void drv_relayOpen(UINT8 ch)
{
    if(ch == 0)
    {
        relay0 = 1;
    }
    else if(ch == 1)
    {
        relay1 = 1;
    }
    else
    {
        relay0 = 1;
        relay1 = 1;
    }
}

void drv_relayClose(UINT8 ch)
{
    if(ch == 0)
    {
        relay0 = 0;
    }
    else if(ch == 1)
    {
        relay1 = 0;
    }
    else
    {
        relay0 = 0;
        relay1 = 0;
    }
}


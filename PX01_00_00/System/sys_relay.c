#include "STC15F2K60S2.h"
#include "sys_relay.h"
#include "sys_delay.h"

sbit relay0 = P3 ^ 4;
sbit relay1 = P3 ^ 5;
sbit relay2 = P3 ^ 6;
sbit relay3 = P3 ^ 7;
sbit relay4 = P2 ^ 0;
sbit relay5 = P2 ^ 1;
sbit relay6 = P2 ^ 2;
sbit relay7 = P2 ^ 3;
sbit relay8 = P2 ^ 4;
sbit relay9 = P2 ^ 5;
sbit relay10 = P3 ^ 3;  //test

void sys_relayOpen(UINT8 ch)
{
    if(ch == 0)
    {
        relay0 = 1;
    }
    else if(ch == 1)
    {
        relay1 = 1;
    }
    else if(ch == 2)
    {
        relay2 = 1;
    }
    else if(ch == 3)
    {
        relay3 = 1;
    }
    else if(ch == 4)
    {
        relay4 = 1;
    }
    else if(ch == 5)
    {
        relay5 = 1;
    }
    else if(ch == 6)
    {
        relay6 = 1;
    }
    else if(ch == 7)
    {
        relay7 = 1;
    }
    else if(ch == 8)
    {
        relay8 = 1;
    }
    else if(ch == 9)
    {
        relay9 = 1;
    }
    else if(ch == TEST_RELAY)
    {
        relay10 = 1;
    }
    else if(ch == ALL_RELAY)
    {
        relay0 = 1;
        sys_delayms(1);
        relay1 = 1;
        sys_delayms(1);
        relay2 = 1;
        sys_delayms(1);
        relay3 = 1;
        sys_delayms(1);
        relay4 = 1;
        sys_delayms(1);
        relay5 = 1;
        sys_delayms(1);
        relay6 = 1;
        sys_delayms(1);
        relay7 = 1;
        sys_delayms(1);
        relay8 = 1;
        sys_delayms(1);
        relay9 = 1;
        sys_delayms(1);
        relay10 = 1;
    }
}

void sys_relayClose(UINT8 ch)
{
    if(ch == 0)
    {
        relay0 = 0;
    }
    else if(ch == 1)
    {
        relay1 = 0;
    }
    else if(ch == 2)
    {
        relay2 = 0;
    }
    else if(ch == 3)
    {
        relay3 = 0;
    }
    else if(ch == 4)
    {
        relay4 = 0;
    }
    else if(ch == 5)
    {
        relay5 = 0;
    }
    else if(ch == 6)
    {
        relay6 = 0;
    }
    else if(ch == 7)
    {
        relay7 = 0;
    }
    else if(ch == 8)
    {
        relay8 = 0;
    }
    else if(ch == 9)
    {
        relay9 = 0;
    }
    else if(ch == TEST_RELAY)
    {
        relay10 = 0;
    }
    else if(ch == ALL_RELAY)
    {
        relay0 = 0;
        relay1 = 0;
        relay2 = 0;
        relay3 = 0;
        relay4 = 0;
        relay5 = 0;
        relay6 = 0;
        relay7 = 0;
        relay8 = 0;
        relay9 = 0;
        relay10 = 0;
    }
}


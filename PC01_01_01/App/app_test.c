#include "stc15f2k60s2.h"
#include "app_test.h"
#include "typedef.h"

sbit b_input = P1^1;
BOOL b_lastInput = FALSE;
UINT8 u8_timeCount = 0;

UINT8 u8_testBuff = 0x00;

void app_testInit(void)
{
    b_input = 1;
}

void app_testHandler1ms(void)
{
    static UINT8 count = 0;
    static BOOL flag = FALSE;
    if(u8_timeCount < 0xFF)
    {
        u8_timeCount++;
    }
    else
    {
        u8_testBuff = 0x00;
    }
    if(b_input != b_lastInput)
    {
        b_lastInput = b_input;
        flag = TRUE;
        if(b_input)
        {
            u8_testBuff >>= 1;
            if(u8_timeCount > 5 && u8_timeCount < 15)
            {
                u8_testBuff |= 0x80;
            }
        }
        else
        {
            u8_timeCount = 0;
        }
    }
}

BOOL app_testGetFuseState(void)
{
    UINT8 x, num = 0;
    x = u8_testBuff;
    while(x)        //计算有多少个1
    {
        x = x&(x-1);
        num++;
    }
    if(num >= 3)     
    {
        return TRUE;
    }
    return FALSE;
}
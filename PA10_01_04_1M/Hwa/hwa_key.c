#include "hwa_key.h"
#include "drive_tm1650.h"

UINT8 KeyDelayCnt = 0;
UINT8 KeySetTimeCnt = 0;
UINT8 KeyTestTimeCnt = 0;
UINT8 lastKey = KEY_NONE;
UINT8 keyDelayTime = 0;
UINT8 hwa_keyGet(void)
{
    UINT8 KEY;

    KEY = drv_tm1650GetKey();
    
    if(KEY == lastKey)
    {
        if(keyDelayTime < 3)
        {
            keyDelayTime++;
            KEY = KEY_NONE;
        }
    }
    else
    {
        lastKey = KEY;
        KEY = KEY_NONE;
        keyDelayTime = 0;
    }
    
    if(KEY == KEY_SET)
    {
        KeySetTimeCnt++;
        if(KeySetTimeCnt >= INTO_SET_TIME)
        {
            KeySetTimeCnt = 0;
            KEY = KEY_SET;
        }
        else
        {
            KEY = KEY_NONE;
        }
    }
    else if(KEY == KEY_TEST)
    {
        KeyTestTimeCnt++;
        if(KeyTestTimeCnt >= INTO_SET_TIME)
        {
            KeyTestTimeCnt = 0;
            KEY = KEY_TEST;
        }
        else
        {
            KEY = KEY_NONE;
        }
    }
    else
    {
        KeySetTimeCnt = 0;
        KeyTestTimeCnt = 0;
    }
    return KEY;
}


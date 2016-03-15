#include "STC15F2K60S2.h"
#include "hwa_coin.h"


sbit PutInCoin = P1^0;
sbit PutInPower = P3^2;

#define PUT_IN_COIN_TIME        ((UINT8)20)     //20ms
#define PUT_IN_CARD_TIME        ((UINT8)50)     //50ms
#define TIME_ERROR              ((UINT8)5)      //5ms

PUT_STATE_ENUM e_putState = PUT_NONE;

UINT8 Count = 0;

void hwa_coinHandler1ms(void) 
{
    if(PutInCoin==0 && e_putState==PUT_NONE)
    {
        if(Count < 0xFF)
        {
            Count++;
        }
    }
    else
    {
        if(Count>=PUT_IN_COIN_TIME-TIME_ERROR && Count<=PUT_IN_COIN_TIME+TIME_ERROR)
        {
            e_putState = PUT_COIN;
            hwa_coinOff();
        }
        else if(Count>=PUT_IN_CARD_TIME-TIME_ERROR && Count<=PUT_IN_CARD_TIME+TIME_ERROR)
        {
            e_putState = PUT_CARD;
            hwa_coinOff();
        }
        Count = 0;
    }
}

PUT_STATE_ENUM hwa_coinPutIn(void)
{
    PUT_STATE_ENUM state = e_putState;
    if(state != PUT_NONE)
    {
        e_putState = PUT_NONE;
    }
    hwa_coinOn();
    return state;
}

void hwa_coinOn(void)
{
    PutInPower = 1;
}

void hwa_coinOff(void)
{
    PutInPower = 0;
}


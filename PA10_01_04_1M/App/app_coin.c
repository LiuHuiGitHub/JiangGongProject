#include "app_coin.h"
#include "app_test.h"
#include "hwa_coin.h"
UINT16 u16_PutInCoinSave = 0;
UINT16 u16_PutInCardSave = 0;


UINT8 app_coinPutIn(void)
{
    PUT_STATE_ENUM putIn;
    putIn = hwa_coinPutIn();
    if(putIn != PUT_NONE)
    {
        if(putIn == PUT_COIN)
        {
            s_moneySave.CoinCount++;
            if(u16_PutInCoinSave < 999 - s_SystemSetting.CoinTimeOrPower)
            {
                u16_PutInCoinSave += s_SystemSetting.CoinTimeOrPower;
            }
            else
            {
                u16_PutInCoinSave = 999;
            }
        }
        else if(putIn == PUT_CARD)
        {
            s_moneySave.CardCount++;
            if(u16_PutInCardSave < 999 - s_SystemSetting.CardTimeOrPower)
            {
                u16_PutInCardSave += s_SystemSetting.CardTimeOrPower;
            }
            else
            {
                u16_PutInCardSave = 999;
            }
        }
        app_testSavePutInMoney();
    }
    return putIn;
}


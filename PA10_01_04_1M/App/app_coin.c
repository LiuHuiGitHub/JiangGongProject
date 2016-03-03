#include "app_coin.h"
#include "app_test.h"
#include "hwa_coin.h"

UINT8 app_coinPutIn(void)
{
    PUT_STATE_ENUM putIn;
    putIn = hwa_coinPutIn();
    if(putIn != PUT_NONE)
    {
        if(putIn == PUT_COIN)
        {
            s_moneySave.CoinCount++;
            if(s_moneySave.PutInCoinSave < 999 - s_SystemSetting.CoinTimeOrPower)
            {
                s_moneySave.PutInCoinSave += s_SystemSetting.CoinTimeOrPower;
            }
            else
            {
                s_moneySave.PutInCoinSave = 999;
            }
        }
        else if(putIn == PUT_CARD)
        {
            s_moneySave.CardCount++;
            if(s_moneySave.PutInCardSave < 999 - s_SystemSetting.CardTimeOrPower)
            {
                s_moneySave.PutInCardSave += s_SystemSetting.CardTimeOrPower;
            }
            else
            {
                s_moneySave.PutInCardSave = 999;
            }
        }
        app_testSavePutInMoney();
    }
    return putIn;
}


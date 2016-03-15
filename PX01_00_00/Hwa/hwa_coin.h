#ifndef __HWA_COIN_H__
#define __HWA_COIN_H__

#include "typedef.h"

typedef enum
{
    PUT_NONE = 0x00,
    PUT_COIN,
    PUT_CARD,
}PUT_STATE_ENUM;

void hwa_coinHandler1ms(void);
PUT_STATE_ENUM hwa_coinPutIn(void);
void hwa_coinOff(void);
void hwa_coinOn(void);

#endif

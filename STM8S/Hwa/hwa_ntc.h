#ifndef __HWA_NTC_H__
#define __HWA_NTC_H__

#include "typedef.h"

void hwa_ntcInit(void);
void hwa_ntcHandler500ms(void);
UINT8 hwa_ntcGetTemp(void);

#endif /* __HWA_NTC_H__ */

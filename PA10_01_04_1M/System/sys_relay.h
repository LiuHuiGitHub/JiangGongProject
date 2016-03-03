#ifndef __SYS_RELAY_H__
#define __SYS_RELAY_H__

#include "typedef.h"

#define ALL_RELAY       11
#define TEST_RELAY      10

void sys_relayOpen(UINT8 ch);
void sys_relayClose(UINT8 ch);

#endif

#ifndef __SYS_UART_H__
#define __SYS_UART_H__

#include "typedef.h"

void sys_uartInit(void);
void sys_uartTransmitByte(UINT8 data);
void sys_uartTransmitData(UINT8 *Data, UINT8 len);
UINT8 sys_uartReceiveData(UINT8 *pData);
#endif

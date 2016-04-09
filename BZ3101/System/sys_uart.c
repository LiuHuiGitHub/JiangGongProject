#include "iostm8s103f3.h"
#include "sys_vector.h"
#include "sys_uart.h"
#include "string.h"

#define SYS_CLOCK			(UINT32)16000000
#define BOUD				(UINT32)115200


#define UART_RX_BUFF_LEN		10
#define UART_RX_OVER_COUNT		10
static UINT8 Rx_Buff[UART_RX_BUFF_LEN + 1];
static UINT8 RxIndex = 0;
static UINT8 RxOverCount = 0;

void sys_uartInit(void)
{
    PD_DDR |= BIT5;
    PD_CR1 |= BIT5;
    PD_CR2 |= BIT5;

    PD_DDR &= ~BIT6;
    PD_CR1 |= BIT6;
    PD_CR2 &= ~BIT6;

    CLK_PCKENR1 |= BIT2;		//enable uart1 clock

    UART1_CR1 = 0x00;
    UART1_CR2 = 0x00;
    UART1_CR3 = 0x00;
    UART1_CR4 = 0x00;
    UART1_CR5 = 0x00;

    UART1_GTR = 0x00;
    UART1_PSCR = 0x00;

    (void)UART1_SR;
    (void)UART1_DR;

    UART1_BRR2 = ((UINT8)((UINT16)(SYS_CLOCK / BOUD) >> 8) & 0xF0) | ((UINT8)(SYS_CLOCK / BOUD) & 0x0F);
    UART1_BRR1 = (UINT8)((SYS_CLOCK / BOUD) >> 4); /*先给BRR2赋值 最后再设置BRR1*/

    UART1_CR2 = 0x2C;
}

void sys_uartTransmitByte(UINT8 data)
{
    /* Loop until the end of transmission */
    while (!(UART1_SR & BIT7));
    UART1_DR = data;
}

void sys_uartTransmitData(UINT8 *Data, UINT8 len)
{
    while(len--)
    {
        sys_uartTransmitByte(*Data++);
    }
}

#pragma vector = UART1_RX_IRQ
__interrupt void UART1_RX_IRQHandler(void)
{
    if(UART1_SR & BIT3)
    {
        (void)UART1_DR;
    }
    else if(UART1_SR & BIT5)
    {
        Rx_Buff[RxIndex] = UART1_DR;
        if(RxIndex < UART_RX_BUFF_LEN)
        {
            RxIndex++;
        }
        RxOverCount = UART_RX_OVER_COUNT;
    }
}

UINT8 sys_uartReceiveData(UINT8 *pData)
{
    UINT8 len;
    if(RxOverCount)
    {
        RxOverCount--;
        if(RxOverCount == 0)
        {
            len = RxIndex;
            memcpy(pData, Rx_Buff, len);
            RxIndex = 0;
            return len;
        }
    }
    return 0;
}

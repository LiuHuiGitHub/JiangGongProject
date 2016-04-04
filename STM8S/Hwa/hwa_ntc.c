#include "hwa_ntc.h"
#include "sys_adc.h"

#define NUM_OF_TEMP		(100)
CONST UINT16 c_u16_ntcTable[NUM_OF_TEMP] = 
{
148 	,
154 	,
160 	,
167 	,
174 	,
181 	,
188 	,
195 	,
203 	,
211 	,
219 	,
227 	,
235 	,
243 	,
252 	,
261 	,
270 	,
279 	,
288 	,
297 	,
307 	,
316 	,
326 	,
336 	,
346 	,
356 	,
366 	,
376 	,
386 	,
397 	,
407 	,
417 	,
428 	,
438 	,
449 	,
459 	,
470 	,
480 	,
490 	,
501 	,
511 	,
521 	,
532 	,
542 	,
552 	,
562 	,
572 	,
582 	,
591 	,
601 	,
610 	,
620 	,
629 	,
638 	,
647 	,
656 	,
665 	,
673 	,
681 	,
690 	,
698 	,
706 	,
714 	,
721 	,
729 	,
736 	,
743 	,
750 	,
757 	,
764 	,
771 	,
777 	,
783 	,
789 	,
795 	,
801 	,
807 	,
813 	,
818 	,
823 	,
829 	,
834 	,
839 	,
843 	,
848 	,
852 	,
857 	,
861 	,
865 	,
869 	,
873 	,
877 	,
881 	,
884 	,
888 	,
891 	,
895 	,
898 	,
901 	,
904 	,
//100¡æ
};


void hwa_ntcInit(void)
{
	sys_adcInit();
}


static UINT8 u8_tempOld = 0;
UINT16 u16_thresholdH;
UINT16 u16_thresholdL;
void hwa_ntcHandler500ms(void)
{
	UINT16 u16_adValue;
//	UINT32 u32_vccValue;
	UINT8 temp;
//	u32_vccValue = sys_adcValue(7);
	u16_adValue = sys_adcValue(3);
	for(temp=0; temp< NUM_OF_TEMP; temp++)
	{
		if(u16_adValue < c_u16_ntcTable[temp])
		{
			break;
		}
	}
	if(temp == 0)									//NTC¿ªÂ·
	{
		u8_tempOld = 0;
	}
	else if(temp > c_u16_ntcTable[NUM_OF_TEMP-1])	//NTC¶ÌÂ·
	{
		u8_tempOld =  100;
	}
	else
	{
		if(temp > u8_tempOld)
		{
			u16_thresholdH = (c_u16_ntcTable[u8_tempOld+1]-c_u16_ntcTable[u8_tempOld])/3+c_u16_ntcTable[u8_tempOld];
			if(u16_adValue >= u16_thresholdH)
			{
				u8_tempOld = temp;
			}
			else
			{
				u8_tempOld = temp-1;
			}
		}
		else if(temp < u8_tempOld)
		{
			u16_thresholdL = c_u16_ntcTable[temp]-(c_u16_ntcTable[temp]-c_u16_ntcTable[temp-1])/3;
			if(u16_adValue <= u16_thresholdL)
			{
				u8_tempOld = temp;
			}
			else
			{
				u8_tempOld = temp+1;
			}
		}
	}
}

UINT8 hwa_ntcGetTemp(void)
{
    return u8_tempOld;
}


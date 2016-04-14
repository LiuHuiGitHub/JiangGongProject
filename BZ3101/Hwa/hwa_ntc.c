#include "hwa_ntc.h"
#include "sys_adc.h"

#define NUM_OF_TEMP		(100)
CONST UINT16 c_u16_ntcTable[NUM_OF_TEMP] = 
{
139 	,
145 	,
152 	,
158 	,
165 	,
172 	,
179 	,
187 	,
194 	,
202 	,
210 	,
218 	,
226 	,
234 	,
243 	,
251 	,
260 	,
269 	,
278 	,
288 	,
297 	,
306 	,
316 	,
326 	,
336 	,
346 	,
356 	,
366 	,
376 	,
387 	,
397 	,
407 	,
418 	,
428 	,
439 	,
449 	,
460 	,
470 	,
480 	,
491 	,
501 	,
511 	,
522 	,
532 	,
542 	,
552 	,
562 	,
572 	,
581 	,
591 	,
601 	,
610 	,
619 	,
628 	,
637 	,
646 	,
655 	,
664 	,
672 	,
680 	,
689 	,
697 	,
704 	,
712 	,
720 	,
727 	,
735 	,
742 	,
749 	,
755 	,
762 	,
769 	,
775 	,
781 	,
787 	,
793 	,
799 	,
805 	,
810 	,
816 	,
821 	,
826 	,
831 	,
836 	,
841 	,
845 	,
850 	,
854 	,
859 	,
863 	,
867 	,
871 	,
875 	,
879 	,
882 	,
886 	,
889 	,
893 	,
896 	,
899 	,
//100¡æ
};


void hwa_ntcInit(void)
{
	sys_adcInit();
}


static UINT8 u8_tempOld = 0;
void hwa_ntcHandler500ms(void)
{
	UINT16 u16_thresholdH;
	UINT16 u16_thresholdL;
	UINT16 u16_adValue;
	UINT8 u8_tempNew;
	u16_adValue = sys_adcValue(4);
	for(u8_tempNew=0; u8_tempNew< NUM_OF_TEMP; u8_tempNew++)
	{
		if(u16_adValue < c_u16_ntcTable[u8_tempNew])
		{
			break;
		}
	}
	if(u8_tempNew == 0)									//NTC¿ªÂ·
	{
		u8_tempOld = 0;
	}
	else if(u8_tempNew > c_u16_ntcTable[NUM_OF_TEMP-1])	//NTC¶ÌÂ·
	{
		u8_tempOld =  100;
	}
	else
	{
		if(u8_tempNew > u8_tempOld)
		{
			u16_thresholdH = (c_u16_ntcTable[u8_tempOld+1]-c_u16_ntcTable[u8_tempOld])/4+c_u16_ntcTable[u8_tempOld];
			if(u16_adValue >= u16_thresholdH)
			{
				u8_tempOld = u8_tempNew;
			}
			else
			{
				u8_tempOld = u8_tempNew-1;
			}
		}
		else if(u8_tempNew < u8_tempOld)
		{
			u16_thresholdL = c_u16_ntcTable[u8_tempNew]-(c_u16_ntcTable[u8_tempNew]-c_u16_ntcTable[u8_tempNew-1])/4;
			if(u16_adValue <= u16_thresholdL)
			{
				u8_tempOld = u8_tempNew;
			}
			else
			{
				u8_tempOld = u8_tempNew+1;
			}
		}
	}
}

UINT8 hwa_ntcGetTemp(void)
{
    return u8_tempOld;
}


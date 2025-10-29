#include "REL_Sender.h"
#include <stdlib.h>
#include "Touch_Kscan_Library.h"

#define PACK_SEND_COUNT_ONCE	2

#if (C_KEY_TOTAL % PACK_SEND_COUNT_ONCE)
	#define PACK_IDX_MAX 			(C_KEY_TOTAL / PACK_SEND_COUNT_ONCE)
	#define PACK_SEND_COUNT_FINAL	(C_KEY_TOTAL % PACK_SEND_COUNT_ONCE)
#else
	#define PACK_IDX_MAX 			((C_KEY_TOTAL-1) / PACK_SEND_COUNT_ONCE)
	#define PACK_SEND_COUNT_FINAL	PACK_SEND_COUNT_ONCE
#endif

#define REL_TYPE_ID_USE	0x07

typedef struct _PackStrcut
{
	u16 rawData;					//�˲�ֵ
	u16 thresholdA;					//��ֵ1
	#ifdef RS_VALUE
	u16 thresholdB;				 	//��ֵ2
	#endif
	u16 noiseData;					//����ֵ
}PackStrcut;

void REL_SenderLoop()
{
	static bit bSend = 0;
	bSend = ~bSend;
	if(bSend == 0) return;	
	
	static u8 curIdx = 0;
	static u8 packIdx = 0;
	
	u8 thisSendCount;	
	PackHeader packHeader;
	PackStrcut packStrcut;
	
	// ��дͷ������
	packHeader.m.typeId = REL_TYPE_ID_USE;
	packHeader.m.packIdxMax = PACK_IDX_MAX;
	packHeader.m.packIdx = packIdx;

	#ifdef VOL_VALUE
	packHeader.extData[0] = VOL_VALUE;
	#else
	packHeader.extData[0] = 0;
	#endif

	packHeader.extData[1] = 0;

	if(packIdx == PACK_IDX_MAX)
	{
		thisSendCount = PACK_SEND_COUNT_FINAL;
		packHeader.length = sizeof(packHeader) + sizeof(packStrcut) * PACK_SEND_COUNT_FINAL + 1;
	}
	else
	{
		thisSendCount = PACK_SEND_COUNT_ONCE;
		packHeader.length = sizeof(packHeader) + sizeof(packStrcut) * PACK_SEND_COUNT_ONCE + 1;
	}
	
	// ��ʼ����
	REL_SendStart(packHeader);
	do
	{
		packStrcut.rawData = _CMS_FactorRawDat[curIdx];				
		packStrcut.thresholdA = _CMS_DispKeyDown[curIdx];
		/*
		#ifdef RS_VALUE
			packStrcut.thresholdB = _CMS_DispKeyDown[curIdx];
		#endif				
		*/
		packStrcut.noiseData = _CMS_NoiseDat[curIdx];
				
		REL_SendNext(packStrcut);	
		thisSendCount--;
		curIdx++;		
	}while(thisSendCount != 0);
	REL_SendEnd();	
	
	// �Ƿ����һ��
	if(packIdx == PACK_IDX_MAX)
	{
		packIdx = 0;
		curIdx = 0;
	}
	else
	{
		packIdx++;
	}
}

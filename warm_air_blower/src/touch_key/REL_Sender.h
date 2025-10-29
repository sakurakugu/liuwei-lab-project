#ifndef _REL_SENDER_
#define _REL_SENDER_

// ���ݷ��Ϳ���,1:���������಻����
#define REL_SENDER_ENABLE	1

/**************************************************************************
**************************�������������޸�*********************************
**************************************************************************/
void REL_SenderLoop();

//////////////////////////////////////////////////////
// ����ʵ�ֶ���
#include "iic_soft.h"
#define REL_PreSendCallBack()								I2C_WriteStart()
#define REL_SendByte(ch)									I2C_Write1bYTE(ch)
#define REL_EndSendCallBack()								I2C_WriteStop()

// �������Ͷ���
#if 1
typedef   signed            char int8_t;
typedef   signed short      int int16_t;
typedef   signed            long  int32_t;

typedef unsigned            char uint8_t;
typedef unsigned short      int  uint16_t;
typedef unsigned  			long uint32_t;

typedef int32_t  s32;
typedef int16_t  s16;
typedef int8_t   s8;

typedef uint32_t  u32;
typedef uint16_t  u16;
typedef uint8_t   u8;
#endif

//////////////////////////////////////////////////////

// ��ͷ�ֽ�
#define REL_HEADER_BYTE	0xA0

// ��ͷ�ṹ
//#pragma pack(1)
//#pragma anon_unions
typedef struct _PackHeader
{
	u8 length;		//����,sizeof(PackHeader) + sizeof(PackStrcut) * n + 1
	union
	{
		u16 mask;
		struct
		{
			u8 packIdx : 4;		// �ְ�����,��0��ʼ
			u8 packIdxMax : 4;		// ��������
			u8 typeId;				// ����ID,����ʹ�õĽṹ��
		}m;
	};
	u8 extData[2];
}PackHeader;
//#pragma pack()

// ���շ��ͺ���					
#define REL_SendStart(packHeader)	\
	{u16 _checkSum = 0;\
	u8 _i;\
	u8 *_bufPoint;\
	REL_PreSendCallBack();\
	REL_SendByte(REL_HEADER_BYTE);\
	packHeader.length+=2;\
	_bufPoint = (u8*)&packHeader;\
	for(_i = 0;_i < sizeof(packHeader);_i++,_bufPoint++) {\
		REL_SendByte(*_bufPoint);\
		_checkSum += *_bufPoint;\
	}

#define REL_SendNext(packStrcut) \
	_bufPoint = (u8*)&packStrcut;\
	for(_i = 0;_i < sizeof(packStrcut);_i++,_bufPoint++) {\
		REL_SendByte(*_bufPoint);\
		_checkSum += *_bufPoint;\
	}

#define REL_SendEnd()	\
	REL_SendByte(0x55);\
	REL_SendByte(_checkSum >> 8);\
	REL_SendByte(_checkSum & 0xff);\
	REL_EndSendCallBack();}

#endif /*_REL_SENDER_USER_*/


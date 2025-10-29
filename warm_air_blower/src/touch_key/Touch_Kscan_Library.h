// ��� CMS79F726 �� LIB_TYPE_PT_NORMAL �� ���� CTouch V2.3.3 ���ɵ�

/*

*/
#ifndef	_CHECKTOUCHKEY_H_
#define	_CHECKTOUCHKEY_H_

#include "TouchKeyConst.h"
#pragma warning disable 752,759
//�ӿں�������
extern void __CMS_CheckTouchKey(void);		//����ɨ����������,������2.5ms~5ms�ڵ���
extern void __CMS_GetTouchKeyValue(void);//�˺��������жϣ������ж�ɨ��ʱ��125us
extern volatile bit _CMS_b_kerr;				//��λ�����־
//ÿ��������Ӧ�Ĵ�����1��λ,�ɸ�����Ҫһһ��Ӧ
//_CMS_KeyFlag[0]��Ӧ��Key8,...,Key2,Key1
//_CMS_KeyFlag[1]��Ӧ��Key16,...,Key10,Key9
//������Чʱ��ӦλΪ1,��ЧʱΪ0
extern volatile uint8_t _CMS_KeyFlag[2];
//-------------------------------------------------------
//ʹ��ʱ�����ʵ��Ӳ������޸���������ò���
//ע�������С����С�ڼ�ⰴ������

// ����ͨ���趨,ΪC_KCHS0��C_KCHS25�е�һ��
const uint8_t _CMS_Table_KeyChannel[] = {
    C_KCHS15, // Key1 - ��Դ���� (RB0)
    C_KCHS14, // Key2 - ��ͷ���� (RB1)
    C_KCHS13, // Key3 - ģʽ���� (RB2)
    C_KCHS12, // Key4 - ��ʱ���� (RB3)

};

// ����������ֵ,�趨ֵ�ɵ���Χ20,��󲻳���1023
// �˰���������ֵ�趨��ο���׼ֵVOL_VALUE
const uint16_t _CMS_Table_KeyDown[] = {
    50, // Key1 - ��Դ����
    50, // Key2 - ��ͷ����
    50, // Key3 - ģʽ����
    50, // Key4 - ��ʱ����

};

//-------------------------------------------------------
//-------------------------------------------------------
//-------------------------------------------------------
/*
���������Ч���ò���
*/
//��ⰴ������,���16
#define		C_KEY_TOTAL		4	//���֧��16ͨ��

//����������������
#define		C_KEY_SHAKE		4	//2~8

//�����ɿ���������
#define		C_KEY_OFFSHAKE  4	//2~8

//��׼��ֵ
#define		VOL_VALUE		25	//�޸Ĵ�ֵ�µĴ���������׼ֵ*2,��Χ20~127

//������
#define		SLU_VALUE		12	//��ָ���ºͷſ��л���������,��ֵ����С�ڵ���VOL_VALUE/2,��Χ1~127

//������׼ֵ�����ٶ�
#define		CM_BASE			10//����ʱ��ԼΪ(���������ʱ��4ms + ��������*1.5) * CM_BASE,��Χ6~127

//ͬʱ���������Ч����,������ֵ��Ϊ��Ч
#define		C_VALID_KEY		4	//��Χ1~16

//�������������Чʱ��,��ɨ��ʱ�䣨����4ms��Ϊ��׼��λ,Ϊ0ʱ������,��Χ0~65535
#define		C_VALID_TIME	(1000/4)*20//20S

//�����ȵȼ�
#define 	LMDVALUE		1//0~1,Խ��Խ����,Ϊ1ʱע���׼ֵ������׼ֵ

//�ڲ���������
#define		KEY_EN1KRES		0//0~1

/*-------------------------------------------------------
�������Ϊͨ�����������޸�
-------------------------------------------------------*/
// �쳣��⿪������TKֵ�쳣�ή����Ӧ��Ӧ
#define UNNOL_CHECK 1   // 0~1
// �쳣���ָ��������������ٴ�������ָ�
#define UNNOL_TIMES 100 // 1~100
// ������ѹ
#define KEY_LDOSEL  4   // 0~7
// ���ʱ��Ƶ��
#define KEY_FRE     1   // 1~3
/***********************************************************
������������޸�
***********************************************************/
const uint8_t  _CMS_KeyTNumber = C_KEY_TOTAL;
const uint8_t  _CMS_KeyShakeC = C_KEY_SHAKE;
const uint8_t  _CMS_KeyOffC = C_KEY_OFFSHAKE;
const uint8_t  _CMS_VolValue = VOL_VALUE;
const uint8_t  _CMS_SluValue = SLU_VALUE;
const uint8_t  _CMS_CmBase = CM_BASE;
const uint8_t  _CMS_KeyValidC = C_VALID_KEY;
const uint16_t _CMS_KeyValidT = C_VALID_TIME;
//-------------------------------------------------------
const uint8_t _CMS_UnNolCheck = UNNOL_CHECK;
const uint8_t _CMS_UnNolTimes = UNNOL_TIMES;
const uint8_t _CMS_LMDValue = LMDVALUE;
const uint8_t _CMS_KeyLDOSel = KEY_LDOSEL;
const uint8_t _CMS_KeyEn1kres = KEY_EN1KRES;
const uint8_t _CMS_KeyFre = KEY_FRE;

unsigned int _CMS_DispKeyDown[C_KEY_TOTAL];
unsigned int _CMS_NoiseDat[C_KEY_TOTAL];
unsigned int _CMS_FactorRawDat[C_KEY_TOTAL];
//--------------------------------------------------
#endif
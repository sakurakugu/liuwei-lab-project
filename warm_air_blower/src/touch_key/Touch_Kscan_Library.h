// 这个 CMS79F726 的 LIB_TYPE_PT_NORMAL 库 是由 CTouch V2.3.3 生成的

/*

*/
#ifndef	_CHECKTOUCHKEY_H_
#define	_CHECKTOUCHKEY_H_

#include "TouchKeyConst.h"
#pragma warning disable 752,759
//接口函数声明
extern void __CMS_CheckTouchKey(void);		//触摸扫描主程序函数,建议在2.5ms~5ms内调用
extern void __CMS_GetTouchKeyValue(void);//此函数放在中断，建议中断扫描时间125us
extern volatile bit _CMS_b_kerr;				//复位错误标志
//每个按键对应寄存器的1个位,可根据需要一一对应
//_CMS_KeyFlag[0]对应于Key8,...,Key2,Key1
//_CMS_KeyFlag[1]对应于Key16,...,Key10,Key9
//按键有效时对应位为1,无效时为0
extern volatile uint8_t _CMS_KeyFlag[2];
//-------------------------------------------------------
//使用时请根据实际硬件情况修改下面的配置参数
//注意数组大小不能小于检测按键数量

// 触摸通道设定,为C_KCHS0到C_KCHS25中的一个
const uint8_t _CMS_Table_KeyChannel[] = {
    C_KCHS15, // Key1 - 电源按键 (RB0)
    C_KCHS14, // Key2 - 摆头按键 (RB1)
    C_KCHS13, // Key3 - 模式按键 (RB2)
    C_KCHS12, // Key4 - 定时按键 (RB3)

};

// 按键按下阈值,设定值可调范围20,最大不超过1023
// 此按键按下阈值设定请参考基准值VOL_VALUE
const uint16_t _CMS_Table_KeyDown[] = {
    50, // Key1 - 电源按键
    50, // Key2 - 摆头按键
    50, // Key3 - 模式按键
    50, // Key4 - 定时按键

};

//-------------------------------------------------------
//-------------------------------------------------------
//-------------------------------------------------------
/*
下面津贴有效配置参数
*/
//检测按键总数,最大16
#define		C_KEY_TOTAL		4	//最多支持16通道

//按键按下消抖次数
#define		C_KEY_SHAKE		4	//2~8

//按键松开消抖次数
#define		C_KEY_OFFSHAKE  4	//2~8

//基准阈值
#define		VOL_VALUE		25	//修改此值下的触摸按键基准值*2,范围20~127

//灵敏度
#define		SLU_VALUE		12	//手指按下和放开切换的灵敏度,此值必须小于等于VOL_VALUE/2,范围1~127

//按键基准值更新速度
#define		CM_BASE			10//更新时间约为(主程序调用时间4ms + 按键个数*1.5) * CM_BASE,范围6~127

//同时按键最大有效个数,超过此值将为无效
#define		C_VALID_KEY		4	//范围1~16

//按键最大连续有效时间,以扫描时间（建议4ms）为基准单位,为0时不限制,范围0~65535
#define		C_VALID_TIME	(1000/4)*20//20S

//灵敏度等级
#define 	LMDVALUE		1//0~1,越大越灵敏,为1时注意基准值及检测基准值

//内部上拉电阻
#define		KEY_EN1KRES		0//0~1

/*-------------------------------------------------------
下面参数为通用配置请勿修改
-------------------------------------------------------*/
// 异常检测开启，当TK值异常会降低相应响应
#define UNNOL_CHECK 1   // 0~1
// 异常检测恢复次数，连续多少次正常后恢复
#define UNNOL_TIMES 100 // 1~100
// 工作电压
#define KEY_LDOSEL  4   // 0~7
// 检测时钟频率
#define KEY_FRE     1   // 1~3
/***********************************************************
下面参数请勿修改
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
#ifndef _IIC_SOFT_H_
#define _IIC_SOFT_H_
#include <cms.h>

//�ܽŶ���
#define   I2C_SCL   RC1
#define   I2C_SDA   RC0
volatile bit   I2C_SCL_IO  @((unsigned)&TRISC*8)+1;  //IB�ڷ�������
volatile bit   I2C_SDA_IO  @((unsigned)&TRISC*8)+0;  //IB�ڷ�������

unsigned char I2C_Read1bYTE(unsigned char ack);			//���ڴ������湤��
unsigned char I2C_Write1bYTE(unsigned char data);
void I2C_WriteStop();
void I2C_WriteStart();
void I2C_WaitMoment();


#endif
#include "iic_soft.h"


/******************************************************************
�������ܣ�IIC�����źź���
��    ע����ʼ�ź�-��SCLΪ�ߵ�ƽʱ��SDA���Ϸ����Ӹߵ��͵ĵ�ƽ�仯
*******************************************************************/
void I2C_WriteStart()
{
	I2C_SDA_IO = 0;
	I2C_SCL_IO = 0;
	I2C_WaitMoment();

	I2C_SDA = 1;				//�Ƚ�SDA=1����׼����SCL=1ʱ����SDA=0
	I2C_SCL = 1;				//ʱ����������
	I2C_WaitMoment();
	I2C_SDA = 0;				//SCL=1ʱ����SDA���ͼ����������ź�

	I2C_WaitMoment();
	I2C_SCL = 0;				//��SCL=0����������źŲ���
}

/******************************************************************
�������ܣ�IIC�����źź���
��    ע�������ź�-��SCLΪ�ߵ�ƽʱ��SDA���Ϸ����ӵ͵��ߵĵ�ƽ�仯
*******************************************************************/
void I2C_WriteStop()
{
	I2C_SDA_IO = 0;
	I2C_SCL_IO = 0;
	I2C_WaitMoment();

	I2C_SDA = 0;				//�Ƚ�SDA=0����׼����SCL=1ʱ����SDA=1
	I2C_SCL = 1;				//ʱ����������
	I2C_WaitMoment();
	I2C_SDA = 1;				//SCL=1ʱ����SDA���߼�����ֹͣ�ź�

	I2C_WaitMoment();
	I2C_SCL = 0;				//��SCL=0����������źŲ���
}

/***********************************************************
�������ܣ�ģ��IIC��������8λ����
��ڲ�����data-����������
�������أ�����ackӦ���ź�
��    ע��IICЭ��ackӦ���ź�Ϊ����Ч
***********************************************************/
unsigned char I2C_Write1bYTE(unsigned char data)
{
	unsigned char i = 8;
	I2C_SDA_IO = 0;
	I2C_SCL_IO = 0;
	I2C_WaitMoment();
	do
	{
		I2C_SCL = 0;
		if (data & 0x80)
			I2C_SDA = 1;
		else
			I2C_SDA = 0;

		I2C_WaitMoment();
		I2C_SCL = 1;			//ʱ������һ�������أ���һλ����д��
		data <<= 1;
		I2C_WaitMoment();
	} while (--i);

	I2C_SCL = 0;
	I2C_SDA_IO = 1;				//��Ϊ����ڽ���Ӧ���ź�
	I2C_WaitMoment();

	I2C_SCL = 1;				//����������׼����Ӧ���ź�
	I2C_WaitMoment();

	i = I2C_SDA;
	I2C_SCL = 0;
	I2C_SDA_IO = 0;
	return i;
}

/***********************************************************
�������ܣ�ģ��IIC��������8λ����
��ڲ�����ack-Ӧ���ź�
�������أ����յ�������
��    ע��IICЭ��ackӦ���ź�Ϊ����Ч
***********************************************************/
unsigned char I2C_Read1bYTE(unsigned char ack)
{
	unsigned char i, data;
	I2C_SDA_IO = 1;
	I2C_SCL_IO = 1;
	I2C_WaitMoment();

	while (!I2C_SCL);			//�ȴ��ӻ�׼���������ͷ�ʱ��
	I2C_SCL_IO = 0;
	I2C_SCL = 0;
	I2C_WaitMoment();

	for (i = 0; i < 8; i++)
	{
		data <<= 1;
		if (I2C_SDA) data |= 0x1;

		I2C_SCL = 1;
		I2C_WaitMoment();

		I2C_SCL = 0;
		I2C_WaitMoment();
	}

	I2C_SDA_IO = 0;
	if (ack)I2C_SDA = 1;			//�Ƿ���Ҫ���������
	else I2C_SDA = 0;
	I2C_WaitMoment();

	I2C_SCL = 1;
	I2C_WaitMoment();

	I2C_SCL = 0;
	return data;
}

//�Ǿ�׼��ʱ
void I2C_WaitMoment()
{
	asm("nop");
	asm("nop");
	asm("nop");
	asm("nop");

	asm("nop");
	asm("nop");
	asm("nop");
	asm("nop");
}


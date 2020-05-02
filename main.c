#include "REG51.h"
#include "oled.h"
#include "bmp.h"
#include "24L01.h"
#include "ds1302.h"

#define uchar unsigned char //�궨���޷����ַ���
#define uint unsigned int	//�궨���޷�������

sbit LED = P3 ^ 5;		//�ߵ�ƽ����
sbit MQ7_DOUT = P1 ^ 0; //�����������
sbit BEEP = P2 ^ 2;		//������  �͵�ƽ����
sbit key2 = P2 ^ 0;
sbit key3 = P2 ^ 1;

uchar flag = 0;
unsigned char tf = 0;
unsigned char TxBuf[10] = {0};
unsigned char RxBuf[10] = {0};

uchar DisplayData[12];

void delay() //��ʱ����
{
	uchar m, n, s;
	for (m = 20; m > 0; m--)
		for (n = 20; n > 0; n--)
			for (s = 248; s > 0; s--)
				;
}

void datapros()
{
	Ds1302ReadTime(); //��ȡʱ��

	DisplayData[11] = TIME[6] >> 4;	  //��   /16==>>4�����ƶ�4λ��
	DisplayData[10] = TIME[6] & 0x0f; //0000 1111 ȡ��λ

	DisplayData[9] = TIME[4] / 16;	 //��
	DisplayData[8] = TIME[4] & 0x0f; //0000 1111 ȡ��λ

	DisplayData[7] = TIME[3] / 16;	 //��
	DisplayData[6] = TIME[3] & 0x0f; //0000 1111 ȡ��λ

	DisplayData[5] = TIME[2] >> 4;	 //ʱ   /16==>>4�����ƶ�4λ��
	DisplayData[4] = TIME[2] & 0x0f; //0000 1111 ȡ��λ

	DisplayData[3] = TIME[1] / 16;	 //��
	DisplayData[2] = TIME[1] & 0x0f; //0000 1111 ȡ��λ

	DisplayData[1] = TIME[0] / 16;	 //��
	DisplayData[0] = TIME[0] & 0x0f; //0000 1111 ȡ��λ
}

int main(void)
{

	OLED_Init();
	OLED_Clear();
	init_NRF24L01();
	Ds1302Init();

	BEEP = 0;
	LED = 1;
	Delay_1ms(300);
	BEEP = 1;
	LED = 0;

	P0 = 0x00;
	nRF24L01_TxPacket(TxBuf);
	Delay(6000);
	P0 = 0xBF;

	TxBuf[1] = 0x00;
	TxBuf[2] = 0x00;
	TxBuf[3] = 0x00;
	TxBuf[4] = 0x00;

	OLED_ShowString(0, 0, "Receive", 16);
	//OLED_ShowString(0, 2, "20/05/01/12/30", 16);

	

	while (1)
	{
		datapros();

		OLED_ShowString(0, 0, "Receive", 16);

		OLED_ShowNum(0, 2, DisplayData[11], 1, 16); //��
		OLED_ShowNum(8, 2, DisplayData[10], 1, 16);
		OLED_ShowString(16, 2, "/", 16);

		OLED_ShowNum(24, 2, DisplayData[9], 1, 16); //��
		OLED_ShowNum(32, 2, DisplayData[8], 1, 16);

		OLED_ShowNum(40, 2, DisplayData[7], 1, 16); //��
		OLED_ShowNum(48, 2, DisplayData[6], 1, 16);
		OLED_ShowString(56, 2, "/", 16);

		OLED_ShowNum(64, 2, DisplayData[5], 1, 16); //ʱ
		OLED_ShowNum(72, 2, DisplayData[4], 1, 16);
		OLED_ShowString(80, 2, ":", 16);

		OLED_ShowNum(88, 2, DisplayData[3], 1, 16); //��
		OLED_ShowNum(96, 2, DisplayData[2], 1, 16);
		OLED_ShowString(104, 2, "/", 16);

		OLED_ShowNum(112, 2, DisplayData[1], 1, 16); //��
		OLED_ShowNum(120, 2, DisplayData[0], 1, 16);

		//�������Ͳ���
		if (key2 == 0)
		{
			TxBuf[1] = 0x00;
			TxBuf[2] = 0x00;
			TxBuf[3] = 0x00;
			TxBuf[4] = 0x00;
			TxBuf[5] = 5;

			nRF24L01_TxPacket(TxBuf);
			TxBuf[5] = 0x00;

			LED = 1;
			BEEP = 0;
			OLED_ShowString(0, 0, "Receive Test", 16);
		}
		else
		{
			LED = 0;
			BEEP = 1;
		}

		//�ֶ�ȡ������
		//		if (key3 == 0)
		//		{
		//			BEEP = 1;
		//			SendSafe(); //���Ͱ�ȫָ��
		//		}

		SetRX_Mode();

		RxBuf[1] = 0x00;
		RxBuf[2] = 0x00;
		RxBuf[3] = 0x00;
		RxBuf[4] = 0x00;
		RxBuf[5] = 0x00;

		Delay(1000);
		nRF24L01_RxPacket(RxBuf);

		if (RxBuf[1] | RxBuf[2] | RxBuf[3] | RxBuf[4])
		{
			if (RxBuf[1] == 1) //�ӻ�����
			{
				BEEP = 0;
				LED = 1;
				OLED_ShowString(0, 4, "Alert 1", 16);
			}
			else
			{
				LED = 0;
				BEEP = 1;
			}

			if (RxBuf[2] == 2) //�ӻ�����1����
			{
				BEEP = 0;
				LED = 1;
				OLED_ShowString(0, 4, "Test Alert 1", 16);
			}
			else
			{
				LED = 0;
				BEEP = 1;
			}

			if (RxBuf[3] == 3) //�����2�ͱ���
			{
				BEEP = 0;
				LED = 1;
				OLED_ShowString(0, 6, "Alert 2", 16);
			}
			else
			{
				LED = 0;
				BEEP = 1;
			}
			if (RxBuf[4] == 4) //�ӻ�����2����
			{
				BEEP = 0;
				LED = 1;
				OLED_ShowString(0, 6, "Test Alert 2", 16);
			}
			else
			{
				LED = 0;
				BEEP = 1;
			}

			//Delay(6000);
		}

		RxBuf[1] = 0x00;
		RxBuf[2] = 0x00;
		RxBuf[3] = 0x00;
		RxBuf[4] = 0x00;
		RxBuf[5] = 0x00;

		TxBuf[1] = 0x00;
		TxBuf[2] = 0x00;
		TxBuf[3] = 0x00;
		TxBuf[4] = 0x00;
	}
}

#include "REG51.h"
#include "oled.h"
#include "bmp.h"
#include "24L01.h"

#define uchar unsigned char //宏定义无符号字符型
#define uint unsigned int	//宏定义无符号整型

sbit LED = P3 ^ 5;		//高电平发光
sbit MQ7_DOUT = P1 ^ 0; //传感器输入端
sbit BEEP = P2 ^ 2;		//蜂鸣器  低电平触发
sbit key2 = P2 ^ 0;
sbit key3 = P2 ^ 1;

uchar flag = 0;
unsigned char tf = 0;
unsigned char TxBuf[10] = {0};
unsigned char RxBuf[10] = {0};

void delay() //延时程序
{
	uchar m, n, s;
	for (m = 20; m > 0; m--)
		for (n = 20; n > 0; n--)
			for (s = 248; s > 0; s--)
				;
}

int main(void)
{

	OLED_Init();
	OLED_Clear();
	init_NRF24L01();

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
	OLED_ShowString(0, 2, "Time:", 16);

	while (1)
	{
		OLED_ShowString(0, 0, "Receive", 16);

		//按键发送测试
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

		//手动取消报警
		//		if (key3 == 0)
		//		{
		//			BEEP = 1;
		//			SendSafe(); //发送安全指令
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
			if (RxBuf[1] == 1) //从机报警
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

			if (RxBuf[2] == 2) //从机按键1测试
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

			if (RxBuf[3] == 3) //如果是2就报警
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
			if (RxBuf[4] == 4) //从机按键2测试
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

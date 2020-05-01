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

	OLED_ShowString(0, 0, "Transmit 1", 16);
	OLED_ShowString(0, 2, "Safety", 16);
	while (1)
	{
		OLED_ShowString(0, 0, "Transmit 1", 16);

		//从机一氧化碳部分程序
		if (MQ7_DOUT == 0) //当浓度高于设定值时 ，执行条件函数
		{
			//delay();
			if (MQ7_DOUT == 0) //确定 浓度高于设定值时 ，执行条件函数
			{
				BEEP = 0;
				LED = 1;
				TxBuf[1] = 0x00;
				TxBuf[2] = 0x00;
				TxBuf[3] = 0x00;
				TxBuf[4] = 0x00;
				TxBuf[1] = 1;
				nRF24L01_TxPacket(TxBuf);
				TxBuf[1] = 0; //发送报警指令
				OLED_ShowString(0, 2, "Danger", 16);
			}
		}
		else
		{
			BEEP = 1;
			LED = 0;
			OLED_ShowString(0, 2, "Safety", 16);
		}

		//按键发送测试
		if (key2 == 0)
		{
			TxBuf[1] = 0x00;
			TxBuf[2] = 2;
			TxBuf[3] = 0x00;
			TxBuf[4] = 0x00;

			nRF24L01_TxPacket(TxBuf);
			TxBuf[2] = 0x00;

			LED = 1;
			BEEP = 0;
			OLED_ShowString(0, 4, "Tx1 Test", 16);
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

        //接收模式
		SetRX_Mode();

		RxBuf[1] = 0x00;
		RxBuf[2] = 0x00;
		RxBuf[3] = 0x00;
		RxBuf[4] = 0x00;
		RxBuf[5] = 0x00;

		Delay(1000);
		nRF24L01_RxPacket(RxBuf);

		if (RxBuf[5])
		{
			if (RxBuf[5] == 5) //按键测试
			{
				LED = 1;
				BEEP = 0;
			}
			else
			{
				LED = 0;
				BEEP = 1;
			}
			Delay(6000);
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

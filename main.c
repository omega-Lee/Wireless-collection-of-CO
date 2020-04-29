#include "REG51.h"
#include "oled.h"
#include "bmp.h"
#include "24C02.h"
#include "ds1302.h"

#define uchar unsigned char//宏定义无符号字符型
#define uint unsigned int  //宏定义无符号整型

sbit LED=P3^5;	  //高电平发光
sbit MQ7_DOUT=P1^0;	 //传感器输入端
sbit BEEP=P2^2; //蜂鸣器  低电平触发

uchar flag=0;

void delay()//延时程序
{
	uchar m,n,s;
	for(m=20;m>0;m--)
		for(n=20;n>0;n--)
			for(s=248;s>0;s--);
}

 int main(void)
 {	
	OLED_Init();		
	OLED_Clear() ; 

	while(1) 
	{		
		OLED_ShowString(0,2,"Time:",16);
		if(MQ7_DOUT==0)//当浓度高于设定值时 ，执行条件函数
		{
			delay();//延时抗干扰
			if(MQ7_DOUT==0)//确定 浓度高于设定值时 ，执行条件函数
			{	
				OLED_Clear(); 
				OLED_ShowString(0,0,"Danger!!!",16);  
				BEEP=0;		   
			}
		}
		else
		{	 
			BEEP=1;
			OLED_ShowString(0,0,"Safety",16);
		}

			
	}	  
	
}
	
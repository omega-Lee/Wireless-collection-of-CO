#include "REG51.h"
#include "oled.h"
#include "bmp.h"
#include "24C02.h"
#include "ds1302.h"

#define uchar unsigned char//�궨���޷����ַ���
#define uint unsigned int  //�궨���޷�������

sbit LED=P3^5;	  //�ߵ�ƽ����
sbit MQ7_DOUT=P1^0;	 //�����������
sbit BEEP=P2^2; //������  �͵�ƽ����

uchar flag=0;

void delay()//��ʱ����
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
		if(MQ7_DOUT==0)//��Ũ�ȸ����趨ֵʱ ��ִ����������
		{
			delay();//��ʱ������
			if(MQ7_DOUT==0)//ȷ�� Ũ�ȸ����趨ֵʱ ��ִ����������
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
	
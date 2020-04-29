#include "reg51.h"
#include "24C02.h"

//��ʱ10us
void Delay_24C02_24C02()
{
	uchar a,b;
	for(b=1;b>0;b--)
	{
		for(a=2;a>0;a--);
	}
}

void IIC_Start_24C02()
{
	//��ʱ����Ϊ�ߵ������ �����߲����½��ر�ʾ ��ʼ�ź� �ߵ͵�ƽ���Ա��ִ���4.7us
	SDA=1;
	Delay_24C02();
	SCL=1;
	Delay_24C02();
	SDA=0;
	Delay_24C02();
	SCL=0;//ʱ�������� ���������߱仯
	Delay_24C02();
}

void IIC_Stop_24C02()
{
	//��ʱ����Ϊ�ߵ�ƽ����� �����߲��������ر�ʾΪ��ֹ �Ҹ��Ե�ƽ���ִ���4.7us
	SDA=0;
	Delay_24C02();
	SCL=1;
	Delay_24C02();
	SDA=1;
	Delay_24C02();
}

//����һ���ֽ�
uchar IIC_Send_Byte(uchar dat)
{
	uchar i,cout;
	for(i=0;i<8;i++)
	{
		//���λ�ȡdat��ÿһλ����
		SDA=dat>>7;//ÿ�ζ���ȡ���λ
		dat=dat<<1;//ÿ��ȥ�����λ
		Delay_24C02();
		SCL=1;//ʱ�������� �����߲�����仯
		Delay_24C02();
		SCL=0;//ʱ�������� ����������仯
	}
	SDA=1;//����������
	Delay_24C02();
	SCL=1;//ʱ�������� ���ݲ�����仯
	//�ȴ�Ӧ�� ��ʱ����Ϊ�ߵ�ƽ������� �����߱����ͱ���ΪӦ�� ��֮Ϊ��Ӧ��
	while(SDA)
	{
		cout++;//�ȴ�Ӧ�������;
		//�������2000usû��Ӧ��SDAû�б����ͣ�����ʧ�ܣ�����Ϊ��Ӧ��
		if(cout>200)	 //�������2000usû��Ӧ����ʧ�ܣ�����Ϊ��Ӧ�𣬱�ʾ���ս���
		{
			SCL=0;
			Delay_24C02();
			return 0;
		}
	}
	SCL=0;
	Delay_24C02();
	return 1;
}

//��ȡһ���ֽ�
uchar IIC_Read_Byte()
{
	uchar i,dat;
	SDA=1;//���������� Ϊ����״̬ �ȴ���ת
	for(i=0;i<8;i++)
	{
		SCL=1;//ʱ�������� ���������ݱ仯
		Delay_24C02();
		dat<<=1;//�ߵ�ַ�����ȴ��� ÿ������һλ ÿ������һ��
		dat|=SDA;//��ȡһλ����
		Delay_24C02();
		SCL=0;//ʱ���߱����� �������ݱ仯
		Delay_24C02();
	}
	return dat;//����һ�ֽ�����
}



//��24C02����һ���ֽ�
void Write_24C02(uchar addr,uchar dat)
{
	IIC_Start_24C02();//��ʼ
	IIC_Send_Byte(0xa0);//д��������ַ
	IIC_Send_Byte(addr);//д���ڴ��ַ
	IIC_Send_Byte(dat);//д������
	IIC_Stop_24C02();//����
}

//��ȡ24C02��ĳ����ַ������
uchar Read_24C02(uchar addr)
{
	uchar dat;
	IIC_Start_24C02();//��ʼ
	IIC_Send_Byte(0xa0);//д��������ַ
	IIC_Send_Byte(addr);//д���ַ
	IIC_Start_24C02();
	IIC_Send_Byte(0xa1);//д���ָ��
	dat=IIC_Read_Byte();
	IIC_Stop_24C02();
	return dat;
}
	
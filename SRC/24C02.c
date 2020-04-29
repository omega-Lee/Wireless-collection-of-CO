#include "reg51.h"
#include "24C02.h"

//延时10us
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
	//在时钟线为高的情况下 数据线产生下降沿表示 开始信号 高低电平各自保持大于4.7us
	SDA=1;
	Delay_24C02();
	SCL=1;
	Delay_24C02();
	SDA=0;
	Delay_24C02();
	SCL=0;//时钟线拉低 运行数据线变化
	Delay_24C02();
}

void IIC_Stop_24C02()
{
	//在时钟线为高电平情况下 数据线产生上升沿表示为终止 且各自电平保持大于4.7us
	SDA=0;
	Delay_24C02();
	SCL=1;
	Delay_24C02();
	SDA=1;
	Delay_24C02();
}

//发送一个字节
uchar IIC_Send_Byte(uchar dat)
{
	uchar i,cout;
	for(i=0;i<8;i++)
	{
		//依次获取dat的每一位数据
		SDA=dat>>7;//每次都获取最高位
		dat=dat<<1;//每次去除最高位
		Delay_24C02();
		SCL=1;//时钟线拉高 数据线不允许变化
		Delay_24C02();
		SCL=0;//时钟线拉低 数据线允许变化
	}
	SDA=1;//数据线拉高
	Delay_24C02();
	SCL=1;//时钟线拉高 数据不允许变化
	//等待应答 在时钟线为高电平的情况下 数据线被拉低表现为应答 反之为非应答
	while(SDA)
	{
		cout++;//等待应答计数器;
		//如果超过2000us没有应答（SDA没有被拉低）发送失败，或者为非应答
		if(cout>200)	 //如果超过2000us没有应答发送失败，或者为非应答，表示接收结束
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

//读取一个字节
uchar IIC_Read_Byte()
{
	uchar i,dat;
	SDA=1;//数据线拉高 为空闲状态 等待反转
	for(i=0;i<8;i++)
	{
		SCL=1;//时钟线拉高 不允许数据变化
		Delay_24C02();
		dat<<=1;//高地址数据先传入 每次移入一位 每次左移一次
		dat|=SDA;//获取一位数据
		Delay_24C02();
		SCL=0;//时钟线被拉低 允许数据变化
		Delay_24C02();
	}
	return dat;//返回一字节数据
}



//向24C02发送一个字节
void Write_24C02(uchar addr,uchar dat)
{
	IIC_Start_24C02();//开始
	IIC_Send_Byte(0xa0);//写入器件地址
	IIC_Send_Byte(addr);//写入内存地址
	IIC_Send_Byte(dat);//写入数据
	IIC_Stop_24C02();//结束
}

//读取24C02的某个地址的数据
uchar Read_24C02(uchar addr)
{
	uchar dat;
	IIC_Start_24C02();//开始
	IIC_Send_Byte(0xa0);//写入器件地址
	IIC_Send_Byte(addr);//写入地址
	IIC_Start_24C02();
	IIC_Send_Byte(0xa1);//写入读指令
	dat=IIC_Read_Byte();
	IIC_Stop_24C02();
	return dat;
}
	
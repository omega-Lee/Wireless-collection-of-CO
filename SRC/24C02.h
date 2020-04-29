#ifndef __24C02_H_
#define __24C02_H_

typedef unsigned int uint;
typedef unsigned char uchar;

sbit SDA=P2^6;
sbit SCL =P2^7;

void Delay_24C02();
void IIC_Start_24C02();
void IIC_Stop_24C02();
uchar IIC_Send_Byte(uchar dat);
uchar IIC_Read_Byte();

void Write_24C02(uchar add,uchar dat);
uchar Read_24C02(uchar add);

#endif
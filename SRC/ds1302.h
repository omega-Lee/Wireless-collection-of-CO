#ifndef __DS1302_H_
#define __DS1302_H_

//---����ͷ�ļ�---//
#include<REG51.h>
#include<intrins.h>

//---�ض���ؼ���---//
#ifndef uchar
#define uchar unsigned char
#endif

#ifndef uint 
#define uint unsigned int
#endif

//---����ds1302ʹ�õ�IO��---//
sbit DSIO=P2^4;
sbit RST=P2^3;
sbit SCLK=P2^5;

//---����ȫ�ֺ���---//
void Ds1302Write(uchar addr, uchar dat);
uchar Ds1302Read(uchar addr);
void Ds1302Init();
void Ds1302ReadTime();

//---����ȫ�ֱ���--//
extern uchar TIME[7];	//����ȫ�ֱ���

#endif
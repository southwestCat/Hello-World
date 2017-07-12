#ifndef __ADC_H
#define __ADC_H	
#include "sys.h"
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEKս��STM32������
//ADC ����	   
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//�޸�����:2012/9/7
//�汾��V1.0
//��Ȩ���У�����ؾ���
//Copyright(C) ������������ӿƼ����޹�˾ 2009-2019
//All rights reserved									  
////////////////////////////////////////////////////////////////////////////////// 

#define N 1	//ÿ��ͨ������N��
#define M 4 	//����ͨ������
#define SAMPLE_LEN 50
#define DRAW_POINT_NUM 100
#define DELTA 100

extern vu16 AD_Value[N][M];
extern vu16 After_filter[M];

void Adc_Init(void);
u16  Get_Adc(u8 ch); 
u16 Get_Adc_Average(u8 ch,u8 times); 

void Adc_Init(void);
void Auto_drive(void);
void Draw_wave(void);
u16 KalmanFilter0 (u16 ResrcData);
u16 KalmanFilter1 (u16 ResrcData);
u16 KalmanFilter2 (u16 ResrcData);
u16 KalmanFilter3 (u16 ResrcData);

 
#endif 

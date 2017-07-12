 #include "adc.h"
 #include "delay.h"
 #include "motor.h"
 #include "lcd.h"
//////////////////////////////////////////////////////////////////////////////////	 
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
	   
		   
//��ʼ��ADC
//�������ǽ��Թ���ͨ��Ϊ��
//����Ĭ�Ͻ�����ͨ��0~3			

static u8 n=0,n_up=0,n_down=0,n_left=0,n_right=0;

void  Adc_Init(void)
{ 	
	ADC_InitTypeDef ADC_InitStructure; 

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE );	  //ʹ��ADC1ͨ��ʱ��
	
	RCC_ADCCLKConfig(RCC_PCLK2_Div6);   //����ADC��Ƶ����6 72M/6=12,ADC���ʱ�䲻�ܳ���14M

	ADC_DeInit(ADC1);  //��λADC1 

	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;	//ADC����ģʽ:ADC1��ADC2�����ڶ���ģʽ
	ADC_InitStructure.ADC_ScanConvMode = DISABLE;	//ģ��ת�������ڵ�ͨ��ģʽ
	ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;	//ģ��ת�������ڵ���ת��ģʽ
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;	//ת��������������ⲿ��������
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;	//ADC�����Ҷ���
	ADC_InitStructure.ADC_NbrOfChannel = 1;	//˳����й���ת����ADCͨ������Ŀ
	ADC_Init(ADC1, &ADC_InitStructure);	//����ADC_InitStruct��ָ���Ĳ�����ʼ������ADCx�ļĴ���   
	
	//����ָ��ADC�Ĺ�����ͨ�����������ǵ�ת��˳��Ͳ���ʱ��
	//ADC1,ADCͨ��x,�������˳��ֵΪy,����ʱ��Ϊ239.5����
//	ADC_RegularChannelConfig(ADC1, ADC_Channel_0, 1, ADC_SampleTime_239Cycles5 );
//	ADC_RegularChannelConfig(ADC1, ADC_Channel_1, 2, ADC_SampleTime_239Cycles5 );
//	ADC_RegularChannelConfig(ADC1, ADC_Channel_2, 3, ADC_SampleTime_239Cycles5 );
//	ADC_RegularChannelConfig(ADC1, ADC_Channel_3, 4, ADC_SampleTime_239Cycles5 );
//	ADC_RegularChannelConfig(ADC1, ADC_Channel_8, 5, ADC_SampleTime_239Cycles5 );
//	ADC_RegularChannelConfig(ADC1, ADC_Channel_9, 6, ADC_SampleTime_239Cycles5 );
//	ADC_RegularChannelConfig(ADC1, ADC_Channel_10, 7, ADC_SampleTime_239Cycles5 );
//	ADC_RegularChannelConfig(ADC1, ADC_Channel_11, 8, ADC_SampleTime_239Cycles5 );
//	ADC_RegularChannelConfig(ADC1, ADC_Channel_12, 9, ADC_SampleTime_239Cycles5 );
//	ADC_RegularChannelConfig(ADC1, ADC_Channel_13, 10, ADC_SampleTime_239Cycles5 );
//	ADC_RegularChannelConfig(ADC1, ADC_Channel_14, 11, ADC_SampleTime_239Cycles5 );
//	ADC_RegularChannelConfig(ADC1, ADC_Channel_15, 12, ADC_SampleTime_239Cycles5 );

//	ADC_RegularChannelConfig(ADC1, ADC_Channel_1, 1, ADC_SampleTime_239Cycles5 );
//	ADC_RegularChannelConfig(ADC1, ADC_Channel_10, 2, ADC_SampleTime_239Cycles5 );
	
	ADC_DMACmd(ADC1, DISABLE);
  
	ADC_Cmd(ADC1, ENABLE);	//ʹ��ָ����ADC1
	
	ADC_ResetCalibration(ADC1);	//ʹ�ܸ�λУ׼  
	 
	while(ADC_GetResetCalibrationStatus(ADC1));	//�ȴ���λУ׼����
	
	ADC_StartCalibration(ADC1);	 //����ADУ׼
 
	while(ADC_GetCalibrationStatus(ADC1));	 //�ȴ�У׼����
 
}				  
//���ADCֵ
//ch:ͨ��ֵ 0~3
u16 Get_Adc(u8 ch)   
{
  	//����ָ��ADC�Ĺ�����ͨ����һ�����У�����ʱ��
	ADC_RegularChannelConfig(ADC1, ch, 1, ADC_SampleTime_239Cycles5 );	//ADC1,ADCͨ��,����ʱ��Ϊ239.5����	  			    
  
	ADC_SoftwareStartConvCmd(ADC1, ENABLE);		//ʹ��ָ����ADC1�����ת����������	
	 
	while(!ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC ));//�ȴ�ת������

	return ADC_GetConversionValue(ADC1);	//�������һ��ADC1�������ת�����
}

u16 Get_Adc_Average(u8 ch,u8 times)
{
	u32 temp_val=0;
	u8 t;
	for(t=0;t<times;t++)
	{
		temp_val+=Get_Adc(ch);
		delay_ms(5);
	}
	return temp_val/times;
} 	 

void Draw_wave()
{
	u16 Get_Max[M][DRAW_POINT_NUM];
	u16 Get_Max_KAL[M][DRAW_POINT_NUM];
	static int dot_last_x1=30,dot_last_y1=50,dot_now_x1=30,dot_now_y1=50;
	static int kal_dot_last_y1=270,kal_dot_now_y1=270;

	int i;
	
	LCD_Clear(WHITE);
	
	for (i=0;i<DRAW_POINT_NUM;i++)
	{
		dot_last_x1=dot_now_x1;
		dot_last_y1=dot_now_y1;
		kal_dot_last_y1=kal_dot_now_y1;
		
		Get_Max[0][i]=Get_Adc(ADC_Channel_10);
		Get_Max[1][i]=Get_Adc(ADC_Channel_11);
		Get_Max[2][i]=Get_Adc(ADC_Channel_12);
		Get_Max[3][i]=Get_Adc(ADC_Channel_13);
		
		Get_Max_KAL[0][i]=KalmanFilter0(Get_Max[0][i]);
		Get_Max_KAL[1][i]=KalmanFilter1(Get_Max[1][i]);
		Get_Max_KAL[2][i]=KalmanFilter2(Get_Max[2][i]);
		Get_Max_KAL[3][i]=KalmanFilter3(Get_Max[3][i]);
		
		dot_now_x1+=2;
		dot_now_y1=Get_Max[0][i]*50/2048;
		if (dot_now_y1>100)
			dot_now_y1=100;
		kal_dot_now_y1=Get_Max_KAL[0][i]*50/2048+220;
		if (kal_dot_now_y1>320)
			kal_dot_now_y1=320;
		POINT_COLOR=MAGENTA;
		LCD_DrawLine(dot_last_x1,dot_last_y1,dot_now_x1,dot_now_y1);
		LCD_DrawLine(dot_last_x1,kal_dot_last_y1,dot_now_x1,kal_dot_now_y1);
//		LCD_Fast_DrawPoint(dot_now_x1,kal_dot_now_y1,BLACK);
		if(dot_now_x1>210)
		{
			dot_last_x1=30;
			dot_last_y1=50;
			dot_now_x1=30;
			dot_now_y1=50;
			kal_dot_last_y1=270;
			kal_dot_now_y1=270;
		}
	}
}

void Auto_drive()
{
	u16 max[M];
	u16 Get_Max[M][SAMPLE_LEN];
	u32 eve;
	int i,j;
	u16 forward,backward,go_left,go_right;
	
	for (i=0;i<SAMPLE_LEN;i++)
	{
		Get_Max[0][i]=Get_Adc(ADC_Channel_10);
		Get_Max[1][i]=Get_Adc(ADC_Channel_11);
		Get_Max[2][i]=Get_Adc(ADC_Channel_12);
		Get_Max[3][i]=Get_Adc(ADC_Channel_13);
	}

	for (j=0;j<M;j++)
	{
		max[j]=Get_Max[j][0];
	}  
	for(i=0;i<M;i++)
	{
		for(j=0;j<SAMPLE_LEN;j++)
		{
			if (Get_Max[i][j]>max[i])
				max[i]=Get_Max[i][j];
		}
	}

	forward=(max[0]+max[1])/2;
	backward=(max[2]+max[3])/2;
	forward+=100;
	go_left=(max[1]+max[3])/2;
	go_right=(max[0]+max[3])/2;
	go_left+=200;

	max[1]+=DELTA;
	max[0]+=DELTA;
	
	eve=(max[0]+max[1]+max[2]+max[3])/4;
	
	
	
	if(eve>3000)
		Motor_PWM_ChangeDuty(0);
	else
	{		
		if(forward>backward)
			Motor_PWM_ChangeDuty(1500);
		else
			Motor_PWM_ChangeDuty(-1500);
	}
	
	if(go_left-go_right>50)
	{
		Servo_ChangDuty(150,LEFT);
	}
	else if(go_right-go_left>50)
	{
		Servo_ChangDuty(150,RIGHT);
	}
	else
	{
		Servo_ChangDuty(150,MIDDLE);
	}
	
//	if ((max[0]>max[1])&&(max[2]>max[3]))
//	{
//		Servo_ChangDuty(150,RIGHT);
//	}
//	else if ((max[1]>max[0])&&(max[3]>max[2]))
//	{
//		Servo_ChangDuty(150,LEFT);
//	}
//	else
//	{
//		Servo_ChangDuty(150,MIDDLE);
//	}
}

u16 KalmanFilter0 (u16 ResrcData)
{
	double R = 0.0542;
	double Q = 0.018;

	static double x_last=0;

	double x_mid = x_last;
	double x_now;

	static double p_last=0;

	double p_mid;
	double p_now;
	double kg;
	u16 x;

	x_mid = x_last; //x_last=x(k-1|k-1),x_mid=x(k|k-1)
	p_mid = p_last + Q; //p_mid=p(k|k-1),p_last=p(k-1|k-1),Q=����
	kg = p_mid / (p_mid + R); //kgΪkalman filter��RΪ����
	x_now = x_mid + kg*(ResrcData - x_mid);//���Ƴ�������ֵ

	p_now = (1 - kg)*p_mid;//����ֵ��Ӧ��covariance       

	p_last = p_now; //����covarianceֵ
	x_last = x_now; //����ϵͳ״ֵ̬
	x=(u16)(x_now);
	return x;
}

u16 KalmanFilter1 (u16 ResrcData)
{
	double R = 0.0542;
	double Q = 0.018;

	static double x_last=0;

	double x_mid = x_last;
	double x_now;

	static double p_last=0;

	double p_mid;
	double p_now;
	double kg;
	u16 x;

	x_mid = x_last; //x_last=x(k-1|k-1),x_mid=x(k|k-1)
	p_mid = p_last + Q; //p_mid=p(k|k-1),p_last=p(k-1|k-1),Q=����
	kg = p_mid / (p_mid + R); //kgΪkalman filter��RΪ����
	x_now = x_mid + kg*(ResrcData - x_mid);//���Ƴ�������ֵ

	p_now = (1 - kg)*p_mid;//����ֵ��Ӧ��covariance       

	p_last = p_now; //����covarianceֵ
	x_last = x_now; //����ϵͳ״ֵ̬
	x=(u16)(x_now);
	return x;
}

u16 KalmanFilter2 (u16 ResrcData)
{
	double R = 0.0542;
	double Q = 0.018;

	static double x_last=0;

	double x_mid = x_last;
	double x_now;

	static double p_last=0;

	double p_mid;
	double p_now;
	double kg;
	u16 x;

	x_mid = x_last; //x_last=x(k-1|k-1),x_mid=x(k|k-1)
	p_mid = p_last + Q; //p_mid=p(k|k-1),p_last=p(k-1|k-1),Q=����
	kg = p_mid / (p_mid + R); //kgΪkalman filter��RΪ����
	x_now = x_mid + kg*(ResrcData - x_mid);//���Ƴ�������ֵ

	p_now = (1 - kg)*p_mid;//����ֵ��Ӧ��covariance       

	p_last = p_now; //����covarianceֵ
	x_last = x_now; //����ϵͳ״ֵ̬
	x=(u16)(x_now);
	return x;
}

u16 KalmanFilter3 (u16 ResrcData)
{
	double R = 0.0542;
	double Q = 0.018;

	static double x_last=0;

	double x_mid = x_last;
	double x_now;

	static double p_last=0;

	double p_mid;
	double p_now;
	double kg;
	u16 x;

	x_mid = x_last; //x_last=x(k-1|k-1),x_mid=x(k|k-1)
	p_mid = p_last + Q; //p_mid=p(k|k-1),p_last=p(k-1|k-1),Q=����
	kg = p_mid / (p_mid + R); //kgΪkalman filter��RΪ����
	x_now = x_mid + kg*(ResrcData - x_mid);//���Ƴ�������ֵ

	p_now = (1 - kg)*p_mid;//����ֵ��Ӧ��covariance       

	p_last = p_now; //����covarianceֵ
	x_last = x_now; //����ϵͳ״ֵ̬
	x=(u16)(x_now);
	return x;
}

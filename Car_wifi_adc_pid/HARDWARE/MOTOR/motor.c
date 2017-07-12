#include "stm32f10x.h"
#include "motor.h"
#include "delay.h"
#include "function.h"

u16 ServoDuty;

static float servo_Kp = 70, servo_Kd = 5;      //ǰ����Kp ��Kd	

void TIM8_Configuration(void)//TIM8��ͨ���ֱ����PWM1/PWM2,��Ӧ����ΪPC6/PC7��ch1 ch2
{
	GPIO_InitTypeDef GPIO_InitStructure; 
    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
    TIM_OCInitTypeDef TIM_OCInitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM8, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7 | GPIO_Pin_8 | GPIO_Pin_9;    
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;       
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;       
    GPIO_Init(GPIOC, &GPIO_InitStructure); 
	   
	TIM_TimeBaseStructure.TIM_Prescaler = 0;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseStructure.TIM_Period = PWMPeriod;
	TIM_TimeBaseStructure.TIM_ClockDivision = 0;
	TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;
	TIM_TimeBaseInit(TIM8, &TIM_TimeBaseStructure);
	
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2;
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_OutputNState = TIM_OutputNState_Disable;
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low;
	TIM_OCInitStructure.TIM_OCNPolarity = TIM_OCNPolarity_High;
	TIM_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Set;
	TIM_OCInitStructure.TIM_OCNIdleState = TIM_OCIdleState_Reset;

	TIM_OC1Init(TIM8, &TIM_OCInitStructure);
	TIM_OC2Init(TIM8, &TIM_OCInitStructure);
	
	TIM8->CCR1 = 0; //PC6�����������1.65V��ѹ  ���0.1V
	TIM8->CCR2 = 0; //PC7�����������1.98V��ѹ  ���0.1V

	TIM_Cmd(TIM8, ENABLE);
	TIM_CtrlPWMOutputs(TIM8, ENABLE);	

}

/////////////////////////////

void TIM3_Configuration(void)//�������ӿ����ã�TIM3��/PA6-A��  PA7-B��
{
    GPIO_InitTypeDef GPIO_InitStructure; 
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_ICInitTypeDef  TIM_ICInitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA , ENABLE);  //ʹ��GPIOA����ʱ��
	                                                       				
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6|GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;          //����Ϊ��������ģʽ 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	   //IO�ڵ�����Ϊ50M 
	GPIO_Init(GPIOA, &GPIO_InitStructure);				   //IO�����ú���
	GPIO_WriteBit(GPIOA, GPIO_Pin_6,Bit_SET);
	GPIO_WriteBit(GPIOA, GPIO_Pin_7,Bit_SET); 

   	TIM_TimeBaseStructure.TIM_Period = PWMPeriod; //��������һ�������¼�װ�����Զ���װ�ؼĴ������ڵ�ֵ
	TIM_TimeBaseStructure.TIM_Prescaler = 0; //����������ΪTIMxʱ��Ƶ�ʳ�����Ԥ��Ƶֵ  ����Ƶ
	TIM_TimeBaseStructure.TIM_ClockDivision = 0;     //����ʱ�ӷָ�
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM���ϼ���ģʽ
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure); 
	
	//���ö�ʱ��3Ϊ������ģʽ  IT1 IT2Ϊ�����ؼ���
	TIM_EncoderInterfaceConfig(TIM3, TIM_EncoderMode_TI12,TIM_ICPolarity_BothEdge,TIM_ICPolarity_BothEdge);
	TIM_ICStructInit(&TIM_ICInitStructure);
    TIM_ICInitStructure.TIM_ICFilter = 6;      //�����˲���
    TIM_ICInit(TIM3, &TIM_ICInitStructure);
    TIM_ClearFlag(TIM3, TIM_FLAG_Update);      //������б�־λ
    TIM_ITConfig(TIM3, TIM_IT_Update, ENABLE); //�����жϸ���
    TIM3->CNT = 0;
	TIM_Cmd(TIM3, ENABLE);
}

void TIM4_Configuration(void)//�������ӿ����ã�TIM4��/PB6-A��  PB7-B��
{
    GPIO_InitTypeDef GPIO_InitStructure; 
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef TIM_OCInitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB , ENABLE);  //ʹ��GPIOB����ʱ��
	                                                       				
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7 | GPIO_Pin_8 | GPIO_Pin_9; 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;          //pwm mode
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	   //IO�ڵ�����Ϊ50M 
	GPIO_Init(GPIOB, &GPIO_InitStructure);				   //IO�����ú���
//	GPIO_WriteBit(GPIOB, GPIO_Pin_6,Bit_SET);
//	GPIO_WriteBit(GPIOB, GPIO_Pin_7,Bit_SET); 

   	TIM_TimeBaseStructure.TIM_Period = PWMPeriod; 
	TIM_TimeBaseStructure.TIM_Prescaler = 143; 
	TIM_TimeBaseStructure.TIM_ClockDivision = 0; 
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  
	TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure); 
	
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2;
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_OutputNState = TIM_OutputNState_Disable;
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low;
	TIM_OCInitStructure.TIM_OCNPolarity = TIM_OCNPolarity_High;
	TIM_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Set;
	TIM_OCInitStructure.TIM_OCNIdleState = TIM_OCIdleState_Reset;

	TIM_OC1Init(TIM4, &TIM_OCInitStructure);
	TIM_OC2Init(TIM4, &TIM_OCInitStructure);
	
	TIM4->CCR1 = 0; //PC6�����������1.65V��ѹ  ���0.1V
	TIM4->CCR2 = 0; //PC7�����������1.98V��ѹ  ���0.1V

	TIM_Cmd(TIM4, ENABLE);
	TIM_CtrlPWMOutputs(TIM4, ENABLE);	
}




void TIM7_Configuration(void)			//10ms����һ���ж�
{
	NVIC_InitTypeDef NVIC_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM7, ENABLE);

	TIM_TimeBaseStructure.TIM_Period = 1000-1;
	TIM_TimeBaseStructure.TIM_Prescaler =7200-1;
	TIM_TimeBaseStructure.TIM_ClockDivision = 0;       
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; 
	TIM_TimeBaseStructure.TIM_RepetitionCounter=0;
	TIM_TimeBaseInit(TIM7,&TIM_TimeBaseStructure);

	TIM_UpdateRequestConfig(TIM7,TIM_UpdateSource_Regular);
	TIM_ITConfig(TIM7,TIM_IT_Update,ENABLE);
	TIM_Cmd(TIM7,ENABLE);
	
	NVIC_InitStructure.NVIC_IRQChannel = TIM7_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0 ;//��ռ���ȼ�0
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;		//�����ȼ�2
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQͨ��ʹ��
	NVIC_Init(&NVIC_InitStructure);	//����ָ���Ĳ�����ʼ��VIC�Ĵ���

}
///////////////////////////////

void TIM_Configuration(void)//TIM��ʼ��
{
   
	TIM8_Configuration();
	TIM3_Configuration();
	TIM4_Configuration();
	TIM7_Configuration();
	
}
///////////////////////////////////////////
/******************************************
*�������ƣ�Motor_PWM_ChangeDuty(s32 control)
*����˵����TIM_CH1 TIM_CH2 ΪPWM�����TIM_CH1 �͵�ƽ TIM_CH2 �ߵ�ƽ �����ת��֮��ת
*///////////////////////////////////////////
  
void Motor_PWM_ChangeDuty(s32 control)//�������(TIM8_CH1 TIM8_CH2)
{		
	s32 MotorSpeed,PwmOne_Set,PwmOne_Mode;

	MotorSpeed = control;//��ȡPID������ֵ
											   
	if(MotorSpeed > PWMPeriod)  MotorSpeed =   PWMPeriod-1 ;//���� CCR��ֵ����С�ڻ����ARR��ֵ
	if(MotorSpeed <-PWMPeriod)  MotorSpeed = -(PWMPeriod-1);//���� 

	if(MotorSpeed<0) { PwmOne_Set = -MotorSpeed; PwmOne_Mode = MotorOneBack;}
	else { PwmOne_Set = MotorSpeed; PwmOne_Mode = MotorOneForward;}
	
	if(PwmOne_Mode) {TIM8->CCR1 = MotorSpdZero; TIM8->CCR2 = PwmOne_Set;} //MotorOneForward 
	else { TIM8->CCR1 = PwmOne_Set; TIM8->CCR2 = MotorSpdZero;} //MotorOneBack	
}
///////////////////////////////////////////
 
void Servo_PWM_ChangeDuty(s32 MotorSpeed)//������ƣ�TIM4_CH1 TIM4_CH2��
{
   s32 PwmTwo_Set,PwmTwo_Mode;

	if(MotorSpeed > SERVO_LEFT)  MotorSpeed = SERVO_LEFT;
	if(MotorSpeed < SERVO_RIGHT)  MotorSpeed =  SERVO_RIGHT;
   if(MotorSpeed<0) { PwmTwo_Set = SERVO_MIDDLE; PwmTwo_Mode = MotorTwoForward;}
   else { PwmTwo_Set = MotorSpeed; PwmTwo_Mode = MotorTwoForward;}

   if(PwmTwo_Mode) {TIM4->CCR1 = MotorSpdZero; TIM4->CCR2 = PwmTwo_Set;} //MotorTwoForward
   else { TIM4->CCR1 = PwmTwo_Set; TIM4->CCR2 = MotorSpdZero;}	//MotorTwoBack


}



///////////////////////////////////////////

void UserMotorSpeedSetThr(s32 MotorSpeed)//���3ת��/ת������(TIM8)
{		
	s32 PwmOne_Set,PwmOne_Mode;

	if(MotorSpeed<0) { PwmOne_Set = -MotorSpeed; PwmOne_Mode = MotorOneBack;}
	else { PwmOne_Set = MotorSpeed; PwmOne_Mode = MotorOneForward;}

	if(PwmOne_Mode) {TIM8->CCR1 = MotorSpdZero; TIM8->CCR2 = PwmOne_Set;} //MotorOneForward
	else { TIM8->CCR1 = PwmOne_Set; TIM8->CCR2 = MotorSpdZero;} //MotorOneBack	
}
////////////////////////////////////////////

void UserMotorSpeedSetFou(s32 MotorSpeed)//���4ת��/ת������(TIM8)
{
   s32 PwmTwo_Set,PwmTwo_Mode;

   if(MotorSpeed<0) { PwmTwo_Set = -MotorSpeed; PwmTwo_Mode = MotorTwoBack;}
   else { PwmTwo_Set = MotorSpeed; PwmTwo_Mode = MotorTwoForward;}

   if(PwmTwo_Mode) {TIM8->CCR3 = MotorSpdZero; TIM8->CCR4 = PwmTwo_Set;} //MotorTwoForward
   else { TIM8->CCR3 = PwmTwo_Set; TIM8->CCR4 = MotorSpdZero;}	//MotorTwoBack
}

void car_start_test(void)
{
	Motor_PWM_ChangeDuty(3000);
	Servo_PWM_ChangeDuty(515);
	delay_ms(1000);
	Motor_PWM_ChangeDuty(0);
	Servo_PWM_ChangeDuty(455);
	delay_ms(1000);
	Motor_PWM_ChangeDuty(-3000);
	Servo_PWM_ChangeDuty(395);
	delay_ms(1000);
	Motor_PWM_ChangeDuty(0);
	Servo_PWM_ChangeDuty(455);
	delay_ms(1000);
	car_func_mode=0;
}

void close_change_speed(void)
{
	int n=0,cnt=0;
	SPEED_DESIRE=0;
	while(cnt<=40)
	{
		if(n==1)
		{
			SPEED_DESIRE-=10;
			Motor_PID_setSpeed(SPEED_DESIRE);
			cnt++;
		}
		if(n==0)
		{
			SPEED_DESIRE+=10;
			Motor_PID_setSpeed(SPEED_DESIRE);
			cnt++;
		}
		if(SPEED_DESIRE==100)
			n=1;
		if(SPEED_DESIRE==-100)
			n=0;
		delay_ms(500);
	}
}
	
void Motor_PID_setSpeed(s32 speed_desire)
{
	get_encoder_TWO();
	User_PidSpeedControl(speed_desire);
}

void car_stop(void)
{
	Motor_PWM_ChangeDuty(0);
	Servo_PWM_ChangeDuty(455);
}

/*********************************************
 * ��ǳ���
 * direction 0�� 1�� 2��
*********************************************/
void Servo_ChangDuty(u16 del, u8 direction)
{
	int dist = 0;
	static int16_t dist_last=0;
	del=del*60/150;
	if(direction==MIDDLE)
	{
		Servo_PWM_ChangeDuty(SERVO_MIDDLE);
	}
	else
	{
		if(direction==RIGHT)//��ת
			dist=-del;
		else if (direction==LEFT) 
			dist=del;
		
		ServoDuty=SERVO_MIDDLE + 0.01*(servo_Kp*dist + servo_Kd*(dist - dist_last));

		dist_last = dist;
		
		Servo_PWM_ChangeDuty(ServoDuty);
	}
}





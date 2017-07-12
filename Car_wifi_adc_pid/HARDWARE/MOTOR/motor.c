#include "stm32f10x.h"
#include "motor.h"
#include "delay.h"
#include "function.h"

u16 ServoDuty;

static float servo_Kp = 70, servo_Kd = 5;      //前向舵机Kp 和Kd	

void TIM8_Configuration(void)//TIM8四通道分别输出PWM1/PWM2,对应引脚为PC6/PC7，ch1 ch2
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
	
	TIM8->CCR1 = 0; //PC6上理论能输出1.65V电压  误差0.1V
	TIM8->CCR2 = 0; //PC7上理论能输出1.98V电压  误差0.1V

	TIM_Cmd(TIM8, ENABLE);
	TIM_CtrlPWMOutputs(TIM8, ENABLE);	

}

/////////////////////////////

void TIM3_Configuration(void)//编码器接口设置（TIM3）/PA6-A相  PA7-B相
{
    GPIO_InitTypeDef GPIO_InitStructure; 
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_ICInitTypeDef  TIM_ICInitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA , ENABLE);  //使能GPIOA外设时钟
	                                                       				
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6|GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;          //设置为上拉输入模式 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	   //IO口的速率为50M 
	GPIO_Init(GPIOA, &GPIO_InitStructure);				   //IO口配置函数
	GPIO_WriteBit(GPIOA, GPIO_Pin_6,Bit_SET);
	GPIO_WriteBit(GPIOA, GPIO_Pin_7,Bit_SET); 

   	TIM_TimeBaseStructure.TIM_Period = PWMPeriod; //设置在下一个更新事件装入活动的自动重装载寄存器周期的值
	TIM_TimeBaseStructure.TIM_Prescaler = 0; //设置用来作为TIMx时钟频率除数的预分频值  不分频
	TIM_TimeBaseStructure.TIM_ClockDivision = 0;     //设置时钟分割
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM向上计数模式
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure); 
	
	//设置定时器3为编码器模式  IT1 IT2为上升沿计数
	TIM_EncoderInterfaceConfig(TIM3, TIM_EncoderMode_TI12,TIM_ICPolarity_BothEdge,TIM_ICPolarity_BothEdge);
	TIM_ICStructInit(&TIM_ICInitStructure);
    TIM_ICInitStructure.TIM_ICFilter = 6;      //输入滤波器
    TIM_ICInit(TIM3, &TIM_ICInitStructure);
    TIM_ClearFlag(TIM3, TIM_FLAG_Update);      //清除所有标志位
    TIM_ITConfig(TIM3, TIM_IT_Update, ENABLE); //允许中断更新
    TIM3->CNT = 0;
	TIM_Cmd(TIM3, ENABLE);
}

void TIM4_Configuration(void)//编码器接口设置（TIM4）/PB6-A相  PB7-B相
{
    GPIO_InitTypeDef GPIO_InitStructure; 
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef TIM_OCInitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB , ENABLE);  //使能GPIOB外设时钟
	                                                       				
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7 | GPIO_Pin_8 | GPIO_Pin_9; 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;          //pwm mode
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	   //IO口的速率为50M 
	GPIO_Init(GPIOB, &GPIO_InitStructure);				   //IO口配置函数
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
	
	TIM4->CCR1 = 0; //PC6上理论能输出1.65V电压  误差0.1V
	TIM4->CCR2 = 0; //PC7上理论能输出1.98V电压  误差0.1V

	TIM_Cmd(TIM4, ENABLE);
	TIM_CtrlPWMOutputs(TIM4, ENABLE);	
}




void TIM7_Configuration(void)			//10ms产生一次中断
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
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0 ;//抢占优先级0
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;		//子优先级2
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);	//根据指定的参数初始化VIC寄存器

}
///////////////////////////////

void TIM_Configuration(void)//TIM初始化
{
   
	TIM8_Configuration();
	TIM3_Configuration();
	TIM4_Configuration();
	TIM7_Configuration();
	
}
///////////////////////////////////////////
/******************************************
*函数名称：Motor_PWM_ChangeDuty(s32 control)
*函数说明：TIM_CH1 TIM_CH2 为PWM输出，TIM_CH1 低电平 TIM_CH2 高电平 电机正转反之反转
*///////////////////////////////////////////
  
void Motor_PWM_ChangeDuty(s32 control)//电机控制(TIM8_CH1 TIM8_CH2)
{		
	s32 MotorSpeed,PwmOne_Set,PwmOne_Mode;

	MotorSpeed = control;//读取PID的输入值
											   
	if(MotorSpeed > PWMPeriod)  MotorSpeed =   PWMPeriod-1 ;//上限 CCR的值必须小于或等于ARR的值
	if(MotorSpeed <-PWMPeriod)  MotorSpeed = -(PWMPeriod-1);//下限 

	if(MotorSpeed<0) { PwmOne_Set = -MotorSpeed; PwmOne_Mode = MotorOneBack;}
	else { PwmOne_Set = MotorSpeed; PwmOne_Mode = MotorOneForward;}
	
	if(PwmOne_Mode) {TIM8->CCR1 = MotorSpdZero; TIM8->CCR2 = PwmOne_Set;} //MotorOneForward 
	else { TIM8->CCR1 = PwmOne_Set; TIM8->CCR2 = MotorSpdZero;} //MotorOneBack	
}
///////////////////////////////////////////
 
void Servo_PWM_ChangeDuty(s32 MotorSpeed)//舵机控制（TIM4_CH1 TIM4_CH2）
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

void UserMotorSpeedSetThr(s32 MotorSpeed)//电机3转速/转向设置(TIM8)
{		
	s32 PwmOne_Set,PwmOne_Mode;

	if(MotorSpeed<0) { PwmOne_Set = -MotorSpeed; PwmOne_Mode = MotorOneBack;}
	else { PwmOne_Set = MotorSpeed; PwmOne_Mode = MotorOneForward;}

	if(PwmOne_Mode) {TIM8->CCR1 = MotorSpdZero; TIM8->CCR2 = PwmOne_Set;} //MotorOneForward
	else { TIM8->CCR1 = PwmOne_Set; TIM8->CCR2 = MotorSpdZero;} //MotorOneBack	
}
////////////////////////////////////////////

void UserMotorSpeedSetFou(s32 MotorSpeed)//电机4转速/转向设置(TIM8)
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
 * 打角程序
 * direction 0中 1左 2右
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
		if(direction==RIGHT)//右转
			dist=-del;
		else if (direction==LEFT) 
			dist=del;
		
		ServoDuty=SERVO_MIDDLE + 0.01*(servo_Kp*dist + servo_Kd*(dist - dist_last));

		dist_last = dist;
		
		Servo_PWM_ChangeDuty(ServoDuty);
	}
}





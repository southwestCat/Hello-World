#include "sys.h"
#include "delay.h"
#include "usart.h" 
#include "led.h" 		 	 
#include "lcd.h"  
#include "key.h"     
#include "malloc.h"	
#include "usart3.h"
#include "common.h" 
#include "exti.h"
#include "motor.h"
#include "function.h"
#include "adc.h"

 
/************************************************
 实验0：ALIENTEK STM32F103开发板工程模板
 注意，这是手册中的调试章节使用的main文件
 技术支持：www.openedv.com
 淘宝店铺：http://eboard.taobao.com 
 关注微信公众平台微信号："正点原子"，免费获取STM32资料。
 广州市星翼电子科技有限公司  
 作者：正点原子 @ALIENTEK
************************************************/

u8 rlen;

int car_func_mode,car_func_data;

void git_test(void);

void Periph_Init()
{
	delay_init();
	
	uart_init(115200);	 	//串口初始化为115200
	
	EXTIX_Init();				//初始化按键中断
	
	TIM_Configuration();
	
	LED_Init();
	
	GPIO_Config();
	
//	MYDMA_Config(DMA1_Channel1,(u32)&ADC1->DR,(u32)AD_Value,N*M);//DMA1通道4,外设为串口1,存储器为SendBuff,长度SEND_BUF_SIZE.
	
	Adc_Init();
	
	LCD_Init();			   	//初始化LCD 	
	
//	KEY_Init();				//按键初始化		
	usart3_init(115200);		//初始化串口3 
	
 	my_mem_init(SRAMIN);		//初始化内部内存池 
	
	atk_8266_wifiap_connect();	

}

int main(void)
{
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//设置中断优先级分组为组2：2位抢占优先级，2位响应优先级
	
	Periph_Init();
		
	while(1)
	{
		if(USART3_RX_STA&0X8000)
		{
			rlen=USART3_RX_STA&0X7FFF;
			USART3_RX_BUF[rlen]=0;
			if  ((USART3_RX_BUF[11]=='F')&&(USART3_RX_BUF[12]=='F'))
			{
				car_func_mode=10*(USART3_RX_BUF[13]-'0')+USART3_RX_BUF[14]-'0';
				car_func_data=1000*(USART3_RX_BUF[15]-'0')+100*(USART3_RX_BUF[16]-'0')+10*(USART3_RX_BUF[17]-'0')+(USART3_RX_BUF[18]-'0');
			}
			USART3_RX_STA=0;
		}
/**********************
 * car_func_mode
 * 0：停车指令；
 * 1：开车测试；
 * 2：闭环测试
 * 3：前进
 * 4：后退
 * 5：左转
 * 6：右转
 * 7：自动巡航
*********************/
		switch(car_func_mode)
		{
			case 0:
			{
				u8 *p;
				p=mymalloc(SRAMIN,32);
				sprintf((char*)p,"mode:%d, data:%d",car_func_mode,car_func_data);//测试数据
				LCD_ShowString(30,140,200,16,16,"receive data : ");	
				LCD_ShowString(30,160,200,16,16,p);	
				myfree(SRAMIN,p);
				
				car_stop();
				
				break;
			}
			case 1:
			{
				u8 *p;
				p=mymalloc(SRAMIN,32);
				sprintf((char*)p,"mode:%d, data:%d",car_func_mode,car_func_data);//测试数据
				LCD_Clear(WHITE);
				LCD_ShowString(30,140,200,16,16,"receive data : ");	
				LCD_ShowString(30,160,200,16,16,p);	
				myfree(SRAMIN,p);
				
				while(car_func_mode)
				{
					car_start_test();
				}
				car_func_mode=0;
				break;
			}
			case 2:
			{
				u8 *p;
				p=mymalloc(SRAMIN,32);
				sprintf((char*)p,"mode:%d, data:%d",car_func_mode,car_func_data);//测试数据
				LCD_Clear(WHITE);
				LCD_ShowString(30,140,200,16,16,"receive data : ");	
				LCD_ShowString(30,160,200,16,16,p);	
				myfree(SRAMIN,p);
				
				close_change_speed();
				car_func_mode=0;
				break;
			}
			case 3:
			{
				u8 *p;
				p=mymalloc(SRAMIN,32);
				sprintf((char*)p,"mode:%d, data:%d",car_func_mode,car_func_data);//测试数据
				LCD_Clear(WHITE);
				LCD_ShowString(30,140,200,16,16,"receive data : ");	
				LCD_ShowString(30,160,200,16,16,p);	
				myfree(SRAMIN,p);
				
				SPEED_DESIRE=30;
				Motor_PID_setSpeed(SPEED_DESIRE);
				Motor_PWM_ChangeDuty(1500);
				break;
			}
			case 4:
			{
				u8 *p;
				p=mymalloc(SRAMIN,32);
				sprintf((char*)p,"mode:%d, data:%d",car_func_mode,car_func_data);//测试数据
				LCD_Clear(WHITE);
				LCD_ShowString(30,140,200,16,16,"receive data : ");	
				LCD_ShowString(30,160,200,16,16,p);	
				myfree(SRAMIN,p);
				
				SPEED_DESIRE=-50;
				Motor_PID_setSpeed(SPEED_DESIRE);
				Motor_PWM_ChangeDuty(-1500);
				break;
			}
			case 5:
			{
				u8 *p;
				p=mymalloc(SRAMIN,32);
				sprintf((char*)p,"mode:%d, data:%d",car_func_mode,car_func_data);//测试数据
				LCD_Clear(WHITE);
				LCD_ShowString(30,140,200,16,16,"receive data : ");	
				LCD_ShowString(30,160,200,16,16,p);	
				myfree(SRAMIN,p);
//				
				Servo_ChangDuty(200,LEFT);
				break;
			}
			case 6:
			{
				u8 *p;
				p=mymalloc(SRAMIN,32);
				sprintf((char*)p,"mode:%d, data:%d",car_func_mode,car_func_data);//测试数据
				LCD_Clear(WHITE);
				LCD_ShowString(30,140,200,16,16,"receive data : ");	
				LCD_ShowString(30,160,200,16,16,p);	
				myfree(SRAMIN,p);
//				
				Servo_PWM_ChangeDuty(395);
				break;
			}
			case 7:	// to find the sound
			{
				LCD_Clear(WHITE);
				LCD_ShowString(30, 100, 200, 24, 24, "Careful drive !");
				Auto_drive();
				break;
			}
			case 8:
			{
				Draw_wave();
				LCD_Clear(WHITE);
				break;
			}
			case 9:
			{
				git_test();
			}
			default: { car_func_mode=0; car_func_data=0; break; }
		}
	}
} 

void git_test()
{
	printf("this is work branch!");
}



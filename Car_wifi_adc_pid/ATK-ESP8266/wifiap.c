#include "common.h"
/////////////////////////////////////////////////////////////////////////////////////////////////////////// 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEK STM32������
//ATK-ESP8266 WIFIģ�� WIFI AP��������	   
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//�޸�����:2014/4/3
//�汾��V1.0
//��Ȩ���У�����ؾ���
//Copyright(C) ������������ӿƼ����޹�˾ 2009-2019
//All rights reserved									  
/////////////////////////////////////////////////////////////////////////////////////////////////////////// 


//ATK-ESP8266 WIFI AP����
//���ڲ���TCP/UDP����
//����ֵ:0,����
//    ����,�������
u8 atk_8266_wifiap_connect(void)
{
	u8 netpro=0;	//����ģʽ
//	u8 key;
//	u8 timex=0; 
	u8 ipbuf[16]; 	//IP����
	u8 *p;
//	u16 t=999;		//���ٵ�һ�λ�ȡ����״̬
//	u16 rlen=0;
	u8 constate=0;	//����״̬
	p=mymalloc(SRAMIN,32);							//����32�ֽ��ڴ�

	LCD_Clear(WHITE);
	POINT_COLOR=RED;
	LCD_ShowString(0,30,200,16,16,"ATK-ESP WIFI-AP test"); 
	LCD_ShowString(0,50,200,16,16,"ATK-ESP configuring, wait...");
	while(atk_8266_send_cmd("AT","OK",20))//���WIFIģ���Ƿ�����
	{
		atk_8266_quit_trans();//�˳�͸��
		atk_8266_send_cmd("AT+CIPMODE=0","OK",200);  //�ر�͸��ģʽ	
		LCD_ShowString(0,30,200,16,16,"error");
		delay_ms(800);
		LCD_Fill(40,55,200,55+16,WHITE);
		LCD_ShowString(0,30,200,16,16,"again");
	} 
	atk_8266_send_cmd("AT+CIPMUX=1","OK",20);   //0�������ӣ�1��������
	sprintf((char*)p,"AT+CIPSERVER=1,%s",(u8*)portnum);
	atk_8266_send_cmd(p,"OK",20);     //����Serverģʽ���˿ں�Ϊ8086

	LCD_Clear(WHITE);
	POINT_COLOR=RED;
	LCD_ShowString(0,30,200,16,16,"ATK-ESP WIFI-AP test");
	LCD_ShowString(30,50,200,16,16,"ATK-ESP configuring,wait...");			
	//LCD_Fill(30,50,239,50+12,WHITE);			//���֮ǰ����ʾ
	LCD_Clear(WHITE);
	LCD_ShowString(30,50,200,16,16,"ATK-ESP Success");
	
	delay_ms(200);

	atk_8266_get_wanip(ipbuf);//������ģʽ,��ȡWAN IP
	sprintf((char*)p,"IP:%s port:%s",ipbuf,(u8*)portnum);
	LCD_ShowString(30,65,200,16,16,p);				//��ʾIP��ַ�Ͷ˿�	
	LCD_ShowString(30,80,200,16,16,"state:"); 		//����״̬
	LCD_ShowString(120,80,200,16,16,"mode:"); 		//����״̬
	LCD_ShowString(30,100,200,16,16,"senddata:"); 	//��������
	LCD_ShowString(30,115,200,16,16,"recedata:");	//��������
	atk_8266_wificonf_show(30,180,"connect to WIFI:",(u8*)wifiap_ssid,(u8*)wifiap_encryption,(u8*)wifiap_password);
	POINT_COLOR=BLUE;
	LCD_ShowString(120+30,80,200,16,16,(u8*)ATK_ESP8266_WORKMODE_TBL[netpro]); 		//����״̬
	USART3_RX_STA=0;
	
//	if(key==KEY0_PRES)	//KEY0 �������� �����жϣ�
//	{
//		sprintf((char*)p,"ATK-8266%stest%02d\r\n",ATK_ESP8266_WORKMODE_TBL[netpro],t/10);//��������
//		LCD_ShowString(30+54,100,200,16,16,p);
//		atk_8266_send_cmd("AT+CIPSEND=0,25","OK",200);  //����ָ�����ȵ�����
//		delay_ms(200);
//		atk_8266_send_data(p,"OK",100);  //����ָ�����ȵ�����
//		timex=100;
//	}
		
//	if(timex)timex--;
//	if(timex==1)LCD_Fill(30+54,100,239,112,WHITE);
//	t++;
//	delay_ms(5);
//	if(USART3_RX_STA&0X8000)		//���յ�һ��������
//	{ 
//		rlen=USART3_RX_STA&0X7FFF;	//�õ����ν��յ������ݳ���
//		USART3_RX_BUF[rlen]=0;		//��ӽ����� 
//		printf("%s",USART3_RX_BUF);	//���͵�����   
//		sprintf((char*)p,"receive%dbyte,as follow",rlen);//���յ����ֽ��� 
//		LCD_Fill(30+54,115,239,130,WHITE);
//		POINT_COLOR=BRED;
//		LCD_ShowString(30+54,115,200,16,16,p); 			//��ʾ���յ������ݳ���
//		POINT_COLOR=BLUE;
//		LCD_Fill(30,130,239,319,WHITE);
//		LCD_ShowString(30,130,200,16,16,USART3_RX_BUF);//��ʾ���յ�������  
//		USART3_RX_STA=0;
//		if(constate!='+')t=1000;		//״̬Ϊ��δ����,������������״̬
//		else t=0;                   //״̬Ϊ�Ѿ�������,10����ټ��
//	}   
//	if(t==1000)//����10����û���յ��κ�����,��������ǲ��ǻ�����.
//	{
//		constate=atk_8266_consta_check();//�õ�����״̬
//		if(constate=='+')LCD_ShowString(30+30,80,200,16,16,"Connected");  //����״̬
//		else LCD_ShowString(30+30,80,200,16,16,"connect failed"); 	  	 
//		t=0;
//	}

	constate=atk_8266_consta_check();//�õ�����״̬
	if(constate=='+')LCD_ShowString(30+30,80,200,16,16,"Connected");  //����״̬
	else LCD_ShowString(30+30,80,200,16,16,"connect failed"); 	  	 
	
	atk_8266_at_response(1);

	myfree(SRAMIN,p);		//�ͷ��ڴ� 
	return 0;		
} 

void quit_connect(void)
{
	atk_8266_quit_trans();	//�˳�͸��
	atk_8266_send_cmd("AT+CIPMODE=0","OK",20);   //�ر�͸��ģʽ
}

void tcp_send_test(void)
{
	u8 *p;
	p=mymalloc(SRAMIN,32);
	atk_8266_send_cmd("AT+CIPSEND=0,25","OK",20);  //����ָ�����ȵ�����
	sprintf((char*)p,"ATK-8266%s����%02d\r\n","tcp server",100);//��������
	LCD_ShowString(30+54,100,200,16,16,p);
	atk_8266_send_data(p,"OK",100);  //����ָ�����ȵ�����
	myfree(SRAMIN,p);
}









#include "common.h"
/////////////////////////////////////////////////////////////////////////////////////////////////////////// 
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//ALIENTEK STM32开发板
//ATK-ESP8266 WIFI模块 WIFI AP驱动代码	   
//正点原子@ALIENTEK
//技术论坛:www.openedv.com
//修改日期:2014/4/3
//版本：V1.0
//版权所有，盗版必究。
//Copyright(C) 广州市星翼电子科技有限公司 2009-2019
//All rights reserved									  
/////////////////////////////////////////////////////////////////////////////////////////////////////////// 


//ATK-ESP8266 WIFI AP测试
//用于测试TCP/UDP连接
//返回值:0,正常
//    其他,错误代码
u8 atk_8266_wifiap_connect(void)
{
	u8 netpro=0;	//网络模式
//	u8 key;
//	u8 timex=0; 
	u8 ipbuf[16]; 	//IP缓存
	u8 *p;
//	u16 t=999;		//加速第一次获取链接状态
//	u16 rlen=0;
	u8 constate=0;	//连接状态
	p=mymalloc(SRAMIN,32);							//申请32字节内存

	LCD_Clear(WHITE);
	POINT_COLOR=RED;
	LCD_ShowString(0,30,200,16,16,"ATK-ESP WIFI-AP test"); 
	LCD_ShowString(0,50,200,16,16,"ATK-ESP configuring, wait...");
	while(atk_8266_send_cmd("AT","OK",20))//检查WIFI模块是否在线
	{
		atk_8266_quit_trans();//退出透传
		atk_8266_send_cmd("AT+CIPMODE=0","OK",200);  //关闭透传模式	
		LCD_ShowString(0,30,200,16,16,"error");
		delay_ms(800);
		LCD_Fill(40,55,200,55+16,WHITE);
		LCD_ShowString(0,30,200,16,16,"again");
	} 
	atk_8266_send_cmd("AT+CIPMUX=1","OK",20);   //0：单连接，1：多连接
	sprintf((char*)p,"AT+CIPSERVER=1,%s",(u8*)portnum);
	atk_8266_send_cmd(p,"OK",20);     //开启Server模式，端口号为8086

	LCD_Clear(WHITE);
	POINT_COLOR=RED;
	LCD_ShowString(0,30,200,16,16,"ATK-ESP WIFI-AP test");
	LCD_ShowString(30,50,200,16,16,"ATK-ESP configuring,wait...");			
	//LCD_Fill(30,50,239,50+12,WHITE);			//清除之前的显示
	LCD_Clear(WHITE);
	LCD_ShowString(30,50,200,16,16,"ATK-ESP Success");
	
	delay_ms(200);

	atk_8266_get_wanip(ipbuf);//服务器模式,获取WAN IP
	sprintf((char*)p,"IP:%s port:%s",ipbuf,(u8*)portnum);
	LCD_ShowString(30,65,200,16,16,p);				//显示IP地址和端口	
	LCD_ShowString(30,80,200,16,16,"state:"); 		//连接状态
	LCD_ShowString(120,80,200,16,16,"mode:"); 		//连接状态
	LCD_ShowString(30,100,200,16,16,"senddata:"); 	//发送数据
	LCD_ShowString(30,115,200,16,16,"recedata:");	//接收数据
	atk_8266_wificonf_show(30,180,"connect to WIFI:",(u8*)wifiap_ssid,(u8*)wifiap_encryption,(u8*)wifiap_password);
	POINT_COLOR=BLUE;
	LCD_ShowString(120+30,80,200,16,16,(u8*)ATK_ESP8266_WORKMODE_TBL[netpro]); 		//连接状态
	USART3_RX_STA=0;
	
//	if(key==KEY0_PRES)	//KEY0 发送数据 改用中断；
//	{
//		sprintf((char*)p,"ATK-8266%stest%02d\r\n",ATK_ESP8266_WORKMODE_TBL[netpro],t/10);//测试数据
//		LCD_ShowString(30+54,100,200,16,16,p);
//		atk_8266_send_cmd("AT+CIPSEND=0,25","OK",200);  //发送指定长度的数据
//		delay_ms(200);
//		atk_8266_send_data(p,"OK",100);  //发送指定长度的数据
//		timex=100;
//	}
		
//	if(timex)timex--;
//	if(timex==1)LCD_Fill(30+54,100,239,112,WHITE);
//	t++;
//	delay_ms(5);
//	if(USART3_RX_STA&0X8000)		//接收到一次数据了
//	{ 
//		rlen=USART3_RX_STA&0X7FFF;	//得到本次接收到的数据长度
//		USART3_RX_BUF[rlen]=0;		//添加结束符 
//		printf("%s",USART3_RX_BUF);	//发送到串口   
//		sprintf((char*)p,"receive%dbyte,as follow",rlen);//接收到的字节数 
//		LCD_Fill(30+54,115,239,130,WHITE);
//		POINT_COLOR=BRED;
//		LCD_ShowString(30+54,115,200,16,16,p); 			//显示接收到的数据长度
//		POINT_COLOR=BLUE;
//		LCD_Fill(30,130,239,319,WHITE);
//		LCD_ShowString(30,130,200,16,16,USART3_RX_BUF);//显示接收到的数据  
//		USART3_RX_STA=0;
//		if(constate!='+')t=1000;		//状态为还未连接,立即更新连接状态
//		else t=0;                   //状态为已经连接了,10秒后再检查
//	}   
//	if(t==1000)//连续10秒钟没有收到任何数据,检查连接是不是还存在.
//	{
//		constate=atk_8266_consta_check();//得到连接状态
//		if(constate=='+')LCD_ShowString(30+30,80,200,16,16,"Connected");  //连接状态
//		else LCD_ShowString(30+30,80,200,16,16,"connect failed"); 	  	 
//		t=0;
//	}

	constate=atk_8266_consta_check();//得到连接状态
	if(constate=='+')LCD_ShowString(30+30,80,200,16,16,"Connected");  //连接状态
	else LCD_ShowString(30+30,80,200,16,16,"connect failed"); 	  	 
	
	atk_8266_at_response(1);

	myfree(SRAMIN,p);		//释放内存 
	return 0;		
} 

void quit_connect(void)
{
	atk_8266_quit_trans();	//退出透传
	atk_8266_send_cmd("AT+CIPMODE=0","OK",20);   //关闭透传模式
}

void tcp_send_test(void)
{
	u8 *p;
	p=mymalloc(SRAMIN,32);
	atk_8266_send_cmd("AT+CIPSEND=0,25","OK",20);  //发送指定长度的数据
	sprintf((char*)p,"ATK-8266%s测试%02d\r\n","tcp server",100);//测试数据
	LCD_ShowString(30+54,100,200,16,16,p);
	atk_8266_send_data(p,"OK",100);  //发送指定长度的数据
	myfree(SRAMIN,p);
}









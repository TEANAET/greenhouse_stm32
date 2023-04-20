/*-------------------------------------------------*/
/*                                                 */
/*            	   中断服务函数          	   	   */
/*                                                 */
/*-------------------------------------------------*/

#include "stm32f10x.h"     //包含需要的头文件
#include "stm32f10x_it.h"  //包含需要的头文件
#include "usart1.h"        //包含需要的头文件
#include "usart2.h"        //包含需要的头文件
#include "usart3.h"        //包含需要的头文件
#include "timer3.h"        //包含需要的头文件
#include "mqtt.h"          //包含需要的头文件
#include "dht11.h"         //包含需要的头文件                  
#include "ds18b20.h"
#include "delay.h"	
#include "control.h"
#include "oled.h"
#include "Beep.h"
#include "Relay.h"
#include "stmflash.h"
#include "led.h"
#include "step_motor.h"
#include "lsens.h"
#include  "json.h"
#define FLASH_SAVE_ADDR  0X08070000
//外部变量
//extern u8 tem_fa,soil_fa,hum_fa,light_fa;
u8 flush_flag = 0;
u8 page_flag = 0;
u8 step_flag,step_on_flag = 0;
extern u8 tem_fa_min,tem_fa_max,soil_fa_min,soil_fa_max,hum_fa_min,hum_fa_max,light_fa_min,light_fa_max;

extern u8 Flash_fa;
extern u8 Soil_val,Light_val,Hum_val;

extern s8 Tem_val;

extern u8 Auto_ctro,Syn_flag;
int i;
u8 Maxbuf[33];

extern char *ledFlag,*beepFlag,*led2Flag,*relayFlag,*relay2Flag,*fansFlag;

/*-------------------------------------------------*/
/*函数名：串口2接收中断函数（最高优先级，处理接收数据）*/
/*参  数：无                                       */
/*返回值：无                                       */
/*-------------------------------------------------*/
void USART2_IRQHandler(void)   
{                      
	if(USART_GetITStatus(USART2, USART_IT_RXNE) != RESET)    //如果USART_IT_RXNE标志置位，表示有数据到了，进入if分支
	{  
		if(connectFlag == 0) 							     //如果connectFlag等于0，当前还没有连接服务器，处于指令配置状态
		{                                     
			if(USART2->DR)
			{                                     			 //处于指令配置状态时，非零值才保存到缓冲区	
				Usart2_RxBuff[Usart2_RxCounter] = USART2->DR;//保存到缓冲区	
				Usart2_RxCounter++; 						 //每接收1个字节的数据，Usart2_RxCounter加1，表示接收的数据总量+1 
			}					
		}
		else
		{		                                           	 //反之connectFlag等于1，连接上服务器了	
			Usart2_RxBuff[Usart2_RxCounter] = USART2->DR;    //把接收到的数据保存到Usart2_RxBuff中				
			if(Usart2_RxCounter == 0)
			{    									    											 //如果Usart2_RxCounter等于0，表示是接收的第1个数据，进入if分支				
				TIM_Cmd(TIM4, ENABLE); 
			}
			else																						 //else分支，表示果Usart2_RxCounter不等于0，不是接收的第一个数据
			{                        									    
				TIM_SetCounter(TIM4, 0);               //TIM4计数器清零
			}	
			Usart2_RxCounter++;         				     //每接收1个字节的数据，Usart2_RxCounter加1，表示接收的数据总量+1 
		}
	}
} 

 /*-------------------------------------------------*/
/*函数名：定时器6中断服务函数。OLED显示          */
/*参  数：无                                       */
/*返回值：无                                       */
/*-------------------------------------------------*/
void TIM6_IRQHandler(void)
{
	if(TIM_GetITStatus(TIM6, TIM_IT_Update) != RESET)//如果TIM_IT_Update置位，表示TIM6溢出中断，进入if	
	{
		
		switch(page_flag%4)
			{
			case 0:
			{
			page_flag++;
			if(flush_flag == 0){//第一次显示
			OLED_Clear();
			flush_flag++;
			oled_dispaly();
			OLED_ShowNum(35,0,Tem_val,2,16);
			OLED_ShowNum(100,0,Hum_val,2,16);
			OLED_ShowNum(55,2,Soil_val,2,16);
			OLED_ShowNum(50,4,Light_val,2,16);
			if(Auto_ctro ==1)
			{
				OLED_Clear_Part(45,80,6,6);
				OLED_ShowString(45,6,"ON",16);
			}else
			{
				OLED_Clear_Part(45,80,6,6);
				OLED_ShowString(45,6,"OFF",16);
			}
		}
		else
		{
		oled_dispaly();
		for(i=0;i<3;i++)
		{
			
			OLED_ShowNum(35,0,Tem_val,2,16);
			OLED_ShowNum(100,0,Hum_val,2,16);
			OLED_ShowNum(55,2,Soil_val,2,16);
			OLED_ShowNum(50,4,Light_val,2,16);
			if(Auto_ctro ==1)
			{
				OLED_Clear_Part(45,80,6,6);
				OLED_ShowString(45,6,"ON",16);
			}
				if(Auto_ctro ==0)
			{
				OLED_Clear_Part(45,80,6,6);
				OLED_ShowString(45,6,"OFF",16);
			}
			DelayMs(2500);
		}
		
		}
		i=0;
		flush_flag=0;//刷屏标志位复位
			}
		break;
			case 1:
			{
				page_flag++;
			if(flush_flag == 0){//第一次显示
				OLED_Clear();
//				OLED_ShowString(45,6,"HAHA",16);
				OLED_ShowString(0,0,"LED:",16);
				OLED_ShowString(0,2,"RELAY:",16);
				OLED_ShowString(0,4,"RELAY2:",16);
				OLED_ShowString(0,6,"BEEP:",16);
				if(strcmp(ledFlag,"LEDON") == 0)
				{
					OLED_ShowString(35,0,"ON",16);
				}else
				{
					OLED_ShowString(35,0,"OFF",16);
				}
				if(strcmp(relayFlag,"RELAYON") == 0)
				{
					OLED_ShowString(55,2,"ON",16);
				}else
				{
					OLED_ShowString(55,2,"OFF",16);
				}
				if(strcmp(relay2Flag,"RELAY2ON") == 0)
				{
					OLED_ShowString(55,4,"ON",16);
				}else
				{
					OLED_ShowString(55,4,"OFF",16);
				}
				if(strcmp(beepFlag,"BEEPON") == 0)
				{
					OLED_ShowString(50,6,"ON",16);
				}else
				{
					OLED_ShowString(50,6,"OFF",16);
				}
			}else
			{
				OLED_ShowString(0,0,"LED:",16);
				OLED_ShowString(0,2,"RELAY:",16);
				OLED_ShowString(0,4,"RELAY2:",16);
				OLED_ShowString(0,6,"BEEP:",16);
				for(i=0;i<3;i++)
				{
					if(strcmp(ledFlag,"LEDON") == 0)
					{
						OLED_ShowString(35,0,"ON",16);
					}else
					{
						OLED_ShowString(35,0,"OFF",16);
					}
					if(strcmp(relayFlag,"RELAYON") == 0)
					{
						OLED_ShowString(55,2,"ON",16);
					}else
					{
						OLED_ShowString(55,2,"OFF",16);
					}
					if(strcmp(relay2Flag,"RELAY2ON") == 0)
					{
						OLED_ShowString(55,4,"ON",16);
					}else
					{
						OLED_ShowString(55,4,"OFF",16);
					}
					if(strcmp(beepFlag,"BEEPON") == 0)
					{
						OLED_ShowString(50,6,"ON",16);
					}else
					{
						OLED_ShowString(50,6,"OFF",16);
					}
					DelayMs(2500);
				}			
		}
		 i=0;
		 flush_flag=0;//刷屏标志位复位
	}break;
			case 2:{
				page_flag++;
			if(flush_flag == 0){//第一次显示
				OLED_Clear();
				OLED_ShowString(0,0,"LED2:",16);
				OLED_ShowString(0,2,"FAN:",16);
//				OLED_ShowString(0,4,"RELAY2:",16);
//				OLED_ShowString(0,6,"BEEP:",16);
				if(strcmp(led2Flag,"LED2ON") == 0)
				{
					OLED_ShowString(45,0,"ON",16);
				}else
				{
					OLED_ShowString(45,0,"OFF",16);
				}
				if(strcmp(fansFlag,"FANSON") == 0)
				{
					OLED_ShowString(37,2,"ON",16);
				}else
				{
					OLED_ShowString(37,2,"OFF",16);
				}
			}else{
				OLED_ShowString(0,0,"LED2:",16);
				OLED_ShowString(0,2,"FAN:",16);
				for(i=0;i<3;i++)
				{
					if(strcmp(led2Flag,"LED2ON") == 0)
					{
						OLED_ShowString(45,0,"ON",16);
					}else
					{
						OLED_ShowString(45,0,"OFF",16);
					}
					if(strcmp(fansFlag,"FANSON") == 0)
					{
					OLED_ShowString(37,2,"ON",16);
					}else
					{
					OLED_ShowString(37,2,"OFF",16);
					}
					DelayMs(2500);
				}			
			}
			 i=0;
			 flush_flag=0;//刷屏标志位复位
			}break;
			case 3:
			{
				page_flag++;
			if(flush_flag == 0){//第一次显示
				OLED_Clear();
				OLED_ShowString(0,0,"T",16);
				OLED_ShowString(15,0,"min:",16);
				OLED_ShowString(75,0,"max:",16);
				OLED_ShowNum(47,0,tem_fa_min,3,16);
				OLED_ShowNum(105,0,tem_fa_max,3,16);
				
				OLED_ShowString(0,2,"H",16);
				OLED_ShowString(15,2,"min:",16);
				OLED_ShowString(75,2,"max:",16);
				OLED_ShowNum(47,2,hum_fa_min,3,16);
				OLED_ShowNum(105,2,hum_fa_max,3,16);
				
				OLED_ShowString(0,4,"L",16);
				OLED_ShowString(15,4,"min:",16);
				OLED_ShowString(75,4,"max:",16);
				OLED_ShowNum(47,4,light_fa_min,3,16);
				OLED_ShowNum(105,4,light_fa_max,3,16);
				
				OLED_ShowString(0,6,"S",16);
				OLED_ShowString(15,6,"min:",16);
				OLED_ShowString(75,6,"max:",16);
				OLED_ShowNum(47,6,soil_fa_min,3,16);
				OLED_ShowNum(105,6,soil_fa_max,3,16);
				
			}else{}
		 i=0;
		 flush_flag=0;//刷屏标志位复位
			}
			break;
	 }
			
			TIM_ClearITPendingBit(TIM6, TIM_IT_Update); //清除TIM6溢出中断标志
			}
			
			
	
//	
}
/*-------------------------------------------------*/
/*函数名：定时器4中断服务函数。处理MQTT数据          */
/*参  数：无                                       */
/*返回值：无                                       */
/*-------------------------------------------------*/
void TIM4_IRQHandler(void)
{
	if(TIM_GetITStatus(TIM4, TIM_IT_Update) != RESET)//如果TIM_IT_Update置位，表示TIM4溢出中断，进入if	
	{                	
		memcpy(&MQTT_RxDataInPtr[2], Usart2_RxBuff, Usart2_RxCounter);  //拷贝数据到接收缓冲区
		MQTT_RxDataInPtr[0] = Usart2_RxCounter/256;                  	//记录数据长度高字节
		MQTT_RxDataInPtr[1] = Usart2_RxCounter%256;	     				 	//记录数据长度低字节
		MQTT_RxDataInPtr += RBUFF_UNIT;                                	//指针下移
		if(MQTT_RxDataInPtr == MQTT_RxDataEndPtr)                     	//如果指针到缓冲区尾部了
		MQTT_RxDataInPtr = MQTT_RxDataBuf[0];                    	//指针归位到缓冲区开头
		Usart2_RxCounter = 0;                                        	//串口2接收数据量变量清零
		TIM_SetCounter(TIM3, 0);                                     	//清零定时器3计数器，重新计时ping包发送时间
		TIM_Cmd(TIM4, DISABLE);                        				 	//关闭TIM4定时器
		TIM_SetCounter(TIM4, 0);                        			 	//清零定时器4计数器
		TIM_ClearITPendingBit(TIM4, TIM_IT_Update);     			 	//清除TIM4溢出中断标志 	
	}
}

///*-------------------------------------------------*/
///*函数名：定时器5中断服务函数。处理USART3的数据，存入mqtt的发送缓冲区 */
///*参  数：无                                       */
///*返回值：无                                       */
///*-------------------------------------------------*/
//void TIM5_IRQHandler(void)
//{ char  tempAll[100]; 
//  int   tempLen;	
//	if(TIM_GetITStatus(TIM5, TIM_IT_Update) != RESET)//如果TIM_IT_Update置位，表示TIM5溢出中断，进入if	
//	{    
//		/*********usart3接受的只是报文的payload，组装成发送报文才可以放入发送缓冲区*******************/		
//		memcpy(tempAll, Usart3_RxBuff, Usart3_RxCounter);  //拷贝数据到接收缓冲区
//	  tempLen = Usart3_RxCounter;
//		
//		MQTT_PublishQs0(Data_TOPIC_NAME, tempAll, tempLen); //使用tempAll数组作为payload组装报文
//				
//		Usart3_RxCounter = 0;                                        	//串口3接收数据量变量清零
//		TIM_Cmd(TIM5, DISABLE);                        				 	//关闭TIM5定时器
//		TIM_SetCounter(TIM5, 0);                        			 	//清零定时器5计数器
//		TIM_ClearITPendingBit(TIM5, TIM_IT_Update);     			 	//清除TIM5溢出中断标志 	
//	}
//}

void TIM5_IRQHandler(void)
{
//	char  tempAll[100]; 
//  int   tempLen;	
	if(TIM_GetFlagStatus(TIM5, TIM_IT_Update) != RESET)//如果TIM_IT_Update置位，表示TIM5溢出中断，进入if	
	{    
		if(Flash_fa != 0 && Auto_ctro !=0)
		{
			// 断网保存阈值数据
//			sprintf((char *)Maxbuf, "%d:%d:%d:%d",tem_fa,hum_fa,light_fa,soil_fa);		// 每3秒保存一次
			sprintf((char *)Maxbuf, "%d:%d:%d:%d:%d:%d:%d:%d",tem_fa_min,tem_fa_max,hum_fa_min,hum_fa_max,
				light_fa_min,light_fa_max,soil_fa_min,soil_fa_max);
			u1_printf("write:%s\r\n", Maxbuf);
			STMFLASH_Write(FLASH_SAVE_ADDR,(u16*)Maxbuf,sizeof(Maxbuf));//写入flash
		}
		Flash_fa = 0;
	//	TIM_Cmd(TIM5, DISABLE);                        				 	//关闭TIM5定时器
		//TIM_SetCounter(TIM5, 0);                        			 	//清零定时器5计数器
		
	}
		TIM_ClearITPendingBit(TIM5, TIM_IT_Update);
}
/*-------------------------------------------------*/
/*函数名：定时器3中断服务函数                      */
/*参  数：无                                       */
/*返回值：无                                       */
/*-------------------------------------------------*/
void TIM3_IRQHandler(void)
{
	if(TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET)//如果TIM_IT_Update置位，表示TIM3溢出中断，进入if	
		{  
		switch(pingFlag) 					//判断pingFlag的状态
		{                               
			case 0:							//如果pingFlag等于0，表示正常状态，发送Ping报文  
					MQTT_PingREQ(); 		//添加Ping报文到发送缓冲区  
					break;
			case 1:							//如果pingFlag等于1，说明上一次发送到的ping报文，没有收到服务器回复，所以1没有被清除为0，可能是连接异常，我们要启动快速ping模式
					TIM3_ENABLE_2S(); 	    //我们将定时器3设置为2s定时,快速发送Ping报文
					MQTT_PingREQ();			//添加Ping报文到发送缓冲区  
					break;
			case 2:							//如果pingFlag等于2，说明还没有收到服务器回复
			case 3:				            //如果pingFlag等于3，说明还没有收到服务器回复
			case 4:				            //如果pingFlag等于4，说明还没有收到服务器回复	
					MQTT_PingREQ();  		//添加Ping报文到发送缓冲区 
					break;
			case 5:							//如果pingFlag等于5，说明我们发送了多次ping，均无回复，应该是连接有问题，我们重启连接
					connectFlag = 0;        //连接状态置0，表示断开，没连上服务器
					TIM_Cmd(TIM3, DISABLE); //关TIM3 				
					break;			
		}
		pingFlag++;           		   		//pingFlag自增1，表示又发送了一次ping，期待服务器的回复
		TIM_ClearITPendingBit(TIM3, TIM_IT_Update); //清除TIM3溢出中断标志 	
	}
}

/*-------------------------------------------------*/
/*函数名：定时器2中断服务函数                      */
/*参  数：无                                       */
/*返回值：无                                       */
/*-------------------------------------------------*/
	extern u16 ADC_Val;
void TIM2_IRQHandler(void)
{
//	float tempvalue;
//	u16 pm25;
  char tem,hum;	
	u8 oled_flag = 0;
	char tem_buff[3],hum_buff[3];
	char head1[3];
	char temp[50];				//定义一个临时缓冲区1,不包括报头
	char tempAll[256];			//定义一个临时缓冲区2，包括所有数据
	
	int	dataLen = 0;			//报文长度

	if(TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET)	
	{ 
//		tempvalue=DS18B20_GetTemperture();
		Light_val = Lsens_Get_Val();
		DHT11_Read_Data(&tem,&hum);
	
//		pm25 = GetGP2YSingleValue();
		Soil_val = Get_Soil_Adc_Average(ADC_Channel_1,10);
		tem_buff[0]=tem/10+0x30;
		tem_buff[1]=tem%10+0x30;
		tem_buff[2]='\0';
		
		hum_buff[0]=hum/10+0x30;
		hum_buff[1]=hum%10+0x30;
		hum_buff[2]='\0';
//		tempvalue=30;
		memset(temp,    0, 50);				    //清空缓冲区1
		memset(tempAll, 0, 256);				//清空缓冲区2
		memset(head1,   0, 3);					//清空MQTT头
		 
		u1_printf("转换前温度:%s\r\n",tem_buff);
		u1_printf("转换前湿度:%s\r\n",hum_buff);
		
		
		Hum_val = Myatoi(hum_buff);
		Tem_val = Myatoi(tem_buff);
		u1_printf("转换后温度:%d\r\n",Tem_val);
		u1_printf("转换后湿度:%d\r\n",Hum_val);
		//OLED_Clear();
//	OLED_ShowString(0,0,"Tem:",16);
//	OLED_ShowString(70,0,"Hum:",16);
//	OLED_ShowString(0,2,"Soil_H:",16);
//	OLED_ShowString(0,4,"Light:",16);
//		oled_dispaly();
//	  OLED_ShowNum(35,0,Tem_val,2,16);
//		OLED_ShowNum(100,0,Hum_val,2,16);
//		OLED_ShowNum(55,2,Soil_val,2,16);
//		OLED_ShowNum(50,4,Light_val,2,16);
//		OLED_ShowString(45,6,"OFF",16);
//		OLED_ShowNum(30,6,Soil_val,2,16);
//		OLED_ShowNum(100,6,Light_val,2,16);

//		step_flag = 0;
		if(Auto_ctro !=0 )
		{
//			oled_dispaly();
//			OLED_ShowNum(35,0,Tem_val,2,16);
//			OLED_ShowNum(100,0,Hum_val,2,16);
//			OLED_ShowNum(55,2,Soil_val,2,16);
//			OLED_ShowNum(50,4,Light_val,2,16);
//			OLED_ShowString(45,6,"ON",16);
			//-------------自动功能-------------
			if(Hum_val<=hum_fa_min)
			{
				beepFlag = "BEEPON";//打开水泵
				BEEP_On();
			}
			if(Hum_val>=hum_fa_max)
			{
				
				relayFlag = "RELAYON";//关闭除湿器
				Relay_On();
	
			}
			if(Hum_val>hum_fa_min && Hum_val<hum_fa_max)
			{
				beepFlag = "BEEPOFF";//关闭水泵
				BEEP_Off();
				//除湿
				relayFlag = "RELAYOFF";//打开除湿器
				Relay_Off();
//				beepFlag = "BEEPOFF";//关闭水泵
//				BEEP_Off();
			}
			if(Tem_val <= tem_fa_min)
			{
				ledFlag = "LEDON";//升温(打开光源)
				LED_On();
			}	
			else if(Tem_val >= tem_fa_max)
			{
//				relay2Flag = "RELAY2ON";//(通风机)降温
//				Relay2_On();
				fansFlag = "FANSON";//(通风机)降温
				Fans_On();
				
			}
			else
				{
					ledFlag = "LEDOFF";//(关闭光源)
					LED_Off();
//					relay2Flag = "RELAY2OFF";//(通风机)关闭
//					Relay2_Off();
				fansFlag = "FANSOFF";//(通风机)关闭
				Fans_Off();
				}
			if(Light_val <= light_fa_min)
			{
				led2Flag = "LED2ON";//补光(打开光源)
				LED2_On();
				
			}
			else	if(Light_val >= light_fa_max)
			{
//				step_flag = 0;
//				step_flag++;
				if(step_flag == 0 && step_on_flag == 0){
					STEP_MOTOR_LOOP(1,3,1);  //步进电机正转 遮阳
					step_flag++;
					step_on_flag = 1;
				}
				
//				ledFlag = "LEDOFF";//补光(打开光源)
//				LED_Off();
//				relay2Flag = "RELAY2ON";//遮阳
//				Relay2_On();
				
			}
			else
			{
				led2Flag = "LED2OFF";//(关闭光源)
				LED2_Off();
//				step_flag = 0;
//				step_flag++;
				if(step_flag == 1 && step_on_flag == 1)
				{
					STEP_MOTOR_LOOP(0,3,1);  //步进电机反转 开帘
					step_flag--;
					step_on_flag = 0;
				}
				
//				relay2Flag = "RELAY2OFF";//关闭
//				Relay2_Off();
			}
			if(Soil_val <= soil_fa_min)
			{
				relay2Flag = "RELAY2ON";//打开水泵
				Relay2_On();
//				Syn_flag=1;
			}
				if(Soil_val >= soil_fa_max)
			{
				relay2Flag = "RELAY2OFF";//关闭水泵
				Relay2_Off();
//				Syn_flag=1;
			}
//			if(Syn_flag ==1){
				Send_Status();//发送控制设备的状态数据
//				Syn_flag = 0;
//			}
			
//			if(Hum_val>=hum_fa)
//			{
//				ledFlag = "LEDON";
//				LED_On();
//			}else
//			{
//				ledFlag = "LEDOFF";
//				LED_Off();
//			}
//			if(Light_val>=light_fa)
//			{
//				beepFlag = "BEEPON";
//				BEEP_On();
//			}else
//			{
//				beepFlag = "BEEPOFF";
//				BEEP_Off();
//			}
//			if(Tem_val>=tem_fa)
//			{
//				relayFlag = "RELAYON";
//				Relay_On();
//			}else
//			{
//				relayFlag = "RELAYOFF";
//				Relay_Off();
//			}
		
	if(Tem_val<0) //如果温度为负
		{
//			sprintf(temp,"{\"Tem\":\"-%d\",\"Hum\":\"%d\"}",Tem_val,Hum_val);//构建报文
//			if(connectFlag == 1)
//				MQTT_PublishQs0(Data_TOPIC_NAME,temp, strlen(temp));
		}
		
		if(Hum_val<=100)
			{
			sprintf(temp,"{\"Solar\":{\"Light\":\"%d\"},\"DHT11\":{\"Tem\":\"%d\",\"Hum\":\"%d\"},\"GY16\":{\"Soil\":\"%d\"}}",Light_val,Tem_val,Hum_val,Soil_val);//构建报文
			T_json(1,0,temp,tempAll);
			//sprintf(temp,"{\"Tem\":\"%d\",\"Hum\":\"%d\"}",Tem_val,Hum_val);
			if(connectFlag == 1)
				MQTT_PublishQs0(Data_TOPIC_NAME,tempAll, strlen(tempAll));
				
			}
	}
	else  //如果温度为正
		{
			if(Hum_val<=100)
			{
			sprintf(temp,"{\"Solar\":{\"Light\":\"%d\"},\"DHT11\":{\"Tem\":\"%d\",\"Hum\":\"%d\"},\"GY16\":{\"Soil\":\"%d\"}}",Light_val,Tem_val,Hum_val,Soil_val);//构建报文
			T_json(1,0,temp,tempAll);
			//sprintf(temp,"{\"Tem\":\"%d\",\"Hum\":\"%d\"}",Tem_val,Hum_val);
			if(connectFlag == 1)
				MQTT_PublishQs0(Data_TOPIC_NAME,tempAll, strlen(tempAll));
				
			}
//			u1_printf("土壤湿度为：%d\r\n",Soil_val);
			//sprintf(temp,"{\"Light\":\"%d\",\"Temp\":\"%d\",\"Hum\":\"%d\",\"Soil\":\"%d\"}",Light_val,Tem_val,Hum_val,Soil_val);//构建报文\
			
			
		}
//		memset(temp, 0, sizeof(temp));
//		memset(tempAll, 0, sizeof(tempAll));
//		sprintf(temp,"{\"Soil\":\"%d\"}",Soil_val);
//		if(connectFlag == 1)
//				MQTT_PublishQs0(Data_TOPIC_NAME,temp, strlen(temp));
//		
//		memset(temp, 0, sizeof(temp));
//		sprintf(temp,"{\"Light\":\"%d\"}",Light_val);
//		if(connectFlag == 1)
//				MQTT_PublishQs0(Data_TOPIC_NAME,temp, strlen(temp));
		
		
  	/*****
		//该段代码是用于连接OneNet云平台，上传数据用的，在mqtt报文的payload部分，
		//按照OneNet的协议进行了修改。直连EMQX不需要，否则引入乱码。
		head1[0] = 0x03; 						//固定报头
		head1[1] = 0x00; 						//固定报头
		head1[2] = strlen(temp);  				//剩余长度	
		sprintf(tempAll, "%c%c%c%s", head1[0], head1[1], head1[2], temp);
			******/		
	
//		sprintf(tempAll, "%s", temp); //替代上面的注释部分，直连EMQX。  
	
		
//		u1_printf("\r\n"); //串口显示相关数据
////		u1_printf("haha%s\r\n",tem_buff);
////		u1_printf("xxx%s\r\n",hum_buff);
//		u1_printf("%s\r\n", tempAll);//连接OneNet云平台 tempAll+3
//		
//		/******	dataLen = strlen(temp) + 3;  //用于连接OneNet云平台	******/
//		dataLen = strlen(temp);  //替代上面的注释部分，直连EMQX。
		
//		MQTT_PublishQs0(Data_TOPIC_NAME,tempAll, dataLen);//添加数据，发布给服务器

		TIM_ClearITPendingBit(TIM2, TIM_IT_Update);   	
	}
	
}

/*-------------------------------------------------*/
/*函数名：不可屏蔽中断处理函数                     */
/*参  数：无                                       */
/*返回值：无                                       */
/*-------------------------------------------------*/
void NMI_Handler(void)
{

}

/*-------------------------------------------------*/
/*函数名：硬件出错后进入的中断处理函数             */
/*参  数：无                                       */
/*返回值：无                                       */
/*-------------------------------------------------*/
void HardFault_Handler(void)
{

}

/*-------------------------------------------------*/
/*函数名：内存管理中断处理函数                     */
/*参  数：无                                       */
/*返回值：无                                       */
/*-------------------------------------------------*/
void MemManage_Handler(void)
{

}

/*-------------------------------------------------*/
/*函数名：预取指失败，存储器访问失败中断处理函数   */
/*参  数：无                                       */
/*返回值：无                                       */
/*-------------------------------------------------*/
void BusFault_Handler(void)
{

}

/*-------------------------------------------------*/
/*函数名：未定义的指令或非法状态处理函数           */
/*参  数：无                                       */
/*返回值：无                                       */
/*-------------------------------------------------*/
void UsageFault_Handler(void)
{

}

/*-------------------------------------------------*/
/*函数名：软中断，SWI 指令调用的处理函数           */
/*参  数：无                                       */
/*返回值：无                                       */
/*-------------------------------------------------*/
void SVC_Handler(void)
{
	
}

/*-------------------------------------------------*/
/*函数名：调试监控器处理函数                       */
/*参  数：无                                       */
/*返回值：无                                       */
/*-------------------------------------------------*/
void DebugMon_Handler(void)
{
	
}

/*-------------------------------------------------*/
/*函数名：可挂起的系统服务处理函数                 */
/*参  数：无                                       */
/*返回值：无                                       */
/*-------------------------------------------------*/
void PendSV_Handler(void)
{
}

/*-------------------------------------------------*/
/*函数名：SysTic系统嘀嗒定时器处理函数             */
/*参  数：无                                       */
/*返回值：无                                       */
/*-------------------------------------------------*/
void SysTick_Handler(void)
{
 
}

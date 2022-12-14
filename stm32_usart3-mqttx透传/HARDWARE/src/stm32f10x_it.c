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
#include "lsens.h"
#include  "json.h"
#define FLASH_SAVE_ADDR  0X08070000
//外部变量
extern u8 tem_fa,soil_fa,hum_fa,light_fa;

extern u8 Flash_fa;
extern u8 Soil_val,Light_val,Hum_val;

extern s8 Tem_val;

extern u8 Auto_ctro;

u8 Maxbuf[11];
u8 oled_flag = 0;

extern char *ledFlag,*beepFlag,*relayFlag;

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
		if(Flash_fa != 0 && Auto_ctro !=0)			// 15s存一次
		{
			// 断网保存阈值数据
			sprintf((char *)Maxbuf, "%d:%d:%d:%d",tem_fa,hum_fa,light_fa,soil_fa);		// 每3秒保存一次
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
		 
		
		Hum_val = Myatoi(hum_buff);
		Tem_val = Myatoi(tem_buff);
		if(oled_flag%2==0 && Auto_ctro==0)
		{
			OLED_Clear();
			oled_flag++;
			oled_dispaly();
			OLED_ShowString(45,6,"OFF",16);
		}
		//OLED_Clear();
//	OLED_ShowString(0,0,"Tem:",16);
//	OLED_ShowString(70,0,"Hum:",16);
//	OLED_ShowString(0,2,"Soil_H:",16);
//	OLED_ShowString(0,4,"Light:",16);
		
	  OLED_ShowNum(35,0,Tem_val,2,16);
		OLED_ShowNum(100,0,Hum_val,2,16);
		OLED_ShowNum(55,2,Soil_val,2,16);
		OLED_ShowNum(50,4,Light_val,2,16);
		
//		OLED_ShowNum(30,6,Soil_val,2,16);
//		OLED_ShowNum(100,6,Light_val,2,16);
		if(Auto_ctro !=0)
		{
			if(oled_flag%2==1)
			{
				OLED_Clear();
				oled_flag++;
				oled_dispaly();
				OLED_ShowString(45,6,"ON",16);
			}
			
			OLED_ShowNum(35,0,Tem_val,2,16);
			OLED_ShowNum(100,0,Hum_val,2,16);
			OLED_ShowNum(55,2,Soil_val,2,16);
			OLED_ShowNum(50,4,Light_val,2,16);
			
			if(Hum_val>=hum_fa)
			{
				ledFlag = "LEDON";
				LED_On();
			}else
			{
				ledFlag = "LEDOFF";
				LED_Off();
			}
//			if(Light_val>=light_fa)
//			{
//				beepFlag = "BEEPON";
//				BEEP_On();
//			}else
//			{
//				beepFlag = "BEEPOFF";
//				BEEP_Off();
//			}
			if(Tem_val>=tem_fa)
			{
				relayFlag = "RELAYON";
				Relay_On();
			}else
			{
				relayFlag = "RELAYOFF";
				Relay_Off();
			}
		}
		
	if(Tem_val<0) //如果温度为负
		{
			sprintf(temp,"{\"Tem\":\"-%d\",\"Hum\":\"%d\"}",Tem_val,Hum_val);//构建报文
			if(connectFlag == 1)
				MQTT_PublishQs0(Data_TOPIC_NAME,temp, strlen(temp));
		}
	else  //如果温度为正
		{
//			u1_printf("土壤湿度为：%d\r\n",Soil_val);
			//sprintf(temp,"{\"Light\":\"%d\",\"Temp\":\"%d\",\"Hum\":\"%d\",\"Soil\":\"%d\"}",Light_val,Tem_val,Hum_val,Soil_val);//构建报文\
			
			sprintf(temp,"{\"Solar\":{\"Light\":\"%d\"},\"DHT11\":{\"Tem\":\"%d\",\"Hum\":\"%d\"},\"GY16\":{\"Soil\":\"%d\"}}",Light_val,Tem_val,Hum_val,Soil_val);//构建报文
			T_json(1,0,temp,tempAll);
			//sprintf(temp,"{\"Tem\":\"%d\",\"Hum\":\"%d\"}",Tem_val,Hum_val);
			if(connectFlag == 1)
				MQTT_PublishQs0(Data_TOPIC_NAME,tempAll, strlen(tempAll));
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

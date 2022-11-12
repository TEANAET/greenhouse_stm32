 /*-----------------------------------------------------*/
/*                                                     */
/*            程序main函数，入口函数源文件              */
/*                                                     */
/*-----------------------------------------------------*/

#include "stm32f10x.h"   //包含需要的头文件
#include "control.h"     //包含需要的头文件
#include "delay.h"       //包含需要的头文件
#include "usart1.h"      //包含需要的头文件
#include "usart2.h"      //包含需要的头文件
#include "usart3.h"      //包含需要的头文件
#include "timer2.h"      //包含需要的头文件
#include "timer3.h"      //包含需要的头文件
#include "timer4.h"      //包含需要的头文件
#include "timer5.h"      //包含需要的头文件
#include "wifi.h"	     //包含需要的头文件
#include "led.h"	     //包含需要的头文件 LED                                                  
#include "mqtt.h"        //包含需要的头文件
#include "beep.h"
#include "lsens.h"
#include "Relay.h"
#include "stmflash.h"
#include "adc.h"
#include "dht11.h"       //包含需要的头文件 空气温湿度
#include "MQ_Sensor.h"
#include "oled.h"
#include <stdlib.h>

#define FLASH_SAVE_ADDR  0X08070000		//设置FLASH 保存地址(必须为偶数，且其值要大于本代码所占用FLASH的大小+0X08000000)


/********************数据值*********************************/
 s8 Tem_val = 0;					// 温度
 u8 Hum_val = 0;					// 湿度
 u8 Light_val = 0;				// 光照度
 u8 Soil_val = 0;
 

/******************阈值命令前缀**********
温度阈值：$TEM#S45*  阈值为45
土壤湿度阈值：$Soil#S45*  阈值为45
光照度阈值：$LIGHT#S60*  阈值为60
*****************************************/
char *LightFA="$LIGHT#S";
char *TemFA="$TEM#S";
char *HumFA="$HUM#S";
char *SoilFA="$Soil#S";


/*********************阈值*******************************/
	s8 tem_fa=0;//温度阈值
	u8 soil_fa=0;//土壤湿度阈值
	u8 hum_fa=0;//湿度阈值
	u8 light_fa=0;//光照阈值


/**********************FLASH***********************************/
	u8 Max_data_buf[11] = {0};//阈值数据缓冲区
	u8 Flash_fa = 0;//存入Flash标志位


/*********************状态标志位***************************/
// 状态标志位
 u8 Auto_ctro = 0;					// 自动控制 默认为0（手动）
 u8 Air_flag = 0;				// 风扇1状态	0：停止
 u8 Light_flag = 0;				// 可调灯状态



/************************自动控制命令***********************/
char *cmdAuto_On = "AUTO_ON";				//自动控制开
char *cmdAuto_Off = "AUTO_OFF";			//自动控制关

/********************控制指令*****************************/
char *cmdLED_On = "LEDON";      //LED打开
char *cmdLED_Off = "LEDOFF";    //LED关闭
char *cmdBEEP_On = "BEEPON";      //蜂鸣器·打开
char *cmdBEEP_Off = "BEEPOFF";    //蜂鸣器关闭
char *cmdRelay_On = "RELAYON";      //继电器·打开
char *cmdRelay_Off = "RELAYOFF";    //继电器关闭



char *cmdRelay2_On = "RELAY2ON";      //继电器·打开
char *cmdRelay2_Off = "RELAY2OFF";    //继电器关闭

/*********************状态标志位****************************/
char *ledFlag = "LEDOFF";       //LED状态
char *beepFlag = "BEEPOFF";       //蜂鸣器状态
char *relayFlag = "RELAYOFF";     //继电器状态
//int   dhtFlag = 0;		        //温湿度数据传输状态
u8 adcx;
int main(void) 
{	

	Delay_Init();          //systick延时功能初始化              
	Usart1_Init(115200);   //串口1功能初始化，波特率115200
	Usart2_Init(115200);   //串口2功能初始化，波特率115200	
//	Usart3_Init(115200);   
	TIM4_Init(500,7200);   //TIM4初始化，定时时间 500*7200*1000/72000000 = 50ms
	  //TIM5初始化，定时时间 500*7200*1000/72000000 = 50ms
	LED_Init();			   //LED初始化
	BEEP_Init();         	//初始化蜂鸣器端口
	Lsens_Init(); 			//初始化光敏传感器
	DHT11_Init();          //初始化DHT11	
	Relay_Init();        //继电器初始化
	Relay2_Init();			//继电器2初始化
	Adc_Soil_Init();     //土壤湿度初始化
	OLED_Init();
	OLED_Clear();
	
//	GP2Y_ADC_init();
	
	

	//Adc_Init();
	WiFi_EnableIO_Init();  //初始化WiFi模块ESP8266的使能引脚PD/EN
	WiFi_ResetIO_Init();   //初始化WiFi的复位IO
	IoT_Parameter_Init();  //初始化MQTT服务器的参数	
	if(connectFlag == 0)
		{
			
			OLED_ShowString(0,6,"connect...",16);
		}
	
		// 读取阈值
	STMFLASH_Read(FLASH_SAVE_ADDR,(u16*)Max_data_buf,sizeof(Max_data_buf));					// 从flash读出阈值
	u1_printf("11111111111111fazhi:%s\r\n", Max_data_buf);
	sscanf((char *)Max_data_buf,"%hhu:%hhu:%hhu:%hhu", &tem_fa, &hum_fa, &light_fa,&soil_fa);
	u1_printf("fazhi:%d,%d,%d,%d\r\n", tem_fa,hum_fa,light_fa,soil_fa);
//	OLED_Clear();
//	OLED_ShowString(0,0,"Tem:",16);
//	OLED_ShowString(70,0,"Hum:",16);
//	OLED_ShowString(0,2,"Soil_H:",16);
//	OLED_ShowString(0,4,"Light:",16);
	while(1)
while(1)
	{		
		
//		DHT11_Read_Data(tem,hum);
//		u1_printf("%s\r%s\r\n",tem,hum);
		// connectFlag=1同服务器建立了连接 
		
		if(connectFlag == 1) 
		{     
			/*-------------------------------------------------------------*/
			/*                     处理发送缓冲区数据						 */
			/*-------------------------------------------------------------*/
			if(MQTT_TxDataOutPtr != MQTT_TxDataInPtr) //if成立的话，说明发送缓冲区有数据了
																							  //缓冲区的指针不等于初始值，被移动过。
			{                
				//3种情况可进入if
				//第1种：0x10 连接报文。0x10和0x82为固定头
				//第2种：0x82 订阅报文且连接报文成功。当connectPackFlag置位，表示连接报文成功
				//			 0x10和0x82为固定头。当报文放入发送缓冲区是，是从第3个字节开始放的，index为2。参考mqtt.c的TxDataBuf_Deal（）函数
				//第3种：其他报文。当subcribePackFlag置位，说明连接和订阅均成功，其他报文可发。
				if((MQTT_TxDataOutPtr[2] == 0x10)||((MQTT_TxDataOutPtr[2] == 0x82)&&(connectPackFlag == 1))||(subcribePackFlag == 1)) 
				{    
					if (MQTT_TxDataOutPtr[2] == 0x30) 
					{	
						u1_printf("发送数据:0x%x，单片机数据推送至服务器\r\n", MQTT_TxDataOutPtr[2]);	
						u1_printf("\r\n");
					}
					else 
					{
						u1_printf("发送数据:0x%x\r\n", MQTT_TxDataOutPtr[2]);//串口提示信息
					}
					
					MQTT_TxData(MQTT_TxDataOutPtr);							//发送数据
					MQTT_TxDataOutPtr += TBUFF_UNIT;						//指针下移
					if(MQTT_TxDataOutPtr == MQTT_TxDataEndPtr)				//如果指针到缓冲区尾部了
					{ 
						MQTT_TxDataOutPtr = MQTT_TxDataBuf[0];				//指针归位到缓冲区开头
					}	
				} 				
			}
			/*-------------------------------------------------------------*/
			/*                     处理接收缓冲区数据                      */
			/*-------------------------------------------------------------*/
			if(MQTT_RxDataOutPtr != MQTT_RxDataInPtr) //if成立的话，说明接收缓冲区有数据了	
			{												
				u1_printf("接收到数据:");
				/*-----------------------------------------------------*/
				/*                    处理CONNACK报文                  */
				/*-----------------------------------------------------*/				
				//if判断，如果第一个字节是0x20，表示收到的是CONNACK报文
				//接着我们要判断第4个字节，看看CONNECT报文是否成功
				if(MQTT_RxDataOutPtr[2] == 0x20)
				{             			
					switch(MQTT_RxDataOutPtr[5])
					{					   
						case 0x00 : u1_printf("CONNECT报文成功\r\n");							//串口输出信息	
										connectPackFlag = 1;									//CONNECT报文成功，订阅报文可发
									break;														//跳出分支case 0x00                                              
						case 0x01 : u1_printf("连接已拒绝，不支持的协议版本，准备重启\r\n");    //串口输出信息
									connectFlag = 0;											//connectFlag置零，重启连接
									break;														//跳出分支case 0x01   
						case 0x02 : u1_printf("连接已拒绝，不合格的客户端标识符，准备重启\r\n");//串口输出信息
									connectFlag = 0;                                            //connectFlag置零，重启连接
									break; 														//跳出分支case 0x02 
						case 0x03 : u1_printf("连接已拒绝，服务端不可用，准备重启\r\n");	    //串口输出信息
									connectFlag = 0;											//connectFlag置零，重启连接
									break;														//跳出分支case 0x03
						case 0x04 : u1_printf("连接已拒绝，无效的用户名或密码，准备重启\r\n");	//串口输出信息
									connectFlag = 0;											//connectFlag置零，重启连接						
									break;														//跳出分支case 0x04
						case 0x05 : u1_printf("连接已拒绝，未授权，准备重启\r\n");				//串口输出信息
									connectFlag = 0;											//connectFlag置零，重启连接						
									break;														//跳出分支case 0x05 		
						default   : u1_printf("连接已拒绝，未知状态，准备重启\r\n");		    //串口输出信息 
									connectFlag = 0;											//connectFlag置零，重启连接					
									break;														//跳出分支case default 								
					}				
			    }			
				//if判断，第一个字节是0x90，表示收到的是SUBACK报文
				//接着我们要判断订阅回复，看看是不是成功
				else if(MQTT_RxDataOutPtr[2] == 0x90)
				{ 
					switch(MQTT_RxDataOutPtr[6])
					{					
						case 0x00 :
						case 0x01 : 
									u1_printf("订阅成功\r\n");			//串口输出信息
									subcribePackFlag = 1;				//subcribePackFlag置1，表示订阅报文成功，其他报文可发送
									pingFlag = 0;						//pingFlag清零
									TIM3_ENABLE_30S();					//启动30s的PING定时器	
									Send_Data();
									TIM5_Init(10000,36000); //定时15s,保存一次阈值
									TIM2_ENABLE_5S();
									break;								//跳出分支                                             
						default: 
									u1_printf("订阅失败，准备重启\r\n");//串口输出信息 
									connectFlag = 0;					//connectFlag置零，重启连接
									break;								//跳出分支 								
						}					
			    }
				//if判断，第一个字节是0xD0，表示收到的是PINGRESP报文
				else if(MQTT_RxDataOutPtr[2] == 0xD0)
				{ 
					u1_printf("PING报文回复\r\n");                      //串口输出信息 
					if(pingFlag == 1)
					{                   						        //如果pingFlag=1，表示第一次发送
						 pingFlag = 0;    				       			//要清除pingFlag标志
					}
					else if(pingFlag > 1)	
					{ 				 									//如果pingFlag>1，表示是多次发送了，而且是2s间隔的快速发送
						pingFlag = 0;     				      			//要清除pingFlag标志
						TIM3_ENABLE_30S(); 				      			//PING定时器重回30s的时间
					}				
				}	
				//if判断，如果第一个字节是0x30，表示收到的是服务器发来的推送数据
				//我们要提取控制命令
				else if(MQTT_RxDataOutPtr[2] == 0x30)
			    { 
					u1_printf("服务器等级0推送\r\n"); 		   	//串口输出信息 
					MQTT_DealPushdata_Qs0(MQTT_RxDataOutPtr);   //处理等级0推送数据
				}									
				MQTT_RxDataOutPtr += RBUFF_UNIT;                //指针下移
				if(MQTT_RxDataOutPtr == MQTT_RxDataEndPtr)      //如果指针到缓冲区尾部了
				{
					MQTT_RxDataOutPtr = MQTT_RxDataBuf[0];      //指针归位到缓冲区开头              
				}		          
			}//处理接收缓冲区数据的else if分支结尾
						
			/*---------------------------------------------------------------------------------------------------------------------*/
			/*                    							 处理命令缓冲区数据                   		          			         */
			/*---------------------------------------------------------------------------------------------------------------------*/
			
			if(MQTT_CMDOutPtr != MQTT_CMDInPtr)							  //if成立的话，说明命令缓冲区有数据了	
			{  				
				char *temp;
				u1_printf("命令:%s\r\n",&MQTT_CMDOutPtr[2]);              //串口输出信息
				
				
			 if(!memcmp(&MQTT_CMDOutPtr[2],cmdAuto_On,strlen(cmdAuto_On)))
				{
					Auto_ctro=1;//自动控制标志位置1
					
					u1_printf("自动控制开!\r\n");
					
				}
				else if(!memcmp(&MQTT_CMDOutPtr[2],cmdAuto_Off,strlen(cmdAuto_Off)))//如果是自动控制关命令
				{    
					Auto_ctro = 0;                 //自动控制标志位置0
					u1_printf("自动控制关!\r\n");
					Flash_fa=0;
				}
				else if(Auto_ctro != 0)//打开了自动控制
				{
				 if(!memcmp(&MQTT_CMDOutPtr[2],TemFA, 6))
					{
					// 进行切割得到对应阈值
						strtok((char *)&MQTT_CMDOutPtr[2], "S");
						temp = strtok(NULL, "S");					// eg:得到xx*
						tem_fa = (u8)atoi(strtok(temp, "*"));			// 得到温度阈值	  
						u1_printf("温度阈值：%d\r\n",tem_fa);
						Flash_fa = 1;//存入Flash标志位
					}
					
					else if(!memcmp(&MQTT_CMDOutPtr[2],HumFA, 6))//湿度阈值命令
					{     
						// 进行切割得到对应阈值
						strtok((char *)&MQTT_CMDOutPtr[2], "S");
						temp = strtok(NULL, "S");					// eg:μ?μ?30.5*
						hum_fa = (u8)atoi(strtok(temp, "*"));	// 得到湿度阈值	
						u1_printf("湿度度阈值：%d\r\n",hum_fa);
						Flash_fa = 1;//存入Flash标志位//存入Flash标志位
					}
					else if(!memcmp(&MQTT_CMDOutPtr[2],LightFA, strlen(LightFA)))//阈值命令
					{     
						// 进行切割得到对应阈值
						strtok((char *)&MQTT_CMDOutPtr[2], "S");
						temp = strtok(NULL, "S");					// eg:μ?μ?30.5*
						light_fa = (u8)atoi(strtok(temp, "*"));	// 得到光照度阈值	
						u1_printf("光照度阈值：%d\r\n",light_fa);
						Flash_fa = 1;//存入Flash标志位
					}
					
					else if(!memcmp(&MQTT_CMDOutPtr[2],SoilFA, strlen(SoilFA)))//阈值命令
					{     
						// 进行切割得到对应阈值
						strtok((char *)&MQTT_CMDOutPtr[2], "S");
						temp = strtok(NULL, "S");					// eg:μ?μ?30.5*
						soil_fa = (u8)atoi(strtok(temp, "*"));	// 得到土壤湿度阈值	
						u1_printf("土壤湿度阈值：%d\r\n",soil_fa);
						Flash_fa = 1;//存入Flash标志位
					}
					
				//	Flash_fa = 1;//存入Flash标志位
				}
				else if(Auto_ctro != 1)				// 手动模式，接受手动状态命令
				{
						
						if(!memcmp(&MQTT_CMDOutPtr[2],cmdLED_On,strlen(cmdLED_On)))//判断指令，如果是CMD1 
					{                                            
						ledFlag = "LEDON";  								             
						LED_On();									  		   //LED开启
						u1_printf("LED开启\r\n");
					}
					else if(!memcmp(&MQTT_CMDOutPtr[2],cmdLED_Off,strlen(cmdLED_Off))) //判断指令，如果是CMD11
					{ 
						ledFlag = "LEDOFF";                                              
						LED_Off(); 								                //LED关闭
						u1_printf("LED关闭\r\n");
					}
					else if(!memcmp(&MQTT_CMDOutPtr[2],cmdBEEP_On,strlen(cmdBEEP_On))) //判断指令，如果是CMD11
					{ 
						beepFlag = "BEEPON";                                           
						BEEP_On(); 								                //LED关闭
						u1_printf("蜂鸣器开启\r\n");
					}
					else if(!memcmp(&MQTT_CMDOutPtr[2],cmdBEEP_Off,strlen(cmdBEEP_Off))) //判断指令，如果是CMD11
					{ 
						beepFlag = "BEEPOFF";                                           
						BEEP_Off();
						u1_printf("蜂鸣器关闭\r\n");
						
					}
					else if(!memcmp(&MQTT_CMDOutPtr[2],cmdRelay_On,strlen(cmdRelay_On))) //判断指令，如果是CMD11
					{ 
						relayFlag = "RELAYON";                                           
						Relay_On(); 								                //LED关闭
						Air_flag=1;
						u1_printf("继电器开启\r\n");
					}
					else if(!memcmp(&MQTT_CMDOutPtr[2],cmdRelay_Off,strlen(cmdRelay_Off))) //判断指令，如果是CMD11
					{ 
						relayFlag = "RELAYOFF";                                           
						Relay_Off();
						Air_flag=0;
						u1_printf("继电器关闭\r\n");
						
					}
					
					else if(!memcmp(&MQTT_CMDOutPtr[2],cmdRelay2_On,strlen(cmdRelay2_On))) //判断指令，如果是CMD11
					{ 
//						relayFlag = "RELAYON";                                           
						Relay2_On(); 								                //LED关闭
//						Air_flag=1;
						u1_printf("继电器2开启\r\n");
					}
					else if(!memcmp(&MQTT_CMDOutPtr[2],cmdRelay2_Off,strlen(cmdRelay2_Off))) //判断指令，如果是CMD11
					{ 
//					relayFlag = "RELAYOFF";                                           
						Relay2_Off();
//						Air_flag=0;
						u1_printf("继电器2关闭\r\n");
						
					}
				}
				
				
				
				//不做处理，后面会发送控制器状态
				else 
					{
					 u1_printf("未知指令\r\n");				//串口输出信息	
					
//					 u3_TxData(MQTT_CMDOutPtr);       //串口3发送命令缓冲区的数据，MQTT_CMDOutPtr的前两个元素为命令长度。
					}
				
//				else if(!memcmp(&MQTT_CMDOutPtr[2],cmdFA,strlen((char *)&MQTT_CMDOutPtr[2])-2))//阀值
			
//				else if(!memcmp(&MQTT_CMDOutPtr[2],cmdDHT11_On,strlen(cmdDHT11_On)))//判断指令，如果是CMD3
//				{      
//					dhtFlag = 1;                 //dataFlag置1，表示处于采集状态了
//					TIM2_ENABLE_10S();           //定时器2，,10s间隔采集温湿度   
//				}
//				else if(!memcmp(&MQTT_CMDOutPtr[2],cmdDHT11_Off,strlen(cmdDHT11_Off)))//判断指令，如果是CMD3
//				{     
//					dhtFlag = 0;                 //dataFlag置0，表示处于停止状态了
//					TIM_Cmd(TIM2,DISABLE);		 //判断2路开关状态和采集状态，并发布给服务器
//				}								
				
			
				MQTT_CMDOutPtr += CBUFF_UNIT;				//指针下移
				if(MQTT_CMDOutPtr == MQTT_CMDEndPtr)	    //如果指针到缓冲区尾部了
				MQTT_CMDOutPtr = MQTT_CMDBuf[0];			//指针归位到缓冲区开头	
				
				Send_Data();//发送控制设备的状态数据
				//处理命令缓冲区数据的else if分支结尾		
//				temp=NULL;			//将temp置空	
			}	//connectFlag=1的if分支的结尾
			
		}
	/*--------------------------------------------------------------------*/
	/*      connectFlag=0同服务器断开了连接,要重启连接服务器            */
	/*--------------------------------------------------------------------*/
		else
		{ 
			u1_printf("需要连接服务器\r\n");               //串口输出信息
			TIM_Cmd(TIM4, DISABLE);                        //关闭TIM4 
			TIM_Cmd(TIM3, DISABLE);                        //关闭TIM3  
			WiFi_RxCounter = 0;                            //WiFi接收数据量变量清零                        
			memset(WiFi_RX_BUF, 0, WiFi_RXBUFF_SIZE);      //清空WiFi接收缓冲区 
			
			/**WiFi_Connect_IoTServer()启动ESP8266，连接热点**/
			if(WiFi_Connect_IoTServer() == 0)			   //如果WiFi连接云服务器函数返回0，表示正确，进入if
			{   			     
				u1_printf("建立TCP连接成功\r\n");          //串口输出信息
				connectFlag = 1;                           //connectFlag置1，表示连接成功	
				OLED_Clear();
				oled_dispaly();//显示固定字符
				//OLED_ShowString(0,2,"connect successful!",16);
				WiFi_RxCounter = 0;                        //WiFi接收数据量变量清零                        
				memset(WiFi_RX_BUF, 0, WiFi_RXBUFF_SIZE);  //清空WiFi接收缓冲区 
				MQTT_Buff_Init();                          //初始化发送缓冲区                    
			}				
		}
	}
}

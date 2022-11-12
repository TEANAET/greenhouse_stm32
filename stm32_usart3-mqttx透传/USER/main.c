 /*-----------------------------------------------------*/
/*                                                     */
/*            ����main��������ں���Դ�ļ�              */
/*                                                     */
/*-----------------------------------------------------*/

#include "stm32f10x.h"   //������Ҫ��ͷ�ļ�
#include "control.h"     //������Ҫ��ͷ�ļ�
#include "delay.h"       //������Ҫ��ͷ�ļ�
#include "usart1.h"      //������Ҫ��ͷ�ļ�
#include "usart2.h"      //������Ҫ��ͷ�ļ�
#include "usart3.h"      //������Ҫ��ͷ�ļ�
#include "timer2.h"      //������Ҫ��ͷ�ļ�
#include "timer3.h"      //������Ҫ��ͷ�ļ�
#include "timer4.h"      //������Ҫ��ͷ�ļ�
#include "timer5.h"      //������Ҫ��ͷ�ļ�
#include "wifi.h"	     //������Ҫ��ͷ�ļ�
#include "led.h"	     //������Ҫ��ͷ�ļ� LED                                                  
#include "mqtt.h"        //������Ҫ��ͷ�ļ�
#include "beep.h"
#include "lsens.h"
#include "Relay.h"
#include "stmflash.h"
#include "adc.h"
#include "dht11.h"       //������Ҫ��ͷ�ļ� ������ʪ��
#include "MQ_Sensor.h"
#include "oled.h"
#include <stdlib.h>

#define FLASH_SAVE_ADDR  0X08070000		//����FLASH �����ַ(����Ϊż��������ֵҪ���ڱ�������ռ��FLASH�Ĵ�С+0X08000000)


/********************����ֵ*********************************/
 s8 Tem_val = 0;					// �¶�
 u8 Hum_val = 0;					// ʪ��
 u8 Light_val = 0;				// ���ն�
 u8 Soil_val = 0;
 

/******************��ֵ����ǰ׺**********
�¶���ֵ��$TEM#S45*  ��ֵΪ45
����ʪ����ֵ��$Soil#S45*  ��ֵΪ45
���ն���ֵ��$LIGHT#S60*  ��ֵΪ60
*****************************************/
char *LightFA="$LIGHT#S";
char *TemFA="$TEM#S";
char *HumFA="$HUM#S";
char *SoilFA="$Soil#S";


/*********************��ֵ*******************************/
	s8 tem_fa=0;//�¶���ֵ
	u8 soil_fa=0;//����ʪ����ֵ
	u8 hum_fa=0;//ʪ����ֵ
	u8 light_fa=0;//������ֵ


/**********************FLASH***********************************/
	u8 Max_data_buf[11] = {0};//��ֵ���ݻ�����
	u8 Flash_fa = 0;//����Flash��־λ


/*********************״̬��־λ***************************/
// ״̬��־λ
 u8 Auto_ctro = 0;					// �Զ����� Ĭ��Ϊ0���ֶ���
 u8 Air_flag = 0;				// ����1״̬	0��ֹͣ
 u8 Light_flag = 0;				// �ɵ���״̬



/************************�Զ���������***********************/
char *cmdAuto_On = "AUTO_ON";				//�Զ����ƿ�
char *cmdAuto_Off = "AUTO_OFF";			//�Զ����ƹ�

/********************����ָ��*****************************/
char *cmdLED_On = "LEDON";      //LED��
char *cmdLED_Off = "LEDOFF";    //LED�ر�
char *cmdBEEP_On = "BEEPON";      //����������
char *cmdBEEP_Off = "BEEPOFF";    //�������ر�
char *cmdRelay_On = "RELAYON";      //�̵�������
char *cmdRelay_Off = "RELAYOFF";    //�̵����ر�



char *cmdRelay2_On = "RELAY2ON";      //�̵�������
char *cmdRelay2_Off = "RELAY2OFF";    //�̵����ر�

/*********************״̬��־λ****************************/
char *ledFlag = "LEDOFF";       //LED״̬
char *beepFlag = "BEEPOFF";       //������״̬
char *relayFlag = "RELAYOFF";     //�̵���״̬
//int   dhtFlag = 0;		        //��ʪ�����ݴ���״̬
u8 adcx;
int main(void) 
{	

	Delay_Init();          //systick��ʱ���ܳ�ʼ��              
	Usart1_Init(115200);   //����1���ܳ�ʼ����������115200
	Usart2_Init(115200);   //����2���ܳ�ʼ����������115200	
//	Usart3_Init(115200);   
	TIM4_Init(500,7200);   //TIM4��ʼ������ʱʱ�� 500*7200*1000/72000000 = 50ms
	  //TIM5��ʼ������ʱʱ�� 500*7200*1000/72000000 = 50ms
	LED_Init();			   //LED��ʼ��
	BEEP_Init();         	//��ʼ���������˿�
	Lsens_Init(); 			//��ʼ������������
	DHT11_Init();          //��ʼ��DHT11	
	Relay_Init();        //�̵�����ʼ��
	Relay2_Init();			//�̵���2��ʼ��
	Adc_Soil_Init();     //����ʪ�ȳ�ʼ��
	OLED_Init();
	OLED_Clear();
	
//	GP2Y_ADC_init();
	
	

	//Adc_Init();
	WiFi_EnableIO_Init();  //��ʼ��WiFiģ��ESP8266��ʹ������PD/EN
	WiFi_ResetIO_Init();   //��ʼ��WiFi�ĸ�λIO
	IoT_Parameter_Init();  //��ʼ��MQTT�������Ĳ���	
	if(connectFlag == 0)
		{
			
			OLED_ShowString(0,6,"connect...",16);
		}
	
		// ��ȡ��ֵ
	STMFLASH_Read(FLASH_SAVE_ADDR,(u16*)Max_data_buf,sizeof(Max_data_buf));					// ��flash������ֵ
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
		// connectFlag=1ͬ���������������� 
		
		if(connectFlag == 1) 
		{     
			/*-------------------------------------------------------------*/
			/*                     �����ͻ���������						 */
			/*-------------------------------------------------------------*/
			if(MQTT_TxDataOutPtr != MQTT_TxDataInPtr) //if�����Ļ���˵�����ͻ�������������
																							  //��������ָ�벻���ڳ�ʼֵ�����ƶ�����
			{                
				//3������ɽ���if
				//��1�֣�0x10 ���ӱ��ġ�0x10��0x82Ϊ�̶�ͷ
				//��2�֣�0x82 ���ı��������ӱ��ĳɹ�����connectPackFlag��λ����ʾ���ӱ��ĳɹ�
				//			 0x10��0x82Ϊ�̶�ͷ�������ķ��뷢�ͻ������ǣ��Ǵӵ�3���ֽڿ�ʼ�ŵģ�indexΪ2���ο�mqtt.c��TxDataBuf_Deal��������
				//��3�֣��������ġ���subcribePackFlag��λ��˵�����ӺͶ��ľ��ɹ����������Ŀɷ���
				if((MQTT_TxDataOutPtr[2] == 0x10)||((MQTT_TxDataOutPtr[2] == 0x82)&&(connectPackFlag == 1))||(subcribePackFlag == 1)) 
				{    
					if (MQTT_TxDataOutPtr[2] == 0x30) 
					{	
						u1_printf("��������:0x%x����Ƭ������������������\r\n", MQTT_TxDataOutPtr[2]);	
						u1_printf("\r\n");
					}
					else 
					{
						u1_printf("��������:0x%x\r\n", MQTT_TxDataOutPtr[2]);//������ʾ��Ϣ
					}
					
					MQTT_TxData(MQTT_TxDataOutPtr);							//��������
					MQTT_TxDataOutPtr += TBUFF_UNIT;						//ָ������
					if(MQTT_TxDataOutPtr == MQTT_TxDataEndPtr)				//���ָ�뵽������β����
					{ 
						MQTT_TxDataOutPtr = MQTT_TxDataBuf[0];				//ָ���λ����������ͷ
					}	
				} 				
			}
			/*-------------------------------------------------------------*/
			/*                     ������ջ���������                      */
			/*-------------------------------------------------------------*/
			if(MQTT_RxDataOutPtr != MQTT_RxDataInPtr) //if�����Ļ���˵�����ջ�������������	
			{												
				u1_printf("���յ�����:");
				/*-----------------------------------------------------*/
				/*                    ����CONNACK����                  */
				/*-----------------------------------------------------*/				
				//if�жϣ������һ���ֽ���0x20����ʾ�յ�����CONNACK����
				//��������Ҫ�жϵ�4���ֽڣ�����CONNECT�����Ƿ�ɹ�
				if(MQTT_RxDataOutPtr[2] == 0x20)
				{             			
					switch(MQTT_RxDataOutPtr[5])
					{					   
						case 0x00 : u1_printf("CONNECT���ĳɹ�\r\n");							//���������Ϣ	
										connectPackFlag = 1;									//CONNECT���ĳɹ������ı��Ŀɷ�
									break;														//������֧case 0x00                                              
						case 0x01 : u1_printf("�����Ѿܾ�����֧�ֵ�Э��汾��׼������\r\n");    //���������Ϣ
									connectFlag = 0;											//connectFlag���㣬��������
									break;														//������֧case 0x01   
						case 0x02 : u1_printf("�����Ѿܾ������ϸ�Ŀͻ��˱�ʶ����׼������\r\n");//���������Ϣ
									connectFlag = 0;                                            //connectFlag���㣬��������
									break; 														//������֧case 0x02 
						case 0x03 : u1_printf("�����Ѿܾ�������˲����ã�׼������\r\n");	    //���������Ϣ
									connectFlag = 0;											//connectFlag���㣬��������
									break;														//������֧case 0x03
						case 0x04 : u1_printf("�����Ѿܾ�����Ч���û��������룬׼������\r\n");	//���������Ϣ
									connectFlag = 0;											//connectFlag���㣬��������						
									break;														//������֧case 0x04
						case 0x05 : u1_printf("�����Ѿܾ���δ��Ȩ��׼������\r\n");				//���������Ϣ
									connectFlag = 0;											//connectFlag���㣬��������						
									break;														//������֧case 0x05 		
						default   : u1_printf("�����Ѿܾ���δ֪״̬��׼������\r\n");		    //���������Ϣ 
									connectFlag = 0;											//connectFlag���㣬��������					
									break;														//������֧case default 								
					}				
			    }			
				//if�жϣ���һ���ֽ���0x90����ʾ�յ�����SUBACK����
				//��������Ҫ�ж϶��Ļظ��������ǲ��ǳɹ�
				else if(MQTT_RxDataOutPtr[2] == 0x90)
				{ 
					switch(MQTT_RxDataOutPtr[6])
					{					
						case 0x00 :
						case 0x01 : 
									u1_printf("���ĳɹ�\r\n");			//���������Ϣ
									subcribePackFlag = 1;				//subcribePackFlag��1����ʾ���ı��ĳɹ����������Ŀɷ���
									pingFlag = 0;						//pingFlag����
									TIM3_ENABLE_30S();					//����30s��PING��ʱ��	
									Send_Data();
									TIM5_Init(10000,36000); //��ʱ15s,����һ����ֵ
									TIM2_ENABLE_5S();
									break;								//������֧                                             
						default: 
									u1_printf("����ʧ�ܣ�׼������\r\n");//���������Ϣ 
									connectFlag = 0;					//connectFlag���㣬��������
									break;								//������֧ 								
						}					
			    }
				//if�жϣ���һ���ֽ���0xD0����ʾ�յ�����PINGRESP����
				else if(MQTT_RxDataOutPtr[2] == 0xD0)
				{ 
					u1_printf("PING���Ļظ�\r\n");                      //���������Ϣ 
					if(pingFlag == 1)
					{                   						        //���pingFlag=1����ʾ��һ�η���
						 pingFlag = 0;    				       			//Ҫ���pingFlag��־
					}
					else if(pingFlag > 1)	
					{ 				 									//���pingFlag>1����ʾ�Ƕ�η����ˣ�������2s����Ŀ��ٷ���
						pingFlag = 0;     				      			//Ҫ���pingFlag��־
						TIM3_ENABLE_30S(); 				      			//PING��ʱ���ػ�30s��ʱ��
					}				
				}	
				//if�жϣ������һ���ֽ���0x30����ʾ�յ����Ƿ�������������������
				//����Ҫ��ȡ��������
				else if(MQTT_RxDataOutPtr[2] == 0x30)
			    { 
					u1_printf("�������ȼ�0����\r\n"); 		   	//���������Ϣ 
					MQTT_DealPushdata_Qs0(MQTT_RxDataOutPtr);   //����ȼ�0��������
				}									
				MQTT_RxDataOutPtr += RBUFF_UNIT;                //ָ������
				if(MQTT_RxDataOutPtr == MQTT_RxDataEndPtr)      //���ָ�뵽������β����
				{
					MQTT_RxDataOutPtr = MQTT_RxDataBuf[0];      //ָ���λ����������ͷ              
				}		          
			}//������ջ��������ݵ�else if��֧��β
						
			/*---------------------------------------------------------------------------------------------------------------------*/
			/*                    							 ���������������                   		          			         */
			/*---------------------------------------------------------------------------------------------------------------------*/
			
			if(MQTT_CMDOutPtr != MQTT_CMDInPtr)							  //if�����Ļ���˵�����������������	
			{  				
				char *temp;
				u1_printf("����:%s\r\n",&MQTT_CMDOutPtr[2]);              //���������Ϣ
				
				
			 if(!memcmp(&MQTT_CMDOutPtr[2],cmdAuto_On,strlen(cmdAuto_On)))
				{
					Auto_ctro=1;//�Զ����Ʊ�־λ��1
					
					u1_printf("�Զ����ƿ�!\r\n");
					
				}
				else if(!memcmp(&MQTT_CMDOutPtr[2],cmdAuto_Off,strlen(cmdAuto_Off)))//������Զ����ƹ�����
				{    
					Auto_ctro = 0;                 //�Զ����Ʊ�־λ��0
					u1_printf("�Զ����ƹ�!\r\n");
					Flash_fa=0;
				}
				else if(Auto_ctro != 0)//�����Զ�����
				{
				 if(!memcmp(&MQTT_CMDOutPtr[2],TemFA, 6))
					{
					// �����и�õ���Ӧ��ֵ
						strtok((char *)&MQTT_CMDOutPtr[2], "S");
						temp = strtok(NULL, "S");					// eg:�õ�xx*
						tem_fa = (u8)atoi(strtok(temp, "*"));			// �õ��¶���ֵ	  
						u1_printf("�¶���ֵ��%d\r\n",tem_fa);
						Flash_fa = 1;//����Flash��־λ
					}
					
					else if(!memcmp(&MQTT_CMDOutPtr[2],HumFA, 6))//ʪ����ֵ����
					{     
						// �����и�õ���Ӧ��ֵ
						strtok((char *)&MQTT_CMDOutPtr[2], "S");
						temp = strtok(NULL, "S");					// eg:��?��?30.5*
						hum_fa = (u8)atoi(strtok(temp, "*"));	// �õ�ʪ����ֵ	
						u1_printf("ʪ�ȶ���ֵ��%d\r\n",hum_fa);
						Flash_fa = 1;//����Flash��־λ//����Flash��־λ
					}
					else if(!memcmp(&MQTT_CMDOutPtr[2],LightFA, strlen(LightFA)))//��ֵ����
					{     
						// �����и�õ���Ӧ��ֵ
						strtok((char *)&MQTT_CMDOutPtr[2], "S");
						temp = strtok(NULL, "S");					// eg:��?��?30.5*
						light_fa = (u8)atoi(strtok(temp, "*"));	// �õ����ն���ֵ	
						u1_printf("���ն���ֵ��%d\r\n",light_fa);
						Flash_fa = 1;//����Flash��־λ
					}
					
					else if(!memcmp(&MQTT_CMDOutPtr[2],SoilFA, strlen(SoilFA)))//��ֵ����
					{     
						// �����и�õ���Ӧ��ֵ
						strtok((char *)&MQTT_CMDOutPtr[2], "S");
						temp = strtok(NULL, "S");					// eg:��?��?30.5*
						soil_fa = (u8)atoi(strtok(temp, "*"));	// �õ�����ʪ����ֵ	
						u1_printf("����ʪ����ֵ��%d\r\n",soil_fa);
						Flash_fa = 1;//����Flash��־λ
					}
					
				//	Flash_fa = 1;//����Flash��־λ
				}
				else if(Auto_ctro != 1)				// �ֶ�ģʽ�������ֶ�״̬����
				{
						
						if(!memcmp(&MQTT_CMDOutPtr[2],cmdLED_On,strlen(cmdLED_On)))//�ж�ָ������CMD1 
					{                                            
						ledFlag = "LEDON";  								             
						LED_On();									  		   //LED����
						u1_printf("LED����\r\n");
					}
					else if(!memcmp(&MQTT_CMDOutPtr[2],cmdLED_Off,strlen(cmdLED_Off))) //�ж�ָ������CMD11
					{ 
						ledFlag = "LEDOFF";                                              
						LED_Off(); 								                //LED�ر�
						u1_printf("LED�ر�\r\n");
					}
					else if(!memcmp(&MQTT_CMDOutPtr[2],cmdBEEP_On,strlen(cmdBEEP_On))) //�ж�ָ������CMD11
					{ 
						beepFlag = "BEEPON";                                           
						BEEP_On(); 								                //LED�ر�
						u1_printf("����������\r\n");
					}
					else if(!memcmp(&MQTT_CMDOutPtr[2],cmdBEEP_Off,strlen(cmdBEEP_Off))) //�ж�ָ������CMD11
					{ 
						beepFlag = "BEEPOFF";                                           
						BEEP_Off();
						u1_printf("�������ر�\r\n");
						
					}
					else if(!memcmp(&MQTT_CMDOutPtr[2],cmdRelay_On,strlen(cmdRelay_On))) //�ж�ָ������CMD11
					{ 
						relayFlag = "RELAYON";                                           
						Relay_On(); 								                //LED�ر�
						Air_flag=1;
						u1_printf("�̵�������\r\n");
					}
					else if(!memcmp(&MQTT_CMDOutPtr[2],cmdRelay_Off,strlen(cmdRelay_Off))) //�ж�ָ������CMD11
					{ 
						relayFlag = "RELAYOFF";                                           
						Relay_Off();
						Air_flag=0;
						u1_printf("�̵����ر�\r\n");
						
					}
					
					else if(!memcmp(&MQTT_CMDOutPtr[2],cmdRelay2_On,strlen(cmdRelay2_On))) //�ж�ָ������CMD11
					{ 
//						relayFlag = "RELAYON";                                           
						Relay2_On(); 								                //LED�ر�
//						Air_flag=1;
						u1_printf("�̵���2����\r\n");
					}
					else if(!memcmp(&MQTT_CMDOutPtr[2],cmdRelay2_Off,strlen(cmdRelay2_Off))) //�ж�ָ������CMD11
					{ 
//					relayFlag = "RELAYOFF";                                           
						Relay2_Off();
//						Air_flag=0;
						u1_printf("�̵���2�ر�\r\n");
						
					}
				}
				
				
				
				//������������ᷢ�Ϳ�����״̬
				else 
					{
					 u1_printf("δָ֪��\r\n");				//���������Ϣ	
					
//					 u3_TxData(MQTT_CMDOutPtr);       //����3����������������ݣ�MQTT_CMDOutPtr��ǰ����Ԫ��Ϊ����ȡ�
					}
				
//				else if(!memcmp(&MQTT_CMDOutPtr[2],cmdFA,strlen((char *)&MQTT_CMDOutPtr[2])-2))//��ֵ
			
//				else if(!memcmp(&MQTT_CMDOutPtr[2],cmdDHT11_On,strlen(cmdDHT11_On)))//�ж�ָ������CMD3
//				{      
//					dhtFlag = 1;                 //dataFlag��1����ʾ���ڲɼ�״̬��
//					TIM2_ENABLE_10S();           //��ʱ��2��,10s����ɼ���ʪ��   
//				}
//				else if(!memcmp(&MQTT_CMDOutPtr[2],cmdDHT11_Off,strlen(cmdDHT11_Off)))//�ж�ָ������CMD3
//				{     
//					dhtFlag = 0;                 //dataFlag��0����ʾ����ֹͣ״̬��
//					TIM_Cmd(TIM2,DISABLE);		 //�ж�2·����״̬�Ͳɼ�״̬����������������
//				}								
				
			
				MQTT_CMDOutPtr += CBUFF_UNIT;				//ָ������
				if(MQTT_CMDOutPtr == MQTT_CMDEndPtr)	    //���ָ�뵽������β����
				MQTT_CMDOutPtr = MQTT_CMDBuf[0];			//ָ���λ����������ͷ	
				
				Send_Data();//���Ϳ����豸��״̬����
				//��������������ݵ�else if��֧��β		
//				temp=NULL;			//��temp�ÿ�	
			}	//connectFlag=1��if��֧�Ľ�β
			
		}
	/*--------------------------------------------------------------------*/
	/*      connectFlag=0ͬ�������Ͽ�������,Ҫ�������ӷ�����            */
	/*--------------------------------------------------------------------*/
		else
		{ 
			u1_printf("��Ҫ���ӷ�����\r\n");               //���������Ϣ
			TIM_Cmd(TIM4, DISABLE);                        //�ر�TIM4 
			TIM_Cmd(TIM3, DISABLE);                        //�ر�TIM3  
			WiFi_RxCounter = 0;                            //WiFi������������������                        
			memset(WiFi_RX_BUF, 0, WiFi_RXBUFF_SIZE);      //���WiFi���ջ����� 
			
			/**WiFi_Connect_IoTServer()����ESP8266�������ȵ�**/
			if(WiFi_Connect_IoTServer() == 0)			   //���WiFi�����Ʒ�������������0����ʾ��ȷ������if
			{   			     
				u1_printf("����TCP���ӳɹ�\r\n");          //���������Ϣ
				connectFlag = 1;                           //connectFlag��1����ʾ���ӳɹ�	
				OLED_Clear();
				oled_dispaly();//��ʾ�̶��ַ�
				//OLED_ShowString(0,2,"connect successful!",16);
				WiFi_RxCounter = 0;                        //WiFi������������������                        
				memset(WiFi_RX_BUF, 0, WiFi_RXBUFF_SIZE);  //���WiFi���ջ����� 
				MQTT_Buff_Init();                          //��ʼ�����ͻ�����                    
			}				
		}
	}
}

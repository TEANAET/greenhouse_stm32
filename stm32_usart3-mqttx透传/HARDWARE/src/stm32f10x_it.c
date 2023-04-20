/*-------------------------------------------------*/
/*                                                 */
/*            	   �жϷ�����          	   	   */
/*                                                 */
/*-------------------------------------------------*/

#include "stm32f10x.h"     //������Ҫ��ͷ�ļ�
#include "stm32f10x_it.h"  //������Ҫ��ͷ�ļ�
#include "usart1.h"        //������Ҫ��ͷ�ļ�
#include "usart2.h"        //������Ҫ��ͷ�ļ�
#include "usart3.h"        //������Ҫ��ͷ�ļ�
#include "timer3.h"        //������Ҫ��ͷ�ļ�
#include "mqtt.h"          //������Ҫ��ͷ�ļ�
#include "dht11.h"         //������Ҫ��ͷ�ļ�                  
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
//�ⲿ����
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
/*������������2�����жϺ�����������ȼ�������������ݣ�*/
/*��  ������                                       */
/*����ֵ����                                       */
/*-------------------------------------------------*/
void USART2_IRQHandler(void)   
{                      
	if(USART_GetITStatus(USART2, USART_IT_RXNE) != RESET)    //���USART_IT_RXNE��־��λ����ʾ�����ݵ��ˣ�����if��֧
	{  
		if(connectFlag == 0) 							     //���connectFlag����0����ǰ��û�����ӷ�����������ָ������״̬
		{                                     
			if(USART2->DR)
			{                                     			 //����ָ������״̬ʱ������ֵ�ű��浽������	
				Usart2_RxBuff[Usart2_RxCounter] = USART2->DR;//���浽������	
				Usart2_RxCounter++; 						 //ÿ����1���ֽڵ����ݣ�Usart2_RxCounter��1����ʾ���յ���������+1 
			}					
		}
		else
		{		                                           	 //��֮connectFlag����1�������Ϸ�������	
			Usart2_RxBuff[Usart2_RxCounter] = USART2->DR;    //�ѽ��յ������ݱ��浽Usart2_RxBuff��				
			if(Usart2_RxCounter == 0)
			{    									    											 //���Usart2_RxCounter����0����ʾ�ǽ��յĵ�1�����ݣ�����if��֧				
				TIM_Cmd(TIM4, ENABLE); 
			}
			else																						 //else��֧����ʾ��Usart2_RxCounter������0�����ǽ��յĵ�һ������
			{                        									    
				TIM_SetCounter(TIM4, 0);               //TIM4����������
			}	
			Usart2_RxCounter++;         				     //ÿ����1���ֽڵ����ݣ�Usart2_RxCounter��1����ʾ���յ���������+1 
		}
	}
} 

 /*-------------------------------------------------*/
/*����������ʱ��6�жϷ�������OLED��ʾ          */
/*��  ������                                       */
/*����ֵ����                                       */
/*-------------------------------------------------*/
void TIM6_IRQHandler(void)
{
	if(TIM_GetITStatus(TIM6, TIM_IT_Update) != RESET)//���TIM_IT_Update��λ����ʾTIM6����жϣ�����if	
	{
		
		switch(page_flag%4)
			{
			case 0:
			{
			page_flag++;
			if(flush_flag == 0){//��һ����ʾ
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
		flush_flag=0;//ˢ����־λ��λ
			}
		break;
			case 1:
			{
				page_flag++;
			if(flush_flag == 0){//��һ����ʾ
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
		 flush_flag=0;//ˢ����־λ��λ
	}break;
			case 2:{
				page_flag++;
			if(flush_flag == 0){//��һ����ʾ
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
			 flush_flag=0;//ˢ����־λ��λ
			}break;
			case 3:
			{
				page_flag++;
			if(flush_flag == 0){//��һ����ʾ
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
		 flush_flag=0;//ˢ����־λ��λ
			}
			break;
	 }
			
			TIM_ClearITPendingBit(TIM6, TIM_IT_Update); //���TIM6����жϱ�־
			}
			
			
	
//	
}
/*-------------------------------------------------*/
/*����������ʱ��4�жϷ�����������MQTT����          */
/*��  ������                                       */
/*����ֵ����                                       */
/*-------------------------------------------------*/
void TIM4_IRQHandler(void)
{
	if(TIM_GetITStatus(TIM4, TIM_IT_Update) != RESET)//���TIM_IT_Update��λ����ʾTIM4����жϣ�����if	
	{                	
		memcpy(&MQTT_RxDataInPtr[2], Usart2_RxBuff, Usart2_RxCounter);  //�������ݵ����ջ�����
		MQTT_RxDataInPtr[0] = Usart2_RxCounter/256;                  	//��¼���ݳ��ȸ��ֽ�
		MQTT_RxDataInPtr[1] = Usart2_RxCounter%256;	     				 	//��¼���ݳ��ȵ��ֽ�
		MQTT_RxDataInPtr += RBUFF_UNIT;                                	//ָ������
		if(MQTT_RxDataInPtr == MQTT_RxDataEndPtr)                     	//���ָ�뵽������β����
		MQTT_RxDataInPtr = MQTT_RxDataBuf[0];                    	//ָ���λ����������ͷ
		Usart2_RxCounter = 0;                                        	//����2������������������
		TIM_SetCounter(TIM3, 0);                                     	//���㶨ʱ��3�����������¼�ʱping������ʱ��
		TIM_Cmd(TIM4, DISABLE);                        				 	//�ر�TIM4��ʱ��
		TIM_SetCounter(TIM4, 0);                        			 	//���㶨ʱ��4������
		TIM_ClearITPendingBit(TIM4, TIM_IT_Update);     			 	//���TIM4����жϱ�־ 	
	}
}

///*-------------------------------------------------*/
///*����������ʱ��5�жϷ�����������USART3�����ݣ�����mqtt�ķ��ͻ����� */
///*��  ������                                       */
///*����ֵ����                                       */
///*-------------------------------------------------*/
//void TIM5_IRQHandler(void)
//{ char  tempAll[100]; 
//  int   tempLen;	
//	if(TIM_GetITStatus(TIM5, TIM_IT_Update) != RESET)//���TIM_IT_Update��λ����ʾTIM5����жϣ�����if	
//	{    
//		/*********usart3���ܵ�ֻ�Ǳ��ĵ�payload����װ�ɷ��ͱ��Ĳſ��Է��뷢�ͻ�����*******************/		
//		memcpy(tempAll, Usart3_RxBuff, Usart3_RxCounter);  //�������ݵ����ջ�����
//	  tempLen = Usart3_RxCounter;
//		
//		MQTT_PublishQs0(Data_TOPIC_NAME, tempAll, tempLen); //ʹ��tempAll������Ϊpayload��װ����
//				
//		Usart3_RxCounter = 0;                                        	//����3������������������
//		TIM_Cmd(TIM5, DISABLE);                        				 	//�ر�TIM5��ʱ��
//		TIM_SetCounter(TIM5, 0);                        			 	//���㶨ʱ��5������
//		TIM_ClearITPendingBit(TIM5, TIM_IT_Update);     			 	//���TIM5����жϱ�־ 	
//	}
//}

void TIM5_IRQHandler(void)
{
//	char  tempAll[100]; 
//  int   tempLen;	
	if(TIM_GetFlagStatus(TIM5, TIM_IT_Update) != RESET)//���TIM_IT_Update��λ����ʾTIM5����жϣ�����if	
	{    
		if(Flash_fa != 0 && Auto_ctro !=0)
		{
			// ����������ֵ����
//			sprintf((char *)Maxbuf, "%d:%d:%d:%d",tem_fa,hum_fa,light_fa,soil_fa);		// ÿ3�뱣��һ��
			sprintf((char *)Maxbuf, "%d:%d:%d:%d:%d:%d:%d:%d",tem_fa_min,tem_fa_max,hum_fa_min,hum_fa_max,
				light_fa_min,light_fa_max,soil_fa_min,soil_fa_max);
			u1_printf("write:%s\r\n", Maxbuf);
			STMFLASH_Write(FLASH_SAVE_ADDR,(u16*)Maxbuf,sizeof(Maxbuf));//д��flash
		}
		Flash_fa = 0;
	//	TIM_Cmd(TIM5, DISABLE);                        				 	//�ر�TIM5��ʱ��
		//TIM_SetCounter(TIM5, 0);                        			 	//���㶨ʱ��5������
		
	}
		TIM_ClearITPendingBit(TIM5, TIM_IT_Update);
}
/*-------------------------------------------------*/
/*����������ʱ��3�жϷ�����                      */
/*��  ������                                       */
/*����ֵ����                                       */
/*-------------------------------------------------*/
void TIM3_IRQHandler(void)
{
	if(TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET)//���TIM_IT_Update��λ����ʾTIM3����жϣ�����if	
		{  
		switch(pingFlag) 					//�ж�pingFlag��״̬
		{                               
			case 0:							//���pingFlag����0����ʾ����״̬������Ping����  
					MQTT_PingREQ(); 		//���Ping���ĵ����ͻ�����  
					break;
			case 1:							//���pingFlag����1��˵����һ�η��͵���ping���ģ�û���յ��������ظ�������1û�б����Ϊ0�������������쳣������Ҫ��������pingģʽ
					TIM3_ENABLE_2S(); 	    //���ǽ���ʱ��3����Ϊ2s��ʱ,���ٷ���Ping����
					MQTT_PingREQ();			//���Ping���ĵ����ͻ�����  
					break;
			case 2:							//���pingFlag����2��˵����û���յ��������ظ�
			case 3:				            //���pingFlag����3��˵����û���յ��������ظ�
			case 4:				            //���pingFlag����4��˵����û���յ��������ظ�	
					MQTT_PingREQ();  		//���Ping���ĵ����ͻ����� 
					break;
			case 5:							//���pingFlag����5��˵�����Ƿ����˶��ping�����޻ظ���Ӧ�������������⣬������������
					connectFlag = 0;        //����״̬��0����ʾ�Ͽ���û���Ϸ�����
					TIM_Cmd(TIM3, DISABLE); //��TIM3 				
					break;			
		}
		pingFlag++;           		   		//pingFlag����1����ʾ�ַ�����һ��ping���ڴ��������Ļظ�
		TIM_ClearITPendingBit(TIM3, TIM_IT_Update); //���TIM3����жϱ�־ 	
	}
}

/*-------------------------------------------------*/
/*����������ʱ��2�жϷ�����                      */
/*��  ������                                       */
/*����ֵ����                                       */
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
	char temp[50];				//����һ����ʱ������1,��������ͷ
	char tempAll[256];			//����һ����ʱ������2��������������
	
	int	dataLen = 0;			//���ĳ���

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
		memset(temp,    0, 50);				    //��ջ�����1
		memset(tempAll, 0, 256);				//��ջ�����2
		memset(head1,   0, 3);					//���MQTTͷ
		 
		u1_printf("ת��ǰ�¶�:%s\r\n",tem_buff);
		u1_printf("ת��ǰʪ��:%s\r\n",hum_buff);
		
		
		Hum_val = Myatoi(hum_buff);
		Tem_val = Myatoi(tem_buff);
		u1_printf("ת�����¶�:%d\r\n",Tem_val);
		u1_printf("ת����ʪ��:%d\r\n",Hum_val);
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
			//-------------�Զ�����-------------
			if(Hum_val<=hum_fa_min)
			{
				beepFlag = "BEEPON";//��ˮ��
				BEEP_On();
			}
			if(Hum_val>=hum_fa_max)
			{
				
				relayFlag = "RELAYON";//�رճ�ʪ��
				Relay_On();
	
			}
			if(Hum_val>hum_fa_min && Hum_val<hum_fa_max)
			{
				beepFlag = "BEEPOFF";//�ر�ˮ��
				BEEP_Off();
				//��ʪ
				relayFlag = "RELAYOFF";//�򿪳�ʪ��
				Relay_Off();
//				beepFlag = "BEEPOFF";//�ر�ˮ��
//				BEEP_Off();
			}
			if(Tem_val <= tem_fa_min)
			{
				ledFlag = "LEDON";//����(�򿪹�Դ)
				LED_On();
			}	
			else if(Tem_val >= tem_fa_max)
			{
//				relay2Flag = "RELAY2ON";//(ͨ���)����
//				Relay2_On();
				fansFlag = "FANSON";//(ͨ���)����
				Fans_On();
				
			}
			else
				{
					ledFlag = "LEDOFF";//(�رչ�Դ)
					LED_Off();
//					relay2Flag = "RELAY2OFF";//(ͨ���)�ر�
//					Relay2_Off();
				fansFlag = "FANSOFF";//(ͨ���)�ر�
				Fans_Off();
				}
			if(Light_val <= light_fa_min)
			{
				led2Flag = "LED2ON";//����(�򿪹�Դ)
				LED2_On();
				
			}
			else	if(Light_val >= light_fa_max)
			{
//				step_flag = 0;
//				step_flag++;
				if(step_flag == 0 && step_on_flag == 0){
					STEP_MOTOR_LOOP(1,3,1);  //���������ת ����
					step_flag++;
					step_on_flag = 1;
				}
				
//				ledFlag = "LEDOFF";//����(�򿪹�Դ)
//				LED_Off();
//				relay2Flag = "RELAY2ON";//����
//				Relay2_On();
				
			}
			else
			{
				led2Flag = "LED2OFF";//(�رչ�Դ)
				LED2_Off();
//				step_flag = 0;
//				step_flag++;
				if(step_flag == 1 && step_on_flag == 1)
				{
					STEP_MOTOR_LOOP(0,3,1);  //���������ת ����
					step_flag--;
					step_on_flag = 0;
				}
				
//				relay2Flag = "RELAY2OFF";//�ر�
//				Relay2_Off();
			}
			if(Soil_val <= soil_fa_min)
			{
				relay2Flag = "RELAY2ON";//��ˮ��
				Relay2_On();
//				Syn_flag=1;
			}
				if(Soil_val >= soil_fa_max)
			{
				relay2Flag = "RELAY2OFF";//�ر�ˮ��
				Relay2_Off();
//				Syn_flag=1;
			}
//			if(Syn_flag ==1){
				Send_Status();//���Ϳ����豸��״̬����
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
		
	if(Tem_val<0) //����¶�Ϊ��
		{
//			sprintf(temp,"{\"Tem\":\"-%d\",\"Hum\":\"%d\"}",Tem_val,Hum_val);//��������
//			if(connectFlag == 1)
//				MQTT_PublishQs0(Data_TOPIC_NAME,temp, strlen(temp));
		}
		
		if(Hum_val<=100)
			{
			sprintf(temp,"{\"Solar\":{\"Light\":\"%d\"},\"DHT11\":{\"Tem\":\"%d\",\"Hum\":\"%d\"},\"GY16\":{\"Soil\":\"%d\"}}",Light_val,Tem_val,Hum_val,Soil_val);//��������
			T_json(1,0,temp,tempAll);
			//sprintf(temp,"{\"Tem\":\"%d\",\"Hum\":\"%d\"}",Tem_val,Hum_val);
			if(connectFlag == 1)
				MQTT_PublishQs0(Data_TOPIC_NAME,tempAll, strlen(tempAll));
				
			}
	}
	else  //����¶�Ϊ��
		{
			if(Hum_val<=100)
			{
			sprintf(temp,"{\"Solar\":{\"Light\":\"%d\"},\"DHT11\":{\"Tem\":\"%d\",\"Hum\":\"%d\"},\"GY16\":{\"Soil\":\"%d\"}}",Light_val,Tem_val,Hum_val,Soil_val);//��������
			T_json(1,0,temp,tempAll);
			//sprintf(temp,"{\"Tem\":\"%d\",\"Hum\":\"%d\"}",Tem_val,Hum_val);
			if(connectFlag == 1)
				MQTT_PublishQs0(Data_TOPIC_NAME,tempAll, strlen(tempAll));
				
			}
//			u1_printf("����ʪ��Ϊ��%d\r\n",Soil_val);
			//sprintf(temp,"{\"Light\":\"%d\",\"Temp\":\"%d\",\"Hum\":\"%d\",\"Soil\":\"%d\"}",Light_val,Tem_val,Hum_val,Soil_val);//��������\
			
			
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
		//�öδ�������������OneNet��ƽ̨���ϴ������õģ���mqtt���ĵ�payload���֣�
		//����OneNet��Э��������޸ġ�ֱ��EMQX����Ҫ�������������롣
		head1[0] = 0x03; 						//�̶���ͷ
		head1[1] = 0x00; 						//�̶���ͷ
		head1[2] = strlen(temp);  				//ʣ�೤��	
		sprintf(tempAll, "%c%c%c%s", head1[0], head1[1], head1[2], temp);
			******/		
	
//		sprintf(tempAll, "%s", temp); //��������ע�Ͳ��֣�ֱ��EMQX��  
	
		
//		u1_printf("\r\n"); //������ʾ�������
////		u1_printf("haha%s\r\n",tem_buff);
////		u1_printf("xxx%s\r\n",hum_buff);
//		u1_printf("%s\r\n", tempAll);//����OneNet��ƽ̨ tempAll+3
//		
//		/******	dataLen = strlen(temp) + 3;  //��������OneNet��ƽ̨	******/
//		dataLen = strlen(temp);  //��������ע�Ͳ��֣�ֱ��EMQX��
		
//		MQTT_PublishQs0(Data_TOPIC_NAME,tempAll, dataLen);//������ݣ�������������

		TIM_ClearITPendingBit(TIM2, TIM_IT_Update);   	
	}
	
}

/*-------------------------------------------------*/
/*�����������������жϴ�����                     */
/*��  ������                                       */
/*����ֵ����                                       */
/*-------------------------------------------------*/
void NMI_Handler(void)
{

}

/*-------------------------------------------------*/
/*��������Ӳ������������жϴ�����             */
/*��  ������                                       */
/*����ֵ����                                       */
/*-------------------------------------------------*/
void HardFault_Handler(void)
{

}

/*-------------------------------------------------*/
/*���������ڴ�����жϴ�����                     */
/*��  ������                                       */
/*����ֵ����                                       */
/*-------------------------------------------------*/
void MemManage_Handler(void)
{

}

/*-------------------------------------------------*/
/*��������Ԥȡָʧ�ܣ��洢������ʧ���жϴ�����   */
/*��  ������                                       */
/*����ֵ����                                       */
/*-------------------------------------------------*/
void BusFault_Handler(void)
{

}

/*-------------------------------------------------*/
/*��������δ�����ָ���Ƿ�״̬������           */
/*��  ������                                       */
/*����ֵ����                                       */
/*-------------------------------------------------*/
void UsageFault_Handler(void)
{

}

/*-------------------------------------------------*/
/*�����������жϣ�SWI ָ����õĴ�����           */
/*��  ������                                       */
/*����ֵ����                                       */
/*-------------------------------------------------*/
void SVC_Handler(void)
{
	
}

/*-------------------------------------------------*/
/*�����������Լ����������                       */
/*��  ������                                       */
/*����ֵ����                                       */
/*-------------------------------------------------*/
void DebugMon_Handler(void)
{
	
}

/*-------------------------------------------------*/
/*���������ɹ����ϵͳ��������                 */
/*��  ������                                       */
/*����ֵ����                                       */
/*-------------------------------------------------*/
void PendSV_Handler(void)
{
}

/*-------------------------------------------------*/
/*��������SysTicϵͳ��શ�ʱ��������             */
/*��  ������                                       */
/*����ֵ����                                       */
/*-------------------------------------------------*/
void SysTick_Handler(void)
{
 
}

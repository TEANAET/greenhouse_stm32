/*-------------------------------------------------*/
/*                                                 */
/*                 ���ƶ˿�           		   	   */
/*                                                 */
/*-------------------------------------------------*/

//LED�������ݷ��͸�������

#include "stm32f10x.h"
#include "control.h"
#include "usart1.h"
#include "mqtt.h"
#include "json.h"

//extern char *ledFlag;               //�����״̬    
//extern char *beepFlag;
//extern int fa;
//extern u8 soil_fa,hum_fa,light_fa;
//extern s8 tem_fa;
extern s8 tem_fa_min,tem_fa_max;
extern u8 hum_fa_min,hum_fa_max,light_fa_min,light_fa_max,soil_fa_min,soil_fa_max;
extern char *ledFlag,*beepFlag,*relayFlag,*relay2Flag,*auto_flag,*led2Flag,*fansFlag;

//void convertUnCharToStr(char* str, unsigned char* UnChar, int ucLen)  
//{  
//    int i = 0;  
//    for(i = 0; i < ucLen; i++)  
//    {  
//        //��ʽ����str,ÿunsigned char ת���ַ�ռ��λ��%xд��%Xд��  
//        sprintf(str + i * 2, "%02x", UnChar[i]);  
//    }  
//}  


int Myatoi( char *str)
{
	int tmps=0;
	if(str==NULL)
		return 0;

	
	while(*str !='\0')
	{
		tmps=tmps*10+(*str -'0');//������ֲ������ӡ�
		str++;
	}

	return tmps;
}
/*-------------------------------------------------*/
/*�����������ݳ��� length ����                      */
/*��  ������                                       */
/*����ֵ������                 				       */
/*-------------------------------------------------*/
int length(int a)
{
	int i = 1;
	while(a/10 != 0)
	{
		i++;
		a = a/10;
	}
	return i;
}

/*-------------------------------------------------*/
/*�����������Ϳ����豸״̬����                          */
/*��  ������                                       */
/*����ֵ��							               */
/*-------------------------------------------------*/
void Send_Status(void)
{		
	char  head1[3];
	char  temp[256];          	//����һ����ʱ������1,��������ͷ
	char  tempAll[256];       	//����һ����ʱ������2��������������

	int   dataLen;     	  	    //���ĳ���	
	
	memset(temp,       0, 256);  //��ջ�����1
	memset(tempAll,    0, 256); //��ջ�����2
	memset(head1,      0, 3);   //���MQTTͷ                           						 

//	sprintf(temp, "{\"LED\":\"%s\",\"BEEP\":\"%s\",\"TEMFA\":\"%d\",\"HUMFA\":\"%d\",\"LIGHTFA\":\"%d\"}", ledFlag,beepFlag,tem_fa,hum_fa,light_fa);//��������,��LED��ΪOneNETƽ̨������������
	sprintf(temp,"{\"Status\":{\"LED\":\"%s\",\"LED2\":\"%s\",\"FAN\":\"%s\",\"BEEP\":\"%s\",\"RELAY\":\"%s\",\"RELAY2\":\"%s\",\"AUTO\":\"%s\"}}",ledFlag,led2Flag,fansFlag,beepFlag,relayFlag,relay2Flag,auto_flag);
	T_json(1,1,temp,tempAll);
	if(connectFlag == 1)
				MQTT_PublishQs0(Data_Status_Return,tempAll, strlen(tempAll));
}

void Send_Data(void)
{		
	char Tem_F[8],Hum_F[8],Soil_F[8],Solar_F[8];
	char  head1[3];
	char  temp[128];          	//����һ����ʱ������1,��������ͷ
	char  tempAll[256];       	//����һ����ʱ������2��������������

	int   dataLen;     	  	    //���ĳ���	
	
	memset(Tem_F,0,8);
	memset(Hum_F,0,8);
	memset(Soil_F,0,8);
	memset(Solar_F,0,8);
	memset(temp,       0, 128);  //��ջ�����1
	memset(tempAll,    0, 256); //��ջ�����2
	memset(head1,      0, 3);   //���MQTTͷ                           						 

//	sprintf(temp, "{\"LED\":\"%s\",\"BEEP\":\"%s\",\"TEMFA\":\"%d\",\"HUMFA\":\"%d\",\"LIGHTFA\":\"%d\"}", ledFlag,beepFlag,tem_fa,hum_fa,light_fa);//��������,��LED��ΪOneNETƽ̨������������
	sprintf(Tem_F,"%d:%d",tem_fa_min,tem_fa_max);
	sprintf(Hum_F,"%d:%d",hum_fa_min,hum_fa_max);
	sprintf(Soil_F,"%d:%d",soil_fa_min,soil_fa_max);
	sprintf(Solar_F,"%d:%d",light_fa_min,light_fa_max);
//	sprintf(temp,"{\"FAZHI\":{\"TEMFA\":\"%d\",\"HUMFA\":\"%d\",\"LIGHTFA\":\"%d\",\"SOILFA\":\"%d\"}}",tem_fa,hum_fa,light_fa,soil_fa);
  sprintf(temp,"{\"FAZHI\":{\"TEMFA\":\"%s\",\"HUMFA\":\"%s\",\"LIGHTFA\":\"%s\",\"SOILFA\":\"%s\"}}",Tem_F,Hum_F,Solar_F,Soil_F);
	T_json(1,2,temp,tempAll);
	if(connectFlag == 1)
				MQTT_PublishQs0(Data_Fa_Return,tempAll, strlen(tempAll));
}


	
	
	
	/****�˶α�����OneNet���ϴ����ݵĸ�ʽҪ��ʵ����ֻ��mqtt��payload******
	head1[0] = 0x03; //�̶���ͷ
	head1[1] = 0x00; //�̶���ͷ
	head1[2] = strlen(temp); //ʣ�೤��	
	sprintf(tempAll, "%c%c%c%s", head1[0], head1[1], head1[2], temp);
	
	dataLen = strlen(temp) + 3;
	u1_printf("%s\r\n", tempAll+3);
	  *******
	//u1_printf("�ܳ��ȣ�%d\r\n",Init_len);
	MQTT_PublishQs0(Data_TOPIC_NAME, tempAll, dataLen); //������ݣ�������������		
	 **********/
	 
	  /**ֱ��EMQX�Ĵ���**/
//	 dataLen = strlen(temp);
//	 MQTT_PublishQs0(Data_Status_Return, temp, dataLen); 
	  /**ֱ��EMQX�Ĵ���**/




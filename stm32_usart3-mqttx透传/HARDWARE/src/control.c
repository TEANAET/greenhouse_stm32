/*-------------------------------------------------*/
/*                                                 */
/*                 控制端口           		   	   */
/*                                                 */
/*-------------------------------------------------*/

//LED控制数据发送给服务器

#include "stm32f10x.h"
#include "control.h"
#include "usart1.h"
#include "mqtt.h"
#include "json.h"

//extern char *ledFlag;               //补光灯状态    
//extern char *beepFlag;
//extern int fa;
extern u8 soil_fa,hum_fa,light_fa;
extern s8 tem_fa;
extern char *ledFlag,*beepFlag,*relayFlag,*relay2Flag,*auto_flag;

//void convertUnCharToStr(char* str, unsigned char* UnChar, int ucLen)  
//{  
//    int i = 0;  
//    for(i = 0; i < ucLen; i++)  
//    {  
//        //格式化输str,每unsigned char 转换字符占两位置%x写输%X写输  
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
		tmps=tmps*10+(*str -'0');//获得数字并“叠加”
		str++;
	}

	return tmps;
}
/*-------------------------------------------------*/
/*函数名：数据长度 length 函数                      */
/*参  数：无                                       */
/*返回值：长度                 				       */
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
/*函数名：发送控制设备状态数据                          */
/*参  数：无                                       */
/*返回值：							               */
/*-------------------------------------------------*/
void Send_Status(void)
{		
	char  head1[3];
	char  temp[256];          	//定义一个临时缓冲区1,不包括报头
	char  tempAll[256];       	//定义一个临时缓冲区2，包括所有数据

	int   dataLen;     	  	    //报文长度	
	
	memset(temp,       0, 256);  //清空缓冲区1
	memset(tempAll,    0, 256); //清空缓冲区2
	memset(head1,      0, 3);   //清空MQTT头                           						 

//	sprintf(temp, "{\"LED\":\"%s\",\"BEEP\":\"%s\",\"TEMFA\":\"%d\",\"HUMFA\":\"%d\",\"LIGHTFA\":\"%d\"}", ledFlag,beepFlag,tem_fa,hum_fa,light_fa);//构建报文,“LED”为OneNET平台的数据流名称
	sprintf(temp,"{\"Status\":{\"LED\":\"%s\",\"BEEP\":\"%s\",\"RELAY\":\"%s\",\"RELAY2\":\"%s\",\"AUTO\":\"%s\"}}",ledFlag,beepFlag,relayFlag,relay2Flag,auto_flag);
	T_json(1,1,temp,tempAll);
	if(connectFlag == 1)
				MQTT_PublishQs0(Data_Status_Return,tempAll, strlen(tempAll));
}

void Send_Data(void)
{		
	char  head1[3];
	char  temp[128];          	//定义一个临时缓冲区1,不包括报头
	char  tempAll[256];       	//定义一个临时缓冲区2，包括所有数据

	int   dataLen;     	  	    //报文长度	
	
	memset(temp,       0, 128);  //清空缓冲区1
	memset(tempAll,    0, 256); //清空缓冲区2
	memset(head1,      0, 3);   //清空MQTT头                           						 

//	sprintf(temp, "{\"LED\":\"%s\",\"BEEP\":\"%s\",\"TEMFA\":\"%d\",\"HUMFA\":\"%d\",\"LIGHTFA\":\"%d\"}", ledFlag,beepFlag,tem_fa,hum_fa,light_fa);//构建报文,“LED”为OneNET平台的数据流名称
	sprintf(temp,"{\"FAZHI\":{\"TEMFA\":\"%d\",\"HUMFA\":\"%d\",\"LIGHTFA\":\"%d\",\"SOILFA\":\"%d\"}}",tem_fa,hum_fa,light_fa,soil_fa);
	T_json(1,2,temp,tempAll);
	if(connectFlag == 1)
				MQTT_PublishQs0(Data_Fa_Return,tempAll, strlen(tempAll));
}


	
	
	
	/****此段报文是OneNet对上传数据的格式要求，实际上只是mqtt的payload******
	head1[0] = 0x03; //固定报头
	head1[1] = 0x00; //固定报头
	head1[2] = strlen(temp); //剩余长度	
	sprintf(tempAll, "%c%c%c%s", head1[0], head1[1], head1[2], temp);
	
	dataLen = strlen(temp) + 3;
	u1_printf("%s\r\n", tempAll+3);
	  *******
	//u1_printf("总长度：%d\r\n",Init_len);
	MQTT_PublishQs0(Data_TOPIC_NAME, tempAll, dataLen); //添加数据，发布给服务器		
	 **********/
	 
	  /**直连EMQX的代码**/
//	 dataLen = strlen(temp);
//	 MQTT_PublishQs0(Data_Status_Return, temp, dataLen); 
	  /**直连EMQX的代码**/




#include "bsp_dht11.h"
#include "delay.h"

static void                           DHT11_GPIO_Config                       ( void );
static void                           DHT11_Mode_IPU                          ( void );
static void                           DHT11_Mode_Out_PP                       ( void );
static uint8_t                        DHT11_ReadByte                          ( void );


//void delay_us(u16 time)
//{    
//   u16 i=0;  
//   while(time--)
//   {
//      i=10;  //�Լ�����
//      while(i--) ;    
//   }
//}
////���뼶����ʱ
//void delay_ms(u16 time)
//{    
//   u16 i=0;  
//   while(time--)
//   {
//      i=12000;  //�Լ�����
//      while(i--) ;    
//   }
//}

 /**
  * @brief  DHT11 ��ʼ������
  * @param  ��
  * @retval ��
  */
//void DHT11_Init ( void )
//{
//	DHT11_GPIO_Config ();
//	DHT11_Dout_1; // ����GPIOB10
//}


///*
// * ��������DHT11_GPIO_Config
// * ����  ������DHT11�õ���I/O��
// * ����  ����
// * ���  ����
// */
//static void DHT11_GPIO_Config ( void )
//{
//	GPIO_InitTypeDef GPIO_InitStructure; 

//  DHT11_Dout_SCK_APBxClock_FUN ( DHT11_Dout_GPIO_CLK, ENABLE ); /*����DHT11_Dout_GPIO_PORT������ʱ��*/							   
//	GPIO_InitStructure.GPIO_Pin = DHT11_Dout_GPIO_PIN; /*ѡ��Ҫ���Ƶ�DHT11_Dout_GPIO_PORT����*/	
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; /*��������ģʽΪͨ���������*/
//	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; /*������������Ϊ50MHz */  
//	GPIO_Init ( DHT11_Dout_GPIO_PORT, &GPIO_InitStructure ); /*���ÿ⺯������ʼ��DHT11_Dout_GPIO_PORT*/
//}


///*
// * ��������DHT11_Mode_IPU
// * ����  ��ʹDHT11-DATA���ű�Ϊ��������ģʽ
// * ����  ����
// * ���  ����
// */
//static void DHT11_Mode_IPU(void)
//{
//	GPIO_InitTypeDef GPIO_InitStructure;

//	GPIO_InitStructure.GPIO_Pin = DHT11_Dout_GPIO_PIN;	  	/*ѡ��Ҫ���Ƶ�DHT11_Dout_GPIO_PORT����*/	
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU ; 	   /*��������ģʽΪ��������ģʽ*/ 
//	GPIO_Init(DHT11_Dout_GPIO_PORT, &GPIO_InitStructure);	 	  /*���ÿ⺯������ʼ��DHT11_Dout_GPIO_PORT*/
//}


///*
// * ��������DHT11_Mode_Out_PP
// * ����  ��ʹDHT11-DATA���ű�Ϊ�������ģʽ
// * ����  ����
// * ���  ����
// */
//static void DHT11_Mode_Out_PP(void)
//{
// 	GPIO_InitTypeDef GPIO_InitStructure;
//									   
//	GPIO_InitStructure.GPIO_Pin = DHT11_Dout_GPIO_PIN;		 	/*ѡ��Ҫ���Ƶ�DHT11_Dout_GPIO_PORT����*/		
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;   	/*��������ģʽΪͨ���������*/
//	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	/*������������Ϊ50MHz */   
//	GPIO_Init(DHT11_Dout_GPIO_PORT, &GPIO_InitStructure);	 	 
//	
//}


///* 
// * ��DHT11��ȡһ���ֽڣ�MSB����
// */
//static uint8_t DHT11_ReadByte ( void )
//{
//	uint8_t i, temp=0;
//	
//	for(i=0;i<8;i++)    
//	{
//		while(DHT11_Dout_IN()==Bit_RESET);		/*ÿbit��50us�͵�ƽ���ÿ�ʼ����ѯֱ���ӻ����� ��50us �͵�ƽ ����*/  
//		/*DHT11 ��26~28us�ĸߵ�ƽ��ʾ��0������70us�ߵ�ƽ��ʾ��1����
//		 *ͨ����� x us��ĵ�ƽ��������������״ ��x ���������ʱ 
//		 */
//		DelayUs(40); //��ʱx us �����ʱ��Ҫ��������0������ʱ�伴��	   	  
//		if(DHT11_Dout_IN()==Bit_SET)/* x us����Ϊ�ߵ�ƽ��ʾ���ݡ�1�� */
//		{
//			while(DHT11_Dout_IN()==Bit_SET); /* �ȴ�����1�ĸߵ�ƽ���� */
//			temp |= (uint8_t)(0x01<<(7-i)); //�ѵ�7-iλ��1��MSB���� 
//		}
//		else	 // x us��Ϊ�͵�ƽ��ʾ���ݡ�0��
//			temp&=(uint8_t)~(0x01<<(7-i)); //�ѵ�7-iλ��0��MSB����
//	}
//	return temp;
//}


///*
// * һ�����������ݴ���Ϊ40bit����λ�ȳ�
// * 8bit ʪ������ + 8bit ʪ��С�� + 8bit �¶����� + 8bit �¶�С�� + 8bit У��� 
// */
//uint8_t DHT11_Read_TempAndHumidity(DHT11_Data_TypeDef *DHT11_Data)
//{  

//	DHT11_Mode_Out_PP();	/*���ģʽ*/
//	DHT11_Dout_0;	/*��������*/
//	DelayMs(18);	/*��ʱ18ms*/

//	DHT11_Dout_1; 	/*�������� ������ʱ30us*/

//	DelayUs(30);   //��ʱ30us

//	DHT11_Mode_IPU();	/*������Ϊ���� �жϴӻ���Ӧ�ź�*/ 


//	if(DHT11_Dout_IN()==Bit_RESET) /*�жϴӻ��Ƿ��е͵�ƽ��Ӧ�ź� �粻��Ӧ����������Ӧ����������*/   
//	{

//		while(DHT11_Dout_IN()==Bit_RESET);		/*��ѯֱ���ӻ����� ��80us �͵�ƽ ��Ӧ�źŽ���*/  
//		while(DHT11_Dout_IN()==Bit_SET);		/*��ѯֱ���ӻ������� 80us �ߵ�ƽ �����źŽ���*/

//		DHT11_Data->humi_int= DHT11_ReadByte();
//		DHT11_Data->humi_deci= DHT11_ReadByte();
//		DHT11_Data->temp_int= DHT11_ReadByte();
//		DHT11_Data->temp_deci= DHT11_ReadByte();
//		DHT11_Data->check_sum= DHT11_ReadByte();

//		DHT11_Mode_Out_PP();		/*��ȡ���������Ÿ�Ϊ���ģʽ*/
//		DHT11_Dout_1;		/*��������*/

//		/*����ȡ�������Ƿ���ȷ*/
//		if(DHT11_Data->check_sum == DHT11_Data->humi_int + DHT11_Data->humi_deci + DHT11_Data->temp_int+ DHT11_Data->temp_deci)
//			return SUCCESS;
//		else 
//			return ERROR;
//	}
//	
//	else
//		return ERROR;
//	
//}

	  


/*************************************END OF FILE******************************/

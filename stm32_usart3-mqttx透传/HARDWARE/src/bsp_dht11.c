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
//      i=10;  //自己定义
//      while(i--) ;    
//   }
//}
////毫秒级的延时
//void delay_ms(u16 time)
//{    
//   u16 i=0;  
//   while(time--)
//   {
//      i=12000;  //自己定义
//      while(i--) ;    
//   }
//}

 /**
  * @brief  DHT11 初始化函数
  * @param  无
  * @retval 无
  */
//void DHT11_Init ( void )
//{
//	DHT11_GPIO_Config ();
//	DHT11_Dout_1; // 拉高GPIOB10
//}


///*
// * 函数名：DHT11_GPIO_Config
// * 描述  ：配置DHT11用到的I/O口
// * 输入  ：无
// * 输出  ：无
// */
//static void DHT11_GPIO_Config ( void )
//{
//	GPIO_InitTypeDef GPIO_InitStructure; 

//  DHT11_Dout_SCK_APBxClock_FUN ( DHT11_Dout_GPIO_CLK, ENABLE ); /*开启DHT11_Dout_GPIO_PORT的外设时钟*/							   
//	GPIO_InitStructure.GPIO_Pin = DHT11_Dout_GPIO_PIN; /*选择要控制的DHT11_Dout_GPIO_PORT引脚*/	
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; /*设置引脚模式为通用推挽输出*/
//	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; /*设置引脚速率为50MHz */  
//	GPIO_Init ( DHT11_Dout_GPIO_PORT, &GPIO_InitStructure ); /*调用库函数，初始化DHT11_Dout_GPIO_PORT*/
//}


///*
// * 函数名：DHT11_Mode_IPU
// * 描述  ：使DHT11-DATA引脚变为上拉输入模式
// * 输入  ：无
// * 输出  ：无
// */
//static void DHT11_Mode_IPU(void)
//{
//	GPIO_InitTypeDef GPIO_InitStructure;

//	GPIO_InitStructure.GPIO_Pin = DHT11_Dout_GPIO_PIN;	  	/*选择要控制的DHT11_Dout_GPIO_PORT引脚*/	
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU ; 	   /*设置引脚模式为浮空输入模式*/ 
//	GPIO_Init(DHT11_Dout_GPIO_PORT, &GPIO_InitStructure);	 	  /*调用库函数，初始化DHT11_Dout_GPIO_PORT*/
//}


///*
// * 函数名：DHT11_Mode_Out_PP
// * 描述  ：使DHT11-DATA引脚变为推挽输出模式
// * 输入  ：无
// * 输出  ：无
// */
//static void DHT11_Mode_Out_PP(void)
//{
// 	GPIO_InitTypeDef GPIO_InitStructure;
//									   
//	GPIO_InitStructure.GPIO_Pin = DHT11_Dout_GPIO_PIN;		 	/*选择要控制的DHT11_Dout_GPIO_PORT引脚*/		
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;   	/*设置引脚模式为通用推挽输出*/
//	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	/*设置引脚速率为50MHz */   
//	GPIO_Init(DHT11_Dout_GPIO_PORT, &GPIO_InitStructure);	 	 
//	
//}


///* 
// * 从DHT11读取一个字节，MSB先行
// */
//static uint8_t DHT11_ReadByte ( void )
//{
//	uint8_t i, temp=0;
//	
//	for(i=0;i<8;i++)    
//	{
//		while(DHT11_Dout_IN()==Bit_RESET);		/*每bit以50us低电平标置开始，轮询直到从机发出 的50us 低电平 结束*/  
//		/*DHT11 以26~28us的高电平表示“0”，以70us高电平表示“1”，
//		 *通过检测 x us后的电平即可区别这两个状 ，x 即下面的延时 
//		 */
//		DelayUs(40); //延时x us 这个延时需要大于数据0持续的时间即可	   	  
//		if(DHT11_Dout_IN()==Bit_SET)/* x us后仍为高电平表示数据“1” */
//		{
//			while(DHT11_Dout_IN()==Bit_SET); /* 等待数据1的高电平结束 */
//			temp |= (uint8_t)(0x01<<(7-i)); //把第7-i位置1，MSB先行 
//		}
//		else	 // x us后为低电平表示数据“0”
//			temp&=(uint8_t)~(0x01<<(7-i)); //把第7-i位置0，MSB先行
//	}
//	return temp;
//}


///*
// * 一次完整的数据传输为40bit，高位先出
// * 8bit 湿度整数 + 8bit 湿度小数 + 8bit 温度整数 + 8bit 温度小数 + 8bit 校验和 
// */
//uint8_t DHT11_Read_TempAndHumidity(DHT11_Data_TypeDef *DHT11_Data)
//{  

//	DHT11_Mode_Out_PP();	/*输出模式*/
//	DHT11_Dout_0;	/*主机拉低*/
//	DelayMs(18);	/*延时18ms*/

//	DHT11_Dout_1; 	/*总线拉高 主机延时30us*/

//	DelayUs(30);   //延时30us

//	DHT11_Mode_IPU();	/*主机设为输入 判断从机响应信号*/ 


//	if(DHT11_Dout_IN()==Bit_RESET) /*判断从机是否有低电平响应信号 如不响应则跳出，响应则向下运行*/   
//	{

//		while(DHT11_Dout_IN()==Bit_RESET);		/*轮询直到从机发出 的80us 低电平 响应信号结束*/  
//		while(DHT11_Dout_IN()==Bit_SET);		/*轮询直到从机发出的 80us 高电平 标置信号结束*/

//		DHT11_Data->humi_int= DHT11_ReadByte();
//		DHT11_Data->humi_deci= DHT11_ReadByte();
//		DHT11_Data->temp_int= DHT11_ReadByte();
//		DHT11_Data->temp_deci= DHT11_ReadByte();
//		DHT11_Data->check_sum= DHT11_ReadByte();

//		DHT11_Mode_Out_PP();		/*读取结束，引脚改为输出模式*/
//		DHT11_Dout_1;		/*主机拉高*/

//		/*检查读取的数据是否正确*/
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

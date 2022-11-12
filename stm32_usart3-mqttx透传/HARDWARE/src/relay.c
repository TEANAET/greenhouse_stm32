#include "Relay.h"
//初始化PB8为输出口.并使能这个口的时钟		    
//继电器初始化
void Relay_Init(void)
{
 
 GPIO_InitTypeDef  GPIO_InitStructure;
 	
 RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);	 //使能GPIOC端口时钟
 
 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;				 //BEEP-->PB.8 端口配置
 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //推挽输出
 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	 //速度为50MHz
 GPIO_Init(GPIOC, &GPIO_InitStructure);	 //根据参数初始化GPIOB.8
 
 GPIO_ResetBits(GPIOC,GPIO_Pin_3);//输出0，关闭继电器输出

}

//继电器2初始化
void Relay2_Init(void)
{
 
 GPIO_InitTypeDef  GPIO_InitStructure;
 	
 RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOG, ENABLE);	 //使能GPIOG端口时钟
 
 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;				 //BEEP-->PB.8 端口配置
 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD; 		 //开漏输出
 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	 //速度为50MHz
 GPIO_Init(GPIOG, &GPIO_InitStructure);	 //根据参数初始化GPIOB.8
 
 //GPIO_ResetBits(GPIOC,GPIO_Pin_3);//输出0，关闭继电器输出
	GPIO_SetBits(GPIOG,GPIO_Pin_4);//输出为1，关闭继电器

}

/*-------------------------------------------------*/
/*函数名：继电器开启                                  */
/*参  数：无                                       */
/*返回值：无                                       */
/*-------------------------------------------------*/
void Relay_Off(void)
{			
	GPIO_ResetBits(GPIOC, Relay_PIN); 						 //PC.0.1.2.3 输出低
} 


/*-------------------------------------------------*/
/*函数名：继电器关闭                                  */
/*参  数：无                                       */
/*返回值：无                                       */
/*-------------------------------------------------*/
void Relay_On(void)
{		
	GPIO_SetBits(GPIOC, Relay_PIN); 						 //PC.0.1.2.3 输出高
}

/*-------------------------------------------------*/
/*函数名：继电器2开启                                  */
/*参  数：无                                       */
/*返回值：无                                       */
/*-------------------------------------------------*/
void Relay2_Off(void)
{			
	GPIO_SetBits(GPIOG, GPIO_Pin_4); 						 //PC.0.1.2.3 输出低
} 


/*-------------------------------------------------*/
/*函数名：继电器2关闭                                  */
/*参  数：无                                       */
/*返回值：无                                       */
/*-------------------------------------------------*/
void Relay2_On(void)
{		
	GPIO_ResetBits(GPIOG, GPIO_Pin_4); 						 //PC.0.1.2.3 输出高
}

 
/*-------------------------------------------------*/
/*         	                                       */
/*           	 LED������ƣ�����           	    */
/*                                                 */
/*-------------------------------------------------*/

// Ӳ�����ӣ�
// PD2  LED0

#include "stm32f10x.h"  //������Ҫ��ͷ�ļ�
#include "led.h"        //������Ҫ��ͷ�ļ�

/*-------------------------------------------------*/
/*����������ʼ��LED����,PC0.1.2.3,���������� 	    */
/*��  ������                                       */
/*����ֵ����                                       */
/*-------------------------------------------------*/
void LED_Init(void)
{    	 
	GPIO_InitTypeDef GPIO_InitStructure;                     
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);    

	GPIO_InitStructure.GPIO_Pin = LED_PIN;   
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;     
	GPIO_Init(GPIOB, &GPIO_InitStructure); 
	GPIO_SetBits(GPIOB, LED_PIN); 						 //PC.0.1.2.3 ����� 
}

/*-------------------------------------------------*/
/*��������LED����                                  */
/*��  ������                                       */
/*����ֵ����                                       */
/*-------------------------------------------------*/
void LED_On(void)
{			
	GPIO_ResetBits(GPIOB, LED_PIN); 						 //PC.0.1.2.3 �����
} 


/*-------------------------------------------------*/
/*��������LED�ر�                                  */
/*��  ������                                       */
/*����ֵ����                                       */
/*-------------------------------------------------*/
void LED_Off(void)
{		
	GPIO_SetBits(GPIOB, LED_PIN); 						 //PC.0.1.2.3 �����
}




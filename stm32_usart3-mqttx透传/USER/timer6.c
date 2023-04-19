#include "stm32f10x.h"  //������Ҫ��ͷ�ļ�

char timer6_flag = 0;   //1:��ʱ��6�жϣ������ݵ�

/*-------------------------------------------------*/
/*����������ʱ��6��ʼ��                            */
/*��  ����arr���Զ���װֵ   0~65535                */
/*��  ����psc��ʱ��Ԥ��Ƶ�� 0~65535                */
/*����ֵ����                                       */
/*˵  ������ʱʱ�䣺arr*psc*1000/72000000  ��λms  */
/*-------------------------------------------------*/
void TIM6_Init(unsigned short int arr, unsigned short int psc)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;              //����һ�����ö�ʱ���ı���
	NVIC_InitTypeDef NVIC_InitStructure;                            //����һ�������жϵı���
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);                 //�����ж��������飺��2�� �������ȼ���0 1 2 3 �����ȼ���0 1 2 3		
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM6, ENABLE);            //ʹ��TIM6ʱ��	
  TIM_TimeBaseInitStructure.TIM_Period = arr; 	                //�����Զ���װ��ֵ
	TIM_TimeBaseInitStructure.TIM_Prescaler = psc;                  //���ö�ʱ��Ԥ��Ƶ��
	TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up; //���ϼ���ģʽ
	TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;     //1��Ƶ
	TIM_TimeBaseInit(TIM6, &TIM_TimeBaseInitStructure);             //����TIM6
	
	TIM_ClearITPendingBit(TIM6, TIM_IT_Update);                     //�������жϱ�־λ
	TIM_ITConfig(TIM6, TIM_IT_Update, ENABLE);                      //ʹ��TIM6����ж�    
	TIM_Cmd(TIM6, ENABLE);                                         //�ȹر�TIM6                         
	
	NVIC_InitStructure.NVIC_IRQChannel = TIM6_IRQn;                 //����TIM6�ж�
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;       //��ռ���ȼ�1
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;              //�����ȼ�0
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;                 //�ж�ͨ��ʹ��
	NVIC_Init(&NVIC_InitStructure);                                 //�����ж�
}
#include "Relay.h"
//��ʼ��PB8Ϊ�����.��ʹ������ڵ�ʱ��		    
//�̵�����ʼ��
void Relay_Init(void)
{
 
 GPIO_InitTypeDef  GPIO_InitStructure;
 	
 RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);	 //ʹ��GPIOC�˿�ʱ��
 
 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;				 //BEEP-->PB.8 �˿�����
 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //�������
 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	 //�ٶ�Ϊ50MHz
 GPIO_Init(GPIOC, &GPIO_InitStructure);	 //���ݲ�����ʼ��GPIOB.8
 
 GPIO_ResetBits(GPIOC,GPIO_Pin_3);//���0���رռ̵������

}

//�̵���2��ʼ��
void Relay2_Init(void)
{
 
 GPIO_InitTypeDef  GPIO_InitStructure;
 	
 RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOG, ENABLE);	 //ʹ��GPIOG�˿�ʱ��
 
 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;				 //BEEP-->PB.8 �˿�����
 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD; 		 //��©���
 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	 //�ٶ�Ϊ50MHz
 GPIO_Init(GPIOG, &GPIO_InitStructure);	 //���ݲ�����ʼ��GPIOB.8
 
 //GPIO_ResetBits(GPIOC,GPIO_Pin_3);//���0���رռ̵������
	GPIO_SetBits(GPIOG,GPIO_Pin_4);//���Ϊ1���رռ̵���

}

/*-------------------------------------------------*/
/*���������̵�������                                  */
/*��  ������                                       */
/*����ֵ����                                       */
/*-------------------------------------------------*/
void Relay_Off(void)
{			
	GPIO_ResetBits(GPIOC, Relay_PIN); 						 //PC.0.1.2.3 �����
} 


/*-------------------------------------------------*/
/*���������̵����ر�                                  */
/*��  ������                                       */
/*����ֵ����                                       */
/*-------------------------------------------------*/
void Relay_On(void)
{		
	GPIO_SetBits(GPIOC, Relay_PIN); 						 //PC.0.1.2.3 �����
}

/*-------------------------------------------------*/
/*���������̵���2����                                  */
/*��  ������                                       */
/*����ֵ����                                       */
/*-------------------------------------------------*/
void Relay2_Off(void)
{			
	GPIO_SetBits(GPIOG, GPIO_Pin_4); 						 //PC.0.1.2.3 �����
} 


/*-------------------------------------------------*/
/*���������̵���2�ر�                                  */
/*��  ������                                       */
/*����ֵ����                                       */
/*-------------------------------------------------*/
void Relay2_On(void)
{		
	GPIO_ResetBits(GPIOG, GPIO_Pin_4); 						 //PC.0.1.2.3 �����
}

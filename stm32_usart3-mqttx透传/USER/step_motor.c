#include "step_motor.h"
 
u8 STEP; 
 
 
void STEP_MOTOR_Init(void){ //�ӿڳ�ʼ��
	
	GPIO_InitTypeDef GPIO_InitStruct;
	RCC_APB2PeriphClockCmd(STEP_MOTOR_CLK, ENABLE);
 
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStruct.GPIO_Pin = STEP_MOTOR_A;
    GPIO_Init(STEP_MOTOR_PORT, &GPIO_InitStruct);
    GPIO_InitStruct.GPIO_Pin = STEP_MOTOR_B;
    GPIO_Init(STEP_MOTOR_PORT, &GPIO_InitStruct);
    GPIO_InitStruct.GPIO_Pin = STEP_MOTOR_C;
    GPIO_Init(STEP_MOTOR_PORT, &GPIO_InitStruct);
    GPIO_InitStruct.GPIO_Pin = STEP_MOTOR_D;
    GPIO_Init(STEP_MOTOR_PORT, &GPIO_InitStruct);
 
    GPIO_ResetBits(STEP_MOTOR_PORT, STEP_MOTOR_A);
    GPIO_ResetBits(STEP_MOTOR_PORT, STEP_MOTOR_B);
    GPIO_ResetBits(STEP_MOTOR_PORT, STEP_MOTOR_C);
    GPIO_ResetBits(STEP_MOTOR_PORT, STEP_MOTOR_D);	
	
}
 
void STEP_MOTOR_OFF (void)  //����ϵ� ȫ����
{
GPIO_ResetBits(STEP_MOTOR_PORT,STEP_MOTOR_A | STEP_MOTOR_B | STEP_MOTOR_C | STEP_MOTOR_D);//���ӿ���0
}
 
void STEP_MOTOR_8A (u8 a,u16 speed)  //�������8��
{
	switch (a){
		case 0:
		GPIO_ResetBits(STEP_MOTOR_PORT,STEP_MOTOR_B | STEP_MOTOR_C | STEP_MOTOR_D);//0
		GPIO_SetBits(STEP_MOTOR_PORT,STEP_MOTOR_A);//1
			break;
		case 1:
		GPIO_ResetBits(STEP_MOTOR_PORT,STEP_MOTOR_C | STEP_MOTOR_D);//0
		GPIO_SetBits(STEP_MOTOR_PORT,STEP_MOTOR_A | STEP_MOTOR_B);//1
			break;
		case 2:
		GPIO_ResetBits(STEP_MOTOR_PORT,STEP_MOTOR_A | STEP_MOTOR_C | STEP_MOTOR_D);//0
		GPIO_SetBits(STEP_MOTOR_PORT,STEP_MOTOR_B);//1
			break;
		case 3:
		GPIO_ResetBits(STEP_MOTOR_PORT,STEP_MOTOR_A | STEP_MOTOR_D);//0
		GPIO_SetBits(STEP_MOTOR_PORT,STEP_MOTOR_B | STEP_MOTOR_C);//1
			break;
		case 4:
		GPIO_ResetBits(STEP_MOTOR_PORT,STEP_MOTOR_A | STEP_MOTOR_B | STEP_MOTOR_D);//0
		GPIO_SetBits(STEP_MOTOR_PORT,STEP_MOTOR_C);//1
			break;
		case 5:
		GPIO_ResetBits(STEP_MOTOR_PORT,STEP_MOTOR_A | STEP_MOTOR_B);//0
		GPIO_SetBits(STEP_MOTOR_PORT,STEP_MOTOR_C | STEP_MOTOR_D);//1
			break;
		case 6:
		GPIO_ResetBits(STEP_MOTOR_PORT,STEP_MOTOR_A | STEP_MOTOR_B | STEP_MOTOR_C);//0
		GPIO_SetBits(STEP_MOTOR_PORT,STEP_MOTOR_D);//1
			break;
		case 7:
		GPIO_ResetBits(STEP_MOTOR_PORT,STEP_MOTOR_B | STEP_MOTOR_C);//0
		GPIO_SetBits(STEP_MOTOR_PORT,STEP_MOTOR_A | STEP_MOTOR_D);//1
			break;
		default:
			break;
	}
	DelayMs(speed); //��ʱ
	STEP_MOTOR_OFF();//����ϵ�״̬�����������
}
 
 
void STEP_MOTOR_NUM (u8 RL,u16 num,u8 speed)  //�������������
{
	u16 i;
	for(i=0;i<num;i++)
    {	
		if(RL==1){ //��RL=1��ת��RL=0��ת
			STEP++;
			if(STEP>7)STEP=0;
		}else{
			if(STEP==0)STEP=8;
			STEP--;
		}
		STEP_MOTOR_8A(STEP,speed);
	}
}
 
void STEP_MOTOR_LOOP (u8 RL,u8 LOOP,u8 speed)  //�����Ȧ������
{
	STEP_MOTOR_NUM(RL,LOOP*4076,speed); 
}

#ifndef __LED_H
#define __LED_H

//#define LED_PIN		GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_4
#define LED_PIN		GPIO_Pin_5
#define LED_PIN2  GPIO_Pin_8

void LED_Init(void);               //��ʼ��	
void LED_On(void);              //��������LED
void LED_Off(void);             //Ϩ������LED
void LED2_Init(void);
void LED2_On(void);
void LED2_Off(void);

#endif

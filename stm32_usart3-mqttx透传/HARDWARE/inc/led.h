
#ifndef __LED_H
#define __LED_H

//#define LED_PIN		GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_4
#define LED_PIN		GPIO_Pin_5


void LED_Init(void);               //初始化	
void LED_On(void);              //点亮所有LED
void LED_Off(void);             //熄灭所有LED

#endif

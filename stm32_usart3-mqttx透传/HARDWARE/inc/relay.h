#ifndef __RELAY_H
#define __RELAY_H
#include "sys.h"
//继电器端口定义
#define Relay_PIN GPIO_Pin_3	// Relay,继电器接口
//#define BEEP_PIN		GPIO_Pin_3
void Relay_Init(void);	//初始化
void Relay_On(void);
void Relay_Off(void);
void Relay2_Init(void);
void Relay2_Off(void);
void Relay2_On(void);
#endif

#ifndef _TIMER6_H
#define _TIMER6_H

extern char timer6_flag;   //外部变量声明，1:定时器6中断，有数据到

void TIM6_Init(unsigned short int, unsigned short int);

#endif

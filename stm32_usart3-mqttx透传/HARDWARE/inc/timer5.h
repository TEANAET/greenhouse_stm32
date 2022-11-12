
#ifndef _TIMER5_H
#define _TIMER5_H

extern char timer5_flag;   //外部变量声明，1:定时器4中断，有数据到

void TIM5_Init(unsigned short int, unsigned short int);

#endif

#ifndef	__BOARD_H
#define 	__BOARD_H
/*stm32�Ĺ̼���ͷ�ļ�*/
#include "stm32f10x.h"
#include "stm32f10x_it.h"
/*������Ӳ��ͷ�ļ�*/
#include "gpio.h"
#include "Pin_drives.h"
#include "iwdg.h"
#include "oled.h"
#include "usart.h"
#include "tim.h"
#include "can.h"
void rt_hw_board_init(void);
void SysTick_Handler(void);













#endif

#ifndef	__BOARD_H
#define 	__BOARD_H
/*stm32的固件库头文件*/
#include "stm32f10x.h"
#include "stm32f10x_it.h"
/*开发板硬件头文件*/
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

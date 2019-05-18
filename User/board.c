/*
 * File      : board.c
 * This file is part of RT-Thread RTOS
 * COPYRIGHT (C) 2006, RT-Thread Development Team
 *
 * The license and distribution terms for this file may be
 * found in the file LICENSE in this distribution or at
 * http://www.rt-thread.org/license/LICENSE
 *
 * Change Logs:
 * Date           Author       Notes
 * 2017-07-24     Tanek        the first version
 */
/*初始化硬件相关头文件*/
#include "board.h"
/*初始化rtthread相关头文件*/
#include <rthw.h>
#include <rtthread.h>

#if	0
/*========================================================*/
#define _SCB_BASE       (0xE000E010UL)
#define _SYSTICK_CTRL   (*(rt_uint32_t *)(_SCB_BASE + 0x0))
#define _SYSTICK_LOAD   (*(rt_uint32_t *)(_SCB_BASE + 0x4))
#define _SYSTICK_VAL    (*(rt_uint32_t *)(_SCB_BASE + 0x8))
#define _SYSTICK_CALIB  (*(rt_uint32_t *)(_SCB_BASE + 0xC))
#define _SYSTICK_PRI    (*(rt_uint8_t  *)(0xE000ED23UL))

// Updates the variable SystemCoreClock and must be called 
// whenever the core clock is changed during program execution.
extern void SystemCoreClockUpdate(void);

// Holds the system core clock, which is the system clock 
// frequency supplied to the SysTick timer and the processor 
// core clock.
extern uint32_t SystemCoreClock;

static uint32_t _SysTick_Config(rt_uint32_t ticks)
{
    if ((ticks - 1) > 0xFFFFFF)
    {
        return 1;
    }
    
    _SYSTICK_LOAD = ticks - 1; 
    _SYSTICK_PRI = 0xFF;
    _SYSTICK_VAL  = 0;
    _SYSTICK_CTRL = 0x07;  
    
    return 0;
}
/*==========================================================*/
#endif
#if defined(RT_USING_USER_MAIN) && defined(RT_USING_HEAP)
#define RT_HEAP_SIZE 1024
static uint32_t rt_heap[RT_HEAP_SIZE];	// heap default size: 4K(1024 * 4)
RT_WEAK void *rt_heap_begin_get(void)
{
    return rt_heap;
}

RT_WEAK void *rt_heap_end_get(void)
{
    return rt_heap + RT_HEAP_SIZE;
}
#endif

/**
 * This function will initial your board.
 */
/**
* @brief 重映射串口 DEBUG_USARTx 到 rt_kprintf()函数
* Note： DEBUG_USARTx 是在 bsp_usart.h 中定义的宏，默认使用串口 1
* @param str：要输出到串口的字符串
* @retval 无
*
* @attention
*
*/
void rt_hw_console_output(const char *str)
{
	/* 进入临界段 */
	rt_enter_critical();

	/* 直到字符串结束 */
	while (*str!='\0')
	{
	/* 换行 */
	if (*str=='\n')
	{
	USART_SendData(DEBUG_USARTx, '\r');
	while (USART_GetFlagStatus(DEBUG_USARTx, USART_FLAG_TXE) == RESET);
	}

	USART_SendData(DEBUG_USARTx, *str++);
	while (USART_GetFlagStatus(DEBUG_USARTx, USART_FLAG_TXE) == RESET);
	}

	/* 退出临界段 */
	rt_exit_critical();
}
//硬件初始化
static void HardWareInit(void)
{
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	ADC_DMA_ConfigInit();
	OLED_ConfigInit();
	USB_USART_Config();
	TIMx_NVIC_Configuration();
	Timer_out_ConfigInit(); 
	Digital_GPIO_Config();
	CANPro_Init();
	IWDG_Config(IWDG_Prescaler_32 ,635);//看门狗溢出时间(508ms)
}
void rt_hw_board_init()
{	
#if	0
	/* System Clock Update */
	SystemCoreClockUpdate();
	
	/* System Tick Configuration */
	_SysTick_Config(SystemCoreClock / RT_TICK_PER_SECOND);
#endif
	/*初始化Systick*/
	SysTick_Config(SystemCoreClock / RT_TICK_PER_SECOND);
	
	/* 硬件 BSP 初始化统统放在这里，比如 LED，串口， LCD 等 */
	HardWareInit();
    /* Call components board initial (use INIT_BOARD_EXPORT()) */
#ifdef RT_USING_COMPONENTS_INIT
    rt_components_board_init();
#endif

#if defined(RT_USING_CONSOLE) && defined(RT_USING_DEVICE)
	rt_console_set_device(RT_CONSOLE_DEVICE_NAME);
#endif
    
#if defined(RT_USING_USER_MAIN) && defined(RT_USING_HEAP)
    rt_system_heap_init(rt_heap_begin_get(), rt_heap_end_get());
#endif
}

void SysTick_Handler(void)
{
	/* enter interrupt */
	rt_interrupt_enter();

	rt_tick_increase();

	/* leave interrupt */
	rt_interrupt_leave();
}

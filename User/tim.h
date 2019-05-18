#ifndef __tim_H
#define __tim_H

#include "stm32f10x.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_it.h"
#include "stm32f10x_tim.h"

#include "rtthread.h"
#include "gpio.h"
#include "USART.h"
#include "ADC.h"

#define left_right_TIM_CLK		RCC_APB1Periph_TIM2
#define left_right_TIM			TIM2
#define left_right_TIM_IRQ		TIM2_IRQn
#define left_right_TIM_IRQHandler	TIM2_IRQHandler

#define tim1_it		1
#define tim2_it		2
#define tim3_it		3
#define SW1EXIT_IRQn			EXTI15_10_IRQn
#define BACKEXIT_IRQn			EXTI9_5_IRQn
#define PROFILEEXIT_IRQn		EXTI9_5_IRQn
#define FOGEXIT_IRQn			EXTI3_IRQn
#define HIGHEXIT_IRQn			EXTI9_5_IRQn
#define MIDEXIT_IRQn			EXTI9_5_IRQn
#define LOWEXIT_IRQn			EXTI9_5_IRQn
#define SEATEXIT_IRQn			EXTI15_10_IRQn
#define FROPOSEXIT_IRQn			EXTI15_10_IRQn



#define PWM_USER 0

#define LIGHT_GPIO_CLK		RCC_APB2Periph_GPIOA
#define LIGHT_GPIO_PORT		GPIOA
#define LIGHT_TIM_APBxClock_FUN		RCC_APB1PeriphClockCmd
//PA1-TIM2_CH2,PA2-TIM2_CH3,PA8-TIM1_CH1,PA7-TIM3_CH2,PA6-TIM3_CH1
/*红灯输出定义*/
#define REDLIGHT_TIM			TIM1
#define REDLIGHT_TIM_CLK		RCC_APB2Periph_TIM1
#define RED_TIM_IRQn			TIM1_UP_IRQn
#define RED_TIM_IRQHandler		TIM1_UP_IRQHandler
#define REDLIGHT_PIN			GPIO_Pin_8
#define REDLIGHT_CCRx			CCR1
#define REDLIGHT_TIM_CHANNEL		TIM_Channel_1



/*绿灯输出定义*/
#define GREENLIGHT_TIM			TIM3
#define GREENLIGHT_TIM_CLK		RCC_APB1Periph_TIM3
#define GREEN_TIM_IRQn			TIM3_IRQn
#define GREEN_TIM_IRQHandler		TIM3_IRQHandler
#define GREENLIGHT_PIN			GPIO_Pin_7
#define GREENLIGHT_CCRx			CCR2
#define GREENLIGHT_TIM_CHANNEL	TIM_Channel_2


/*蓝灯输出定义*/
#define BLUELIGHT_TIM			TIM3
#define BLUELIGHT_TIM_CLK		RCC_APB1Periph_TIM3
#define BLUE_TIM_IRQn			TIM3_IRQn
#define BLUE_TIM_IRQHandler		TIM3_IRQHandler
#define BLUELIGHT_PIN			GPIO_Pin_6
#define BLUELIGHT_CCRx			CCR1
#define BLUELIGHT_TIM_CHANNEL		TIM_Channel_1


/*远光灯输出定义*/
#define FARLIGHT_TIM			TIM2
#define FARLIGHT_TIM_CLK		RCC_APB1Periph_TIM2
#define FAR_TIM_IRQn			TIM2_IRQn
#define FAR_TIM_IRQHandler		TIM2_IRQHandler
#define FARLIGHT_PIN			GPIO_Pin_1
#define FARLIGHT_CCRx			CCR2
#define FARLIGHT_TIM_CHANNEL		TIM_Channel_2

/*近光灯输出定义*/
#define NEARLIGHT_TIM			TIM2
#define NEARLIGHT_TIM_CLK		RCC_APB1Periph_TIM2
#define NEAR_TIM_IRQn			TIM2_IRQn
#define NEAR_TIM_IRQHandler		TIM2_IRQHandler
#define NEARLIGHT_PIN			GPIO_Pin_2
#define NEARLIGHT_CCRx			CCR3
#define NEARLIGHT_TIM_CHANNEL		TIM_Channel_3


#define PeriodNUM					1000
#define PWMFull			255
#define PWMHungry		0




void Set_TimerX_IT(u8 timx_it,FunctionalState newState);

void TIMx_NVIC_Configuration(void);
void delayms(int timer);
void SysTick_Init(void);
void Delay_ms(__IO u32 timer);

void Timer1_BaseInitStruct_Config(u16 arr,u16 psc);
void Timer2_BaseInitStruct_Config(u16 arr,u16 psc);
void Timer3_BaseInitStruct_Config(u16 arr,u16 psc);
void TimerBaseInit(void);



u8 farlight_Set(u8 bool);
u8 nearlight_Set(u8 bool);


static void TIMx_GPIO_Config(void);
void Timer_out_ConfigInit(void);
int PWM_enable(TIM_TypeDef *Tim,int channel);
int PWM_disable(TIM_TypeDef *Tim,int channel);
void PWM_OUT(void);

#endif

































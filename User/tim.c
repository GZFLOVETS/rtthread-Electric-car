#include "tim.h"

extern digitalSignal DigitalSignal;

void TIMx_NVIC_Configuration(void)
{

	NVIC_InitTypeDef	NVIC_InitStructure;
	
	/*设置各定时器中断优先级配置*/
	
	NVIC_InitStructure.NVIC_IRQChannel = TIM1_UP_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
	NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}

void Timer1_BaseInitStruct_Config(u16 arr,u16 psc)
{
	TIM_TimeBaseInitTypeDef		TIM_TimeBaseStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);
	TIM_TimeBaseStructure.TIM_Period = arr-1;
	TIM_TimeBaseStructure.TIM_Prescaler = psc-1;
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInit(TIM1,&TIM_TimeBaseStructure);
}

void Timer2_BaseInitStruct_Config(u16 arr,u16 psc)
{
	TIM_TimeBaseInitTypeDef		TIM_TimeBaseStructure;
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
	TIM_TimeBaseStructure.TIM_Period = arr-1;
	TIM_TimeBaseStructure.TIM_Prescaler = psc-1;
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInit(TIM2,&TIM_TimeBaseStructure);
}
void Timer3_BaseInitStruct_Config(u16 arr,u16 psc)
{
	TIM_TimeBaseInitTypeDef		TIM_TimeBaseStructure;
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
	TIM_TimeBaseStructure.TIM_Period = arr-1;
	TIM_TimeBaseStructure.TIM_Prescaler = psc-1;
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInit(TIM3,&TIM_TimeBaseStructure);
}
void TimerBaseInit(void)
{
	Timer1_BaseInitStruct_Config(255,720);
	Timer2_BaseInitStruct_Config(255,720);
	Timer3_BaseInitStruct_Config(255,720);
}

void Set_TimerX_IT(u8 timx_it,FunctionalState newState)
{
	switch(timx_it)
		{
		case tim1_it:
			TIM_ITConfig(TIM1,TIM_IT_Update,newState);
		break;
		case tim2_it:
			TIM_ITConfig(TIM2,TIM_IT_Update,newState);
		break;
		case tim3_it:
			TIM_ITConfig(TIM3,TIM_IT_Update,newState);
		break;
		default :break;
		}
}


static void TIMx_GPIO_Config(void)
{
	GPIO_InitTypeDef	GPIO_InitStructure;

	RCC_APB2PeriphClockCmd(LIGHT_GPIO_CLK,ENABLE);
	GPIO_InitStructure.GPIO_Pin = (REDLIGHT_PIN	|
							 	GREENLIGHT_PIN	|
								BLUELIGHT_PIN	|
								 FARLIGHT_PIN	|
							 NEARLIGHT_PIN);
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
	GPIO_Init(LIGHT_GPIO_PORT,&GPIO_InitStructure);
}

static void RedLight_PwmMode_Config(void)
{
	TIM_OCInitTypeDef 			TIM_OCInitStructure;
	TIM_ClearFlag(REDLIGHT_TIM,TIM_FLAG_Update);
	/*red light PWM模式配置*/
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_Pulse = 0;
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
	TIM_OC1Init(REDLIGHT_TIM,&TIM_OCInitStructure);
	TIM_OC1PreloadConfig(REDLIGHT_TIM,TIM_OCPreload_Enable);
	TIM_ARRPreloadConfig(REDLIGHT_TIM,ENABLE);
	
	TIM_CtrlPWMOutputs(REDLIGHT_TIM,ENABLE);//高级定时器必须开启这个
	
	TIM_Cmd(REDLIGHT_TIM,ENABLE);
	TIM_ITConfig(TIM1,TIM_IT_Update,DISABLE);

}
static void BlueLight_PwmMode_Config(void)
{	
	TIM_OCInitTypeDef 			TIM_OCInitStructure;
	TIM_ClearFlag(BLUELIGHT_TIM,TIM_FLAG_Update);
	/*bule light PWM模式配置*/
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_Pulse = 0;
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
	TIM_OC1Init(BLUELIGHT_TIM,&TIM_OCInitStructure);
	TIM_OC1PreloadConfig(BLUELIGHT_TIM,TIM_OCPreload_Enable);
	TIM_ARRPreloadConfig(BLUELIGHT_TIM,ENABLE);
	TIM_Cmd(BLUELIGHT_TIM,ENABLE);
	TIM_ITConfig(TIM3,TIM_IT_Update,DISABLE);

}

static void GreenLight_PwmMode_Config(void)
{
	TIM_OCInitTypeDef 			TIM_OCInitStructure;
	TIM_ClearFlag(GREENLIGHT_TIM,TIM_FLAG_Update);
	/*green light PWM模式配置*/
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_Pulse = 0;
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
	TIM_OC2Init(GREENLIGHT_TIM,&TIM_OCInitStructure);
	TIM_OC2PreloadConfig(GREENLIGHT_TIM,TIM_OCPreload_Enable);
	TIM_ARRPreloadConfig(GREENLIGHT_TIM,ENABLE);
	TIM_Cmd(GREENLIGHT_TIM,ENABLE);
	TIM_ITConfig(TIM3,TIM_IT_Update,DISABLE);
}

static void FarLight_PwmMode_Config(void)
{
	TIM_OCInitTypeDef 			TIM_OCInitStructure;
	TIM_ClearFlag(FARLIGHT_TIM,TIM_FLAG_Update);
	/*FARLIGHT light PWM模式配置*/
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_Pulse = 0;
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
	TIM_OC2Init(FARLIGHT_TIM,&TIM_OCInitStructure);
	TIM_OC2PreloadConfig(FARLIGHT_TIM,TIM_OCPreload_Enable);
	TIM_ARRPreloadConfig(FARLIGHT_TIM,ENABLE);
	TIM_Cmd(FARLIGHT_TIM,ENABLE);
	TIM_ITConfig(TIM2,TIM_IT_Update,DISABLE);
}
static void NearLight_PwmMode_Config(void)
{
	TIM_OCInitTypeDef 			TIM_OCInitStructure;
	TIM_ClearFlag(NEARLIGHT_TIM,TIM_FLAG_Update);
	/*near light PWM模式配置*/
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_Pulse = 0;
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
	TIM_OC3Init(NEARLIGHT_TIM,&TIM_OCInitStructure);
	TIM_OC3PreloadConfig(NEARLIGHT_TIM,TIM_OCPreload_Enable);
	TIM_ARRPreloadConfig(NEARLIGHT_TIM,ENABLE);
	TIM_Cmd(NEARLIGHT_TIM,ENABLE);
	TIM_ITConfig(TIM2,TIM_IT_Update,DISABLE);
	
}

int PWM_enable(TIM_TypeDef *Tim,int channel)
{
	if(Tim!=TIM1 && Tim!=TIM2 && Tim!=TIM3 && Tim!=TIM4)
	{
		return ERROR;
	}else {
		TIM_CCxCmd(Tim,channel,TIM_CCx_Enable);
		return SUCCESS;
	}
}
int PWM_disable(TIM_TypeDef *Tim,int channel)
{
	if(Tim!=TIM1 && Tim!=TIM2 && Tim!=TIM3 && Tim!=TIM4)
	{
		return ERROR;
	}else {
		TIM_CCxCmd(Tim,channel,TIM_CCx_Disable);
		return SUCCESS;
	}
}


#if PWM_USER
static void PWM_OUT(void)
{
	static int i=0;
	//pwm test code 
	TIM_SetCompare1(REDLIGHT_TIM,i);
	TIM_SetCompare2(GREENLIGHT_TIM,i);
	TIM_SetCompare1(BLUELIGHT_TIM,i);
	TIM_SetCompare2(FARLIGHT_TIM,i);
	TIM_SetCompare3(NEARLIGHT_TIM,i);
	if(i<255) i++;
	else i=0;
}
#endif
 u8 farlight_Set(u8 bool)
{
	u8 ret=0;
	if(bool == 1)
		{
		TIM_SetCompare2(FARLIGHT_TIM,PWMFull);
		ret = 1;
		}
	else if(bool == 0)
		{
		TIM_SetCompare2(FARLIGHT_TIM,PWMHungry);
		ret = 0;
		}
		return ret;
}

 u8 nearlight_Set(u8 bool)
{
	u8 ret =0;
	if(bool == 1)
		{
		TIM_SetCompare3(NEARLIGHT_TIM,PWMFull);
		ret = 1;
		}
	else if(bool == 0)
		{
		TIM_SetCompare3(NEARLIGHT_TIM,PWMHungry);
		ret = 0;
		}
		return ret ;
}

void Timer_out_ConfigInit(void)
{
	TIMx_GPIO_Config();
	TimerBaseInit();
	RedLight_PwmMode_Config();
	GreenLight_PwmMode_Config();
	BlueLight_PwmMode_Config();
	FarLight_PwmMode_Config();
	NearLight_PwmMode_Config();
	PWM_disable(TIM1,TIM_Channel_1);
	PWM_disable(TIM3,TIM_Channel_2);
	PWM_disable(TIM3,TIM_Channel_1);
	PWM_disable(TIM2,TIM_Channel_2);
	PWM_disable(TIM2,TIM_Channel_3);
}
/*任务线程:输出pwm*/
void tim_pwm_thread_entry(void *parameter)
{
	while(1)
	{
		
		if(1==DigitalSignal.KeyStatus.farlightkey)
		{
			PWM_enable(TIM2,TIM_Channel_2);
			DigitalSignal.Light_LockStatus.far_light = farlight_Set(DigitalSignal.KeyStatus.farlightkey);
		}else PWM_disable(TIM2,TIM_Channel_2);
		
		if(1==DigitalSignal.KeyStatus.nearlightkey)
		{
			PWM_enable(TIM2,TIM_Channel_3);
			DigitalSignal.Light_LockStatus.near_light = nearlight_Set(DigitalSignal.KeyStatus.nearlightkey);
		}else PWM_disable(TIM2,TIM_Channel_3);
		
		rt_thread_delay(20);/* 延 时 20 个 systick*/
	}
}
















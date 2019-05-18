#include "gpio.h"
#include "Pin_drives.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "rtthread.h"
digitalSignal DigitalSignal;

/*数字量长动开关状态输入*/
static uint8_t Digital_longmove_status(GPIO_TypeDef* GPIOx,uint16_t GPIO_Pin)
{
	if(GPIO_ReadInputDataBit(GPIOx,GPIO_Pin) == DISABLE)
	{
		return success;
	}
	else return fail;
}

/*数字量点动开关状态输入*/
static  uint8_t Digital_pointmove_status(GPIO_TypeDef* GPIOx,uint16_t GPIO_Pin)
{
	if(GPIO_ReadInputDataBit(GPIOx,GPIO_Pin) == DISABLE)
	{
		while(GPIO_ReadInputDataBit(GPIOx,GPIO_Pin) == DISABLE);//必须消除阻塞,采用EXTI外部中断上升沿触发。
		return KEYON;
	}
	else return KEYOFF;
}
static uint8_t Set_Digital_Put(GPIO_TypeDef * GPIOx,uint16_t PIN,uint8_t bool)
{
	if(1==bool)
		{
		GPIO_SetBits(GPIOx,PIN);
		return success;
		}
	else 
		{
		GPIO_ResetBits(GPIOx,PIN);
		return fail;
		}
}	


void GPIO_In_ConfigInit(void)
{
	/* 开关：远光灯,近光灯,按键1,轮廓灯,尾灯,倒车灯 */
	pin_mode(far_lightkey,GPIO_Mode_IN_FLOATING);
	pin_mode(near_lightkey,GPIO_Mode_IN_FLOATING);
	pin_mode(SW1,GPIO_Mode_IN_FLOATING);
	pin_mode(profile_lightkey,GPIO_Mode_IN_FLOATING);
	pin_mode(tail_lightkey,GPIO_Mode_IN_FLOATING);
	pin_mode(back_lightkey,GPIO_Mode_IN_FLOATING);
	
	/* 开关：左转向,右转向,高速,中速,低速,倒车,座位锁,前位锁,按键2 */
	pin_mode(left_turnkey,GPIO_Mode_IN_FLOATING);
	pin_mode(right_turnkey,GPIO_Mode_IN_FLOATING);
	pin_mode(high_speedkey,GPIO_Mode_IN_FLOATING);
	pin_mode(med_speedkey,GPIO_Mode_IN_FLOATING);
	pin_mode(low_speedkey,GPIO_Mode_IN_FLOATING);
	pin_mode(back_speedkey,GPIO_Mode_IN_FLOATING);
	pin_mode(seat_lockkey,GPIO_Mode_IN_FLOATING);
	pin_mode(froPos_lockkey,GPIO_Mode_IN_FLOATING);
	pin_mode(SW2,GPIO_Mode_IN_FLOATING);
	/* 开关：喇叭 */
	pin_mode(horn_key,GPIO_Mode_IN_FLOATING);

}
void GPIO_Out_ConfigInit(void)
{
	/* 输出: 左转向灯,右转向灯,轮廓灯,车牌,*/
	pin_mode(left_lightput,GPIO_Mode_Out_PP);
	pin_mode(right_lightput,GPIO_Mode_Out_PP);
	pin_mode(profile_lightput,GPIO_Mode_Out_PP);
	pin_mode(carbrand_lightput,GPIO_Mode_Out_PP);
	/* 输出: 雾灯,尾灯,倒车灯,座位锁,前位锁*/
	pin_mode(fog_lightput,GPIO_Mode_Out_PP);
	pin_mode(tail_lightput,GPIO_Mode_Out_PP);
	pin_mode(back_lightput,GPIO_Mode_Out_PP);
	pin_mode(seat_lockput,GPIO_Mode_Out_PP);
	pin_mode(froPos_lockput,GPIO_Mode_Out_PP);
	/* 输出:LED测试灯,刹车灯 */
	pin_mode(LED_TEST,GPIO_Mode_Out_PP);
	pin_mode(brake_lightput,GPIO_Mode_Out_PP);
}
void Digital_In(void)
{
	//printf("数字量输入\r\n");
	DigitalSignal.KeyStatus.leftturnkey = !pin_read(left_turnkey);
	//printf("左转向开关:%d 开:1,关:0\r\n",KeyStatus.leftturnkey);

	DigitalSignal.KeyStatus.rightturnkey= !pin_read(right_turnkey);
	//printf("右转向灯开关:%d 开:1,关:0\r\n",KeyStatus.rightturnkey);

	DigitalSignal.KeyStatus.farlightkey = !pin_read(far_lightkey);
	//printf("远光灯开关:%d 开:1,关:0\r\n",KeyStatus.farlightkey);

	DigitalSignal.KeyStatus.nearlightkey = !pin_read(near_lightkey);
	//printf("近光灯开关:%d 开:1,关:0\r\n",KeyStatus.nearlightkey);

	DigitalSignal.KeyStatus.highspeedkey= !pin_read(high_speedkey);
	//printf("高速开关:%d 开:1,关:0\r\n",KeyStatus.highspeedkey);

	DigitalSignal.KeyStatus.medspeedkey= !pin_read(med_speedkey);
	//printf("中速开关:%d 开:1,关:0\r\n",KeyStatus.medspeedkey);

	DigitalSignal.KeyStatus.lowspeedkey= !pin_read(low_speedkey);
	//printf("低速开关:%d 开:1,关:0\r\n",KeyStatus.lowspeedkey);

	DigitalSignal.KeyStatus.backspeedkey= !pin_read(back_speedkey);
	//printf("倒车开关:%d 开:1,关:0\r\n",KeyStatus.backspeedkey);

	DigitalSignal.KeyStatus.seatlockkey= !pin_read(seat_lockkey);
	//printf("坐垫锁开关:%d 开:1,关:0\r\n",KeyStatus.seatlockkey);
	DigitalSignal.KeyStatus.froPoslockkey= !pin_read(froPos_lockkey);
	//printf("前位锁开关:%d 开:1,关:0\r\n",KeyStatus.froPoslockkey);
	
	DigitalSignal.KeyStatus.sw2= !pin_read(SW2);
	//printf("按键2开关:%d 开:1,关:0\r\n",KeyStatus.sw2);

	//Task1.KeyStatus.sw1= !pin_read(SW1);
	//printf("按键1开关:%d 开:1,关:0\r\n",KeyStatus.sw1);

	DigitalSignal.KeyStatus.hornkey= !pin_read(horn_key);
	//printf("喇叭开关:%d 开:1,关:0\r\n",KeyStatus.hornkey);

	DigitalSignal.KeyStatus.foglightkey= !pin_read(fog_lightkey);
	//printf("雾灯开关:%d 开:1,关:0\r\n",KeyStatus.foglightkey);

	DigitalSignal.KeyStatus.brakelightkey= !pin_read(brake_lightkey);
	//printf("刹车灯开关:%d 开:1,关:0\r\n",KeyStatus.brakelightkey);

	DigitalSignal.KeyStatus.profilelightkey= !pin_read(profile_lightkey);
	//printf("轮廓灯开关:%d 开:1,关:0\r\n",KeyStatus.profilelightkey);

	DigitalSignal.KeyStatus.taillightkey= !pin_read(tail_lightkey);
	//printf("尾灯开关:%d 开:1,关:0\r\n",KeyStatus.taillightkey);

	DigitalSignal.KeyStatus.backlightkey= !pin_read(back_lightkey);
	//printf("倒车灯开关:%d 开:1,关:0\r\n",KeyStatus.backlightkey);

}
void Digital_Out(void)
{
	//数字量输出
	DigitalSignal.Light_LockStatus.brake_light = pin_write(brake_lightput,DigitalSignal.KeyStatus.brakelightkey);
	
	DigitalSignal.Light_LockStatus.seat_lock = pin_write(seat_lockput,DigitalSignal.KeyStatus.seatlockkey);
	DigitalSignal.Light_LockStatus.froPos_lock = pin_write(froPos_lockput,DigitalSignal.KeyStatus.froPoslockkey);

	DigitalSignal.Light_LockStatus.profile_light = pin_write(profile_lightput,DigitalSignal.KeyStatus.profilelightkey);
	DigitalSignal.Light_LockStatus.carbrand_light = pin_write(carbrand_lightput,DigitalSignal.KeyStatus.hornkey);//车牌灯输出换成喇叭。。。
	
	DigitalSignal.Light_LockStatus.fog_light = pin_write(fog_lightput,DigitalSignal.KeyStatus.foglightkey);
	DigitalSignal.Light_LockStatus.tail_light = pin_write(tail_lightput,DigitalSignal.KeyStatus.taillightkey|DigitalSignal.KeyStatus.brakelightkey|DigitalSignal.KeyStatus.backspeedkey);
	DigitalSignal.Light_LockStatus.back_light = pin_write(back_lightput,DigitalSignal.KeyStatus.backspeedkey);
	
	DigitalSignal.Light_LockStatus.Trunk_lock = pin_write(Trunk_lockput,KEYOFF);

}

/*初始化任务变量*/
void InitDigitalSignal(void)
{
	DigitalSignal.KeyStatus.backlightkey=0;
	DigitalSignal.KeyStatus.backspeedkey=0;
	DigitalSignal.KeyStatus.bluelightkey=0;
	DigitalSignal.KeyStatus.brakelightkey=0;
	DigitalSignal.KeyStatus.carbrandlightkey=0;
	DigitalSignal.KeyStatus.farlightkey=0;
	DigitalSignal.KeyStatus.foglightkey=0;
	DigitalSignal.KeyStatus.froPoslockkey=0;
	DigitalSignal.KeyStatus.greenlightkey=0;
	DigitalSignal.KeyStatus.highspeedkey=0;
	DigitalSignal.KeyStatus.hornkey=0;
	DigitalSignal.KeyStatus.leftturnkey=0;
	DigitalSignal.KeyStatus.lowspeedkey=0;
	DigitalSignal.KeyStatus.medspeedkey=0;
	DigitalSignal.KeyStatus.nearlightkey=0;
	DigitalSignal.KeyStatus.profilelightkey=0;
	DigitalSignal.KeyStatus.redlightkey=0;
	DigitalSignal.KeyStatus.rightturnkey=0;
	DigitalSignal.KeyStatus.seatlockkey=0;
	DigitalSignal.KeyStatus.sw1=0;
	DigitalSignal.KeyStatus.sw2=0;
	DigitalSignal.KeyStatus.taillightkey=0;
	
	DigitalSignal.Light_LockStatus.back_light=0;
	DigitalSignal.Light_LockStatus.blue_light=0;
	DigitalSignal.Light_LockStatus.brake_light=0;
	DigitalSignal.Light_LockStatus.carbrand_light=0;
	DigitalSignal.Light_LockStatus.far_light=0;
	DigitalSignal.Light_LockStatus.fog_light=0;
	DigitalSignal.Light_LockStatus.froPos_lock=0;
	DigitalSignal.Light_LockStatus.green_light=0;
	DigitalSignal.Light_LockStatus.LEDTEST=0;
	DigitalSignal.Light_LockStatus.left_light=0;
	DigitalSignal.Light_LockStatus.near_light=0;
	DigitalSignal.Light_LockStatus.Trunk_lock=0;
	DigitalSignal.Light_LockStatus.profile_light=0;
	DigitalSignal.Light_LockStatus.red_light=0;
	DigitalSignal.Light_LockStatus.right_light=0;
	DigitalSignal.Light_LockStatus.seat_lock=0;
	DigitalSignal.Light_LockStatus.tail_light=0;

	DigitalSignal.left_turn_flag=0;
	DigitalSignal.right_turn_flag=0;

}
/*外部中断时必须开启AFIO时钟*/
void SW1EXIT_Control(FunctionalState newStart)
{
	/*配置EXIT外部中断源*/
	EXTI_InitTypeDef EXTI_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOB, GPIO_PinSource11);
	EXTI_InitStructure.EXTI_Line = EXTI_Line11;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
	EXTI_InitStructure.EXTI_LineCmd = newStart;
	EXTI_Init(&EXTI_InitStructure);
}
/*外部中断时必须开启AFIO时钟*/
void PROFILEEXIT_Control(FunctionalState newStart)
{
	/*配置EXIT外部中断源*/
	EXTI_InitTypeDef EXTI_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOB, GPIO_PinSource5);
	EXTI_InitStructure.EXTI_Line = EXTI_Line5;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
	EXTI_InitStructure.EXTI_LineCmd = newStart;
	EXTI_Init(&EXTI_InitStructure);
}
/*外部中断时必须开启AFIO时钟*/
void HIGHEXIT_Control(FunctionalState newStart)
{
	/*配置EXIT外部中断源*/
	EXTI_InitTypeDef EXTI_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOC, GPIO_PinSource6);
	EXTI_InitStructure.EXTI_Line = EXTI_Line6;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
	EXTI_InitStructure.EXTI_LineCmd = newStart;
	EXTI_Init(&EXTI_InitStructure);
}
/*外部中断时必须开启AFIO时钟*/
void MIDEXIT_Control(FunctionalState newStart)
{
	/*配置EXIT外部中断源*/
	EXTI_InitTypeDef EXTI_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOC, GPIO_PinSource7);
	EXTI_InitStructure.EXTI_Line = EXTI_Line7;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
	EXTI_InitStructure.EXTI_LineCmd = newStart;
	EXTI_Init(&EXTI_InitStructure);
}
/*外部中断时必须开启AFIO时钟*/
void LOWEXIT_Control(FunctionalState newStart)
{
	/*配置EXIT外部中断源*/
	EXTI_InitTypeDef EXTI_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOC, GPIO_PinSource8);
	EXTI_InitStructure.EXTI_Line = EXTI_Line8;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
	EXTI_InitStructure.EXTI_LineCmd = newStart;
	EXTI_Init(&EXTI_InitStructure);
}
/*外部中断时必须开启AFIO时钟*/
void FOGEXIT_Control(FunctionalState newStart)
{
	/*配置EXIT外部中断源*/
	EXTI_InitTypeDef EXTI_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOB, GPIO_PinSource3);
	EXTI_InitStructure.EXTI_Line = EXTI_Line3;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
	EXTI_InitStructure.EXTI_LineCmd = newStart;
	EXTI_Init(&EXTI_InitStructure);
}
/*外部中断时必须开启AFIO时钟*/
void BACKEXIT_Control(FunctionalState newStart)
{
	/*配置EXIT外部中断源*/
	EXTI_InitTypeDef EXTI_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOB, GPIO_PinSource9);
	EXTI_InitStructure.EXTI_Line = EXTI_Line9;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
	EXTI_InitStructure.EXTI_LineCmd = newStart;
	EXTI_Init(&EXTI_InitStructure);
}
void SeatLockEXIT_Control(FunctionalState newStart)
{
	/*配置EXIT外部中断源*/
	EXTI_InitTypeDef EXTI_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOC, GPIO_PinSource10);
	EXTI_InitStructure.EXTI_Line = EXTI_Line10;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
	EXTI_InitStructure.EXTI_LineCmd = newStart;
	EXTI_Init(&EXTI_InitStructure);
}
void FroPosLockEXIT_Control(FunctionalState newStart)
{
	/*配置EXIT外部中断源*/
	EXTI_InitTypeDef EXTI_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOC, GPIO_PinSource11);
	EXTI_InitStructure.EXTI_Line = EXTI_Line11;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
	EXTI_InitStructure.EXTI_LineCmd = newStart;
	EXTI_Init(&EXTI_InitStructure);
}
void Digital_NVIC_Init(void)
{
	NVIC_InitTypeDef	NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel = EXTI15_10_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE ;
	NVIC_Init(&NVIC_InitStructure);
	NVIC_InitStructure.NVIC_IRQChannel = EXTI9_5_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE ;
	NVIC_Init(&NVIC_InitStructure);
	NVIC_InitStructure.NVIC_IRQChannel = EXTI3_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE ;
	NVIC_Init(&NVIC_InitStructure);
}
void EXTI_KEY_Init(void)
{
	SW1EXIT_Control(ENABLE);
}
void Digital_GPIO_Config(void)
{
	Digital_NVIC_Init();
	GPIO_In_ConfigInit();
	GPIO_Out_ConfigInit();
	InitDigitalSignal();
	EXTI_KEY_Init();
}


/*任务线程:数字量采集与灯光控制*/
void digital_thread_entry(void *parameter)
{
	
	while(1)
	{
		Digital_In();
		Digital_Out();
		rt_thread_delay(20); /* 延时 20 个 tick */
	}
}
/*任务线程:系统指示灯led*/
void ledTest_thread_entry(void *parameter)
{
	while(1)
	{
		digitalToggle(GPIOC,GPIO_Pin_5);
		rt_thread_delay(500); /* 延时 500 个 tick */
	}
}
/*任务线程:转向灯闪烁*/
void turnLight_thread_entry(void *parameter)
{
	while(1)
	{

		if(1==DigitalSignal.KeyStatus.leftturnkey)
		{
			digitalToggle(GPIOA,GPIO_Pin_3);
		}else pin_write(left_lightput,0);
		
		if(1==DigitalSignal.KeyStatus.rightturnkey)
		{
			digitalToggle(GPIOA,GPIO_Pin_0);
		}else pin_write(right_lightput,0);
		rt_thread_delay(400);/* 延 时 20 个 systick*/
	}
}

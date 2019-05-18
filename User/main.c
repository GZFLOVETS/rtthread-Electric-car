/**
  ******************************************************************************
  * @file    main.c
  * @author  fire
  * @version V1.0
  * @date    2013-xx-xx
  * @brief   测试led
  ******************************************************************************
  * @attention
  *
  * 实验平台:秉火 F103-霸道 STM32 开发板 
  * 论坛    :http://www.firebbs.cn
  * 淘宝    :https://fire-stm32.taobao.com
  *
  ******************************************************************************
  */ 
	
#include "board.h"
#include <rtthread.h>
/*===============静态线程的创建=======================*/
static struct rt_thread digital_thread;
static struct rt_thread adc_thread;
static struct rt_thread flatbed_thread;
/* 定义线程控栈时要求 RT_ALIGN_SIZE 个字节对齐 */
ALIGN(RT_ALIGN_SIZE)
/* 定义线程栈 */
static rt_uint8_t digital_thread_stack[1024];
static rt_uint8_t adc_thread_stack[256];
static rt_uint8_t flatbed_thread_stack[512];
/*=======================================================*/

/*===================动态线程的创建=======================*/
static rt_thread_t ledTest_thread = RT_NULL;
static rt_thread_t iwdg_thread = RT_NULL;
static rt_thread_t oled_thread = RT_NULL;
static rt_thread_t tim_pwm_thread = RT_NULL;
static rt_thread_t can_thread = RT_NULL;
static rt_thread_t turnLight_thread = RT_NULL;
/*=======================================================*/
extern void digital_thread_entry(void *parameter);
extern void adc_thread_entry(void *parameter);
extern void ledTest_thread_entry(void *parameter);
extern void iwdg_thread_entry(void *parameter);
extern void oled_thread_entry(void *parameter);
extern void tim_pwm_thread_entry(void *parameter);
extern void can_thread_entry(void *parameter);
extern void turnLight_thread_entry(void *parameter);
extern void flatbed_thread_entry(void *parameter);

/*JTAG做普通引脚时GPIO最后配置*/
static void JTAG_GPIO_Config(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);//引脚映射时必须开启AFIO时钟
	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE);/*使能SWD 禁用JTAG*/

	pin_mode(fog_lightkey,GPIO_Mode_IN_FLOATING);

	pin_mode(brake_lightkey,GPIO_Mode_IN_FLOATING);
}

/**
  * @brief  主函数
  * @param  无
  * @retval 无
  */
int main(void)
{	
	JTAG_GPIO_Config();//JTAG的GPIO初始化必须放置在GPIO配置的最后面
	
	rt_thread_init(&digital_thread,"digital",digital_thread_entry,RT_NULL,&digital_thread_stack,sizeof(digital_thread_stack),10,20);
	rt_thread_startup(&digital_thread);
	
	rt_thread_init(&adc_thread,"adc",adc_thread_entry,RT_NULL,&adc_thread_stack,sizeof(adc_thread_stack),10,20);
	rt_thread_startup(&adc_thread);
	
	rt_thread_init(&flatbed_thread,"flatbed",flatbed_thread_entry,RT_NULL,&flatbed_thread_stack,sizeof(flatbed_thread_stack),6,20);
	rt_thread_startup(&flatbed_thread);
	
	ledTest_thread = rt_thread_create("ledtest",ledTest_thread_entry,RT_NULL,128,5,20);
	if(ledTest_thread!=RT_NULL)
	{
		rt_thread_startup(ledTest_thread);
	}else return -1;
	
	iwdg_thread = rt_thread_create("iwdg",iwdg_thread_entry,RT_NULL,128,5,20);
	if(iwdg_thread!=RT_NULL)
	{
		rt_thread_startup(iwdg_thread);
	}else return -1;
	
	oled_thread = rt_thread_create("oled",oled_thread_entry,RT_NULL,512,10,100);
	if(oled_thread!=RT_NULL)
	{
		rt_thread_startup(oled_thread);
	}else return -1;
	
	tim_pwm_thread = rt_thread_create("timpwm",tim_pwm_thread_entry,RT_NULL,512,10,20);
	if(tim_pwm_thread!=RT_NULL)
	{
		rt_thread_startup(tim_pwm_thread);
	}else return -1;
	
	can_thread = rt_thread_create("can",can_thread_entry,RT_NULL,512,8,20);
	if(can_thread!=RT_NULL)
	{
		rt_thread_startup(can_thread);
	}else return -1;
	
	turnLight_thread = rt_thread_create("turnLig",turnLight_thread_entry,RT_NULL,128,10,20);
	if(turnLight_thread!=RT_NULL)
	{
		rt_thread_startup(turnLight_thread);
	}else return -1;
}

/*********************************************END OF FILE**********************/

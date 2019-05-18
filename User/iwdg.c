#include "iwdg.h"

/*
功能：独立看门狗配置
形参：预分频值：prv，重装载寄存器值：rlv
备注：
溢 出 时 间 Tout = prv/40 * rlv (s) ， prv 可 以 是
[4,8,16,32,64,128,256]。如果我们需要设置 1s 的超时溢出， prv可以取 IWDG_Prescaler_64，
rlv 取 625，即调用:IWDG_Config(IWDG_Prescaler_64 ,625)。 Tout=64/40*625=1s。
*/
void IWDG_Config(uint8_t prv,uint16_t rlv)
{
	IWDG_WriteAccessCmd(IWDG_WriteAccess_Enable);
	
	IWDG_SetPrescaler(prv);
	IWDG_SetReload(rlv);
	IWDG_ReloadCounter();
	
	IWDG_Enable();
}

/*
功能：喂狗
*/
void IWDG_Feed(void)
{
	//把重装载寄存器的值放到计数器中，喂狗，防止 IWDG 复位
	//当计数器的值减到 0 的时候会产生系统复位
	IWDG_ReloadCounter();
}


/*任务线程:独立看门狗*/
void iwdg_thread_entry(void *parameter)
{
	while(1)
	{
		IWDG_Feed();
		rt_thread_delay(500); /* 延时 500 个 tick */
	}
}


























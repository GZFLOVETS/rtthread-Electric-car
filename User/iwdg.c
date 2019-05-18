#include "iwdg.h"

/*
���ܣ��������Ź�����
�βΣ�Ԥ��Ƶֵ��prv����װ�ؼĴ���ֵ��rlv
��ע��
�� �� ʱ �� Tout = prv/40 * rlv (s) �� prv �� �� ��
[4,8,16,32,64,128,256]�����������Ҫ���� 1s �ĳ�ʱ����� prv����ȡ IWDG_Prescaler_64��
rlv ȡ 625��������:IWDG_Config(IWDG_Prescaler_64 ,625)�� Tout=64/40*625=1s��
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
���ܣ�ι��
*/
void IWDG_Feed(void)
{
	//����װ�ؼĴ�����ֵ�ŵ��������У�ι������ֹ IWDG ��λ
	//����������ֵ���� 0 ��ʱ������ϵͳ��λ
	IWDG_ReloadCounter();
}


/*�����߳�:�������Ź�*/
void iwdg_thread_entry(void *parameter)
{
	while(1)
	{
		IWDG_Feed();
		rt_thread_delay(500); /* ��ʱ 500 �� tick */
	}
}


























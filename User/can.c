#include "stm32f10x.h"
#include "stm32f10x_gpio.h"
#include "can.h"
#include "stm32f10x_it.h"
#include <string.h>

uint8_t Can_Reve_Flag=0;
CanTxMsg TxMessage;
CanRxMsg RxMessage;
static void CAN_GPIO_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(CAN_TX_GPIO_CLK|CAN_RX_GPIO_CLK, ENABLE);
	
	GPIO_InitStructure.GPIO_Pin = CAN_TX_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(CAN_TX_GPIO_PORT, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = CAN_RX_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(CAN_RX_GPIO_PORT, &GPIO_InitStructure);
	
}

static void CAN_Mode_Config(void)
{
	CAN_InitTypeDef CAN_InitStructure;
	
	RCC_APB1PeriphClockCmd(CAN_CLK, ENABLE);
	CAN_DeInit(CANx);
	CAN_StructInit(&CAN_InitStructure);
	
	CAN_InitStructure.CAN_TTCM = DISABLE;
	CAN_InitStructure.CAN_ABOM = ENABLE;
	CAN_InitStructure.CAN_AWUM = ENABLE;
	CAN_InitStructure.CAN_NART = DISABLE;
	CAN_InitStructure.CAN_RFLM = DISABLE;
	CAN_InitStructure.CAN_TXFP = DISABLE;
	
	CAN_InitStructure.CAN_Mode = CAN_Mode_Normal;
	CAN_InitStructure.CAN_SJW = CAN_SJW_2tq;
	CAN_InitStructure.CAN_BS1 = CAN_BS1_4tq;
	CAN_InitStructure.CAN_BS2 = CAN_BS2_3tq;
	
	CAN_InitStructure.CAN_Prescaler = 36;
	CAN_Init(CANx,&CAN_InitStructure);
	/*计算CAN总线的波特率：
		1Tq = 1/(36M/CAN_Prescaler) 
		Tbit = （CAN_BS2_Xtq+CAN_BS1_Xtq+1）*1Tq = 8us
		波特率 = 1/Tbit = 125Khz
	*/
}

static void CAN_Filter_Config(u32 FilterId)
{
	CAN_FilterInitTypeDef CAN_FilterInitStructure;
	
	CAN_FilterInitStructure.CAN_FilterNumber = 0;
	CAN_FilterInitStructure.CAN_FilterMode = CAN_FilterMode_IdMask;
	CAN_FilterInitStructure.CAN_FilterScale = CAN_FilterScale_32bit;
	/*使能筛选器，按照标志符的内容进行对比筛选*/
	CAN_FilterInitStructure.CAN_FilterIdHigh = ((((u32)FilterId<<3)|CAN_ID_EXT|CAN_RTR_DATA)&0XFFFF0000)>>16;
	CAN_FilterInitStructure.CAN_FilterIdLow = (((u32)FilterId<<3)|CAN_ID_EXT|CAN_RTR_DATA)&0XFFFF;
	CAN_FilterInitStructure.CAN_FilterMaskIdHigh = ((u32)FilterId>>16)&0XFF;
	CAN_FilterInitStructure.CAN_FilterMaskIdLow = ((u32)FilterId)&0XFF;
	CAN_FilterInitStructure.CAN_FilterFIFOAssignment = CAN_Filter_FIFO0;
	CAN_FilterInitStructure.CAN_FilterActivation = ENABLE;
	CAN_FilterInit(&CAN_FilterInitStructure);
	
	CAN_ITConfig(CANx,CAN_IT_FMP0,ENABLE);
}
static void CAN_NVIC_Config(void)
{
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel = CAN_RX_IRQ;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}
/*CAN报文内容设置*/
void CAN_SetMsg(CanTxMsg *TxMessage)
{
	u8 ubCounter = 0;
	if(Can_Reve_Flag!=1)
	{
	TxMessage->ExtId = 0X1314;
	TxMessage->IDE = CAN_ID_EXT;
	TxMessage->RTR = CAN_RTR_DATA;
	TxMessage->DLC = 8;
		for(ubCounter=0;ubCounter < 8;ubCounter++)
		{
			TxMessage->Data[ubCounter] = ubCounter;
		}
	}
	else 
	{
	TxMessage->ExtId = 0X13461234;
	TxMessage->IDE = CAN_ID_EXT;
	TxMessage->RTR = CAN_RTR_DATA;
	TxMessage->DLC = 8;
		for(ubCounter=0;ubCounter < 8;ubCounter++)
		{
			TxMessage->Data[ubCounter] = ubCounter;
		}
	}
	
}
static void CAN_Message_Analysis(void)
{
	return ;
}
static void CAN_Message_Send(void)
{
	return ;
}

void CAN_Transmit_data(void)
{
	CAN_SetMsg(&TxMessage);
}
void CANPro_Init(void)
{
	CAN_NVIC_Config();
	CAN_GPIO_Config();
	CAN_Mode_Config();
	CAN_Filter_Config(0x1314);
}

void CAN_Receive_data(void)
{
	CAN_Receive(CANx, CAN_FIFO0, &RxMessage);
	if((RxMessage.ExtId == 0X13141314)&&(RxMessage.IDE == CAN_ID_EXT)&&(RxMessage.DLC == 8))
	{
		Can_Reve_Flag = 1;
	}else {
		Can_Reve_Flag = 0;
	}
}
/*任务线程:can通讯*/
void can_thread_entry(void *parameter)
{
	while(1)
	{
		CAN_Message_Analysis();
		CAN_Message_Send();
		CAN_SetMsg(&TxMessage);
		CAN_Transmit(CANx,&TxMessage);
		rt_thread_delay(50);/* 延 时 50 个 systick*/
	}
}






























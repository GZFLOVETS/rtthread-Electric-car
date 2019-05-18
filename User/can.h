#ifndef __can_H
#define __can_H

#include "stm32f10x.h"
#include "stm32f10x_rcc.h"
#define CANx		CAN1
#define CAN_CLK		RCC_APB1Periph_CAN1
#define CAN_RX_IRQ	USB_LP_CAN1_RX0_IRQn
#define CAN_RX_IRQHandler	USB_LP_CAN1_RX0_IRQHandler


#define CAN_RX_PIN	GPIO_Pin_11
#define CAN_TX_PIN	GPIO_Pin_12
#define CAN_TX_GPIO_PORT	GPIOA
#define CAN_RX_GPIO_PORT	GPIOA
#define CAN_TX_GPIO_CLK		RCC_APB2Periph_GPIOA
#define CAN_RX_GPIO_CLK		RCC_APB2Periph_GPIOA

void CAN_Receive_data(void);
void CAN_Transmit_data(void);

void CANPro_Init(void);
void CAN_SetMsg(CanTxMsg *TxMessage);









#endif






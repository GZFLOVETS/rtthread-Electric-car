#ifndef __USART_H
#define __USART_H 

#include "stm32f10x.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_rcc.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "gpio.h"
#include "adc.h"

#define DEBUG_USARTx						USART1
#define USB_USARTx						USART1
#define USB_USART_CLK						RCC_APB2Periph_USART1
#define USB_USART_APBxClkCmd					RCC_APB2PeriphClockCmd
#define USB_USART_BAUDRATE					115200

#define USB_USART_GPIO_CLK					RCC_APB2Periph_GPIOA
#define USB_USART_GPIO_APBxClkCmd				RCC_APB2PeriphClockCmd

#define USB_USART_TX_GPIO_PORT				GPIOA
#define USB_USART_TX_GPIO_PIN					GPIO_Pin_9
#define USB_USART_RX_GPIO_PORT				GPIOA
#define USB_USART_RX_GPIO_PIN					GPIO_Pin_10

#define USB_USART_IRQ						USART1_IRQn
#define USB_USART_Handler					USART1_IRQHandler
#define USB_buffer_size						200
#define USB_memorypool_size					20
#define DMA_Define						1
#if DMA_Define
#define USB_RX_DMA_Channel					DMA1_Channel5
#define USB_DMA_BASE_ADDR					(u32)(&USART1->DR)

#define USB_TX_DMA_Channel					DMA1_Channel4
#endif

/*=================================flatbed define=======================================*/
#define AreaSize	128
/*数据拆分宏定义*/
#define BYTE0(dwTemp) (*(char*)(&dwTemp)+0)
#define BYTE1(dwTemp) (*(char*)(&dwTemp)+1)
#define BYTE2(dwTemp) (*(char*)(&dwTemp)+2)
#define BYTE3(dwTemp) (*(char*)(&dwTemp)+3)
/*定义帧头*/
#define command1_rev_addr	0x21
#define command10_send_addr	0x02
#define command21_send_addr	0x22
#define fralength			0x10
/*接收命令1的数据内容*/
#define command1Num			0x01
#define CBsendIntervalmax	0x3C
#define askmodemax			0x02

/*命令10的数据内容*/
#define command10Num		0x10
#define handshakeAddr		0x21
#define lordversionNum		0x00
#define boutversionNum		0x01
#define Timeday				0x06
#define Timemonth			0x03
#define Timeyear			0x11

/*命令21的数据内容*/
#define command21Num		0x21
/*===================================================================*/


typedef struct BUFF_ARRAY{
	u8 RxBuffer[USB_buffer_size];
	u8 Memory_Pool[USB_memorypool_size];
	u8 data_to_send[USB_memorypool_size];
	u16 USART_RX_STA;
}__attribute__((packed)) BUFF_ARRAY;




/*flatbed的发送数据结构体*/
typedef struct SendDetail{
	u8 CBstatus;
	u8 CBerror;
	u8 Lightstatus;
	u16 speedpulse;
	u8 loadstatus;
}__attribute__((packed)) SendDetail;



void USB_USART_Config(void);
void Usart_SendString(USART_TypeDef * pUSARTx,char *str);
void Usart_SendByte(USART_TypeDef * pUSARTx,uint8_t ch);
#if DMA_Define
void DMAget_rx_byte(void);
#endif















#endif


























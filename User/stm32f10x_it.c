/**
  ******************************************************************************
  * @file    Project/STM32F10x_StdPeriph_Template/stm32f10x_it.c 
  * @author  MCD Application Team
  * @version V3.5.0
  * @date    08-April-2011
  * @brief   Main Interrupt Service Routines.
  *          This file provides template for all exceptions handler and 
  *          peripherals interrupt service routine.
  ******************************************************************************
  * @attention
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTI
  
  AL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2011 STMicroelectronics</center></h2>
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x_it.h"
extern uint16_t ADC_ConvertedValue[ADC_ConvertedValueSize];
extern digitalSignal DigitalSignal;
extern BUFF_ARRAY buff_array;
/** @addtogroup STM32F10x_StdPeriph_Template
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/******************************************************************************/
/*            Cortex-M3 Processor Exceptions Handlers                         */
/******************************************************************************/

/**
  * @brief  This function handles NMI exception.
  * @param  None
  * @retval None
  */
void NMI_Handler(void)
{
}
#if	0
/**
  * @brief  This function handles Hard Fault exception.
  * @param  None
  * @retval None
  */
void HardFault_Handler(void)
{
  /* Go to infinite loop when Hard Fault exception occurs */
  while (1)
  {
  }
}
#endif
/**
  * @brief  This function handles Memory Manage exception.
  * @param  None
  * @retval None
  */
void MemManage_Handler(void)
{
  /* Go to infinite loop when Memory Manage exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Bus Fault exception.
  * @param  None
  * @retval None
  */
void BusFault_Handler(void)
{
  /* Go to infinite loop when Bus Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Usage Fault exception.
  * @param  None
  * @retval None
  */
void UsageFault_Handler(void)
{
  /* Go to infinite loop when Usage Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles SVCall exception.
  * @param  None
  * @retval None
  */
void SVC_Handler(void)
{
}

/**
  * @brief  This function handles Debug Monitor exception.
  * @param  None
  * @retval None
  */
void DebugMon_Handler(void)
{
}
#if	0
/**
  * @brief  This function handles PendSVC exception.
  * @param  None
  * @retval None
  */
void PendSV_Handler(void)
{
}

/**
  * @brief  This function handles SysTick Handler.
  * @param  None
  * @retval None
  */
void SysTick_Handler(void)
{
}
#endif
/******************************************************************************/
/*                 STM32F10x Peripherals Interrupt Handlers                   */
/*  Add here the Interrupt Handler for the used peripheral(s) (PPP), for the  */
/*  available peripheral interrupt handler's name please refer to the startup */
/*  file (startup_stm32f10x_xx.s).                                            */
/******************************************************************************/

/**
  * @brief  This function handles PPP interrupt request.
  * @param  None
  * @retval None
  */
/*void PPP_IRQHandler(void)
{
}*/

/**
  * @}
  */ 
/**
  * @brief  This function handles USART1Handler.
  * @param  None
  * @retval None
  */

void USART1_IRQHandler(void)
{
#if DMA_Define
	if(USART_GetITStatus(USB_USARTx,USART_IT_IDLE)!=RESET)
	{
		DMA_Cmd(USB_RX_DMA_Channel,DISABLE);
		DMA_ClearFlag(DMA1_FLAG_GL5);
		DMAget_rx_byte();
		USB_RX_DMA_Channel->CNDTR = USB_buffer_size;
		DMA_Cmd(USB_RX_DMA_Channel,ENABLE);

		USART_ReceiveData(USB_USARTx);
		USART_ClearITPendingBit(USB_USARTx, USART_IT_IDLE);
	}
#else
	u8 pTemp=0;
	if(USART_GetITStatus(USB_USARTx,USART_IT_RXNE)!=RESET)
	{
		pTemp=USART_ReceiveData(USB_USARTx);
		if((buff_array.USART_RX_STA&0X8000)==0)
		{
			if(buff_array.USART_RX_STA&0X4000)
			{
				if(pTemp!=0x0a) buff_array.USART_RX_STA=0;
				else buff_array.USART_RX_STA|=0x8000;
			}else{
				if(pTemp==0x0d) buff_array.USART_RX_STA|=0x4000;
				else {
					buff_array.RxBuffer[buff_array.USART_RX_STA&0X3FFF]=pTemp;
					buff_array.USART_RX_STA++;
					if(buff_array.USART_RX_STA>(USB_buffer_size-1)) buff_array.USART_RX_STA=0;
				}
			}
		}
	}
#endif
}

void EXTI15_10_IRQHandler(void)
{
	if(EXTI_GetITStatus(EXTI_Line11)==SET)
	{
		DigitalSignal.KeyStatus.sw1=1;
		EXTI_ClearITPendingBit(EXTI_Line11);
	}
}
/**
  * @brief  This function handles ADC_IRQHandler.
  * @param  None
  * @retval None
  */
void ADC1_2_IRQHandler(void)
{
	if(ADC_GetITStatus(ADC1,ADC_IT_EOC)==SET)
	{
		ADC_ClearITPendingBit(ADC1,ADC_IT_EOC);
	}
}

/**
  * @brief  This function handles CAN1_RX0_IRQHandler.
  * @param  None
  * @retval None
  */

void CAN_RX_IRQHandler(void)
{
	if(CAN_GetITStatus(CANx,CAN_IT_FMP0)!=RESET)
	{
		CAN_Receive_data();
		CAN_ClearITPendingBit(CANx, CAN_IT_FMP0);
	}
}
/**
  * @brief  This function handles TIM1_IRQ Handler.
  * @param  None
  * @retval None
  */

void TIM1_UP_IRQHandler(void)
{
	if(TIM_GetITStatus(TIM1,TIM_IT_Update) != RESET)
	{
		TIM_ClearITPendingBit(TIM1,TIM_IT_Update);
	}
}

/**
  * @brief  This function handles TIM2_IRQHandler.
  * @param  None
  * @retval None
  */

void TIM2_IRQHandler(void)
{
	if(TIM_GetITStatus(TIM2,TIM_IT_Update) != RESET)
	{
		TIM_ClearITPendingBit(TIM2,TIM_IT_Update);
	}
}

/**
  * @brief  This function handles TIM3_IRQHandler.
  * @param  None
  * @retval None
  */
void TIM3_IRQHandler(void)
{
	if(TIM_GetITStatus(TIM3,TIM_IT_Update) != RESET)
	{
		TIM_ClearITPendingBit(TIM3,TIM_IT_Update);
	}
}
/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/

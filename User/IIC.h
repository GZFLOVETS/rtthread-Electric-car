#ifndef __IIC_H
#define __IIC_H 

#include "stm32f10x.h"
#include "oled.h"

//-----------------OLED IIC�˿ڶ���----------------  					   
#define I2C_SDA_PIN 		GPIO_Pin_7
#define I2C_SCL_PIN		GPIO_Pin_6
#define I2C_SDA_PORT		GPIOB
#define I2C_SCL_PORT		GPIOB
#define OLED_SCLK_Clr() GPIO_ResetBits(I2C_SCL_PORT,I2C_SCL_PIN)//SCL IIC�ӿڵ�ʱ���ź�
#define OLED_SCLK_Set() GPIO_SetBits(I2C_SCL_PORT,I2C_SCL_PIN)

#define OLED_SDIN_Clr() GPIO_ResetBits(I2C_SDA_PORT,I2C_SDA_PIN)//SDA IIC�ӿڵ������ź�
#define OLED_SDIN_Set() GPIO_SetBits(I2C_SDA_PORT,I2C_SDA_PIN)

 		     
#define OLED_CMD  0	//д����
#define OLED_DATA 1	//д����

#if OLED_USER_0_96
#define IIC_SLAVE_ADDR 0x78  //IIC slave device address
#endif
#if OLED_USER_1_5
#define IIC_SLAVE_ADDR 0x78
#endif


void IIC_Start(void);
void IIC_Stop(void);
void Write_IIC_Command(unsigned char IIC_Command);
void Write_IIC_Data(unsigned char IIC_Data);
void Write_IIC_Byte(unsigned char IIC_Byte);
uint8_t I2C_CheckDevice(uint8_t _Address);
uint8_t IIC_Wait_Ack(void);
void IIC_Init(void);
void OLED_WR_Byte(unsigned char dat,unsigned char cmd);



























#endif

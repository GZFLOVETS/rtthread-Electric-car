//////////////////////////////////////////////////////////////////////////////////	 
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//测试硬件：单片机STM32F103ZET6,晶振72M  单片机工作电压3.3V或5V
//QDtech-OLED液晶驱动 for STM32
//xiao冯@ShenZhen QDtech co.,LTD
//公司网站:www.qdtech.net
//淘宝网站：http://qdtech.taobao.com
//我司提供技术支持，任何技术问题欢迎随时交流学习 
//邮箱:QDtech2008@gmail.com 
//Skype:QDtech2008
//技术交流QQ群:324828016
//创建日期:2018/6/6
//版本：V1.0
//版权所有，盗版必究。
//Copyright(C) 深圳市全动电子技术有限公司 2009-2019
//All rights reserved
/****************************************************************************************************
//=========================================电源接线================================================//
//      5V  接DC 5V电源
//     GND  接地
//======================================OLED屏数据线接线==========================================//
//本模块数据总线类型为IIC
//     SCL  接PB14   // IIC时钟信号
//     SDA  接PB13    // IIC数据信号
//======================================OLED屏控制线接线==========================================//
//本模块数据总线类型为IIC，不需要接控制信号线    
//=========================================触摸屏接线=========================================//
//本模块本身不带触摸，不需要接触摸屏线
//============================================================================================//
  * @attention
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, QD electronic SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
**************************************************************************************************/	
#ifndef __OLED_H
#define __OLED_H			  
#include "stm32f10x.h"
#include <stdlib.h>	
#include <stdio.h>
#include <string.h>
#include "gpio.h"
#include "oledfont.h"
#include "IIC.h"
#include "adc.h"
#include "rtthread.h"
#define OLED_USER_0_96 0	//0.96寸的屏
#define OLED_USER_1_5  1	//1.5寸的屏

#define OLED_MODE 0
#if OLED_USER_0_96
#define SIZE 8
#define XLevelL		0x00
#define XLevelH		0x10
#define Max_Column	128
#define Max_Row		64
#define Brightness	0xFF 
#define X_WIDTH 	128
#define Y_WIDTH 	64	   
#endif

#if OLED_USER_1_5
#define SIZE 8
#define XLevelL		0x00
#define XLevelH		0x10
#define Max_Column	128
#define Max_Row		128
#define Brightness	0xFF 
#define X_WIDTH 	128
#define Y_WIDTH 	128	   
#endif



//OLED控制用函数
static void OLED_Display_On(void);
static void OLED_Display_Off(void);	   							   		    
static void OLED_Init(void);
void OLED_Clear(unsigned dat);
static void OLED_DrawPoint(u8 x,u8 y,u8 t);
static void OLED_Fill(u16 sx,u16 sy,u16 ex,u16 ey,u16 color);
static void OLED_ShowChar(u8 x,u8 y,u8 chr,u8 Char_Size);
void OLED_ShowNum(u8 x,u8 y,u32 num,u8 len,u8 size);
void OLED_ShowString(u8 x,u8 y,u8 *chr,u8 Char_Size);	 
static void OLED_Set_Pos(unsigned char x, unsigned char y);
static void OLED_ShowCHinese(u8 x,u8 y,u8 no);
void OLED_On(void);
static void OLED_DrawBMP(unsigned char x0, unsigned char y0,unsigned char x1, unsigned char y1,unsigned char BMP[]);
static void Delay_50ms(unsigned int Del_50ms);
static void Delay_1ms(unsigned int Del_1ms);
static void fill_picture(unsigned char fill_Data);
static void Picture(void);

void oled_show(int keycount);

void OLED_ConfigInit(void);

#endif  
	 




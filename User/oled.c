//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//����Ӳ������Ƭ��STM32F103ZET6,����72M  ��Ƭ��������ѹ3.3V��5V
//QDtech-OLEDҺ������ for STM32
//xiao��@ShenZhen QDtech co.,LTD
//��˾��վ:www.qdtech.net
//�Ա���վ��http://qdtech.taobao.com
//��˾�ṩ����֧�֣��κμ������⻶ӭ��ʱ����ѧϰ 
//����:QDtech2008@gmail.com 
//Skype:QDtech2008
//��������QQȺ:324828016
//��������:2018/6/6
//�汾��V1.0
//��Ȩ���У�����ؾ���
//Copyright(C) ������ȫ�����Ӽ������޹�˾ 2009-2019
//All rights reserved
/****************************************************************************************************
//=========================================��Դ����================================================//
//      5V  ��DC 5V��Դ
//     GND  �ӵ�
//======================================OLED�������߽���==========================================//
//��ģ��������������ΪIIC
//     SCL  ��PB14    // IICʱ���ź�
//     SDA  ��PB13    // IIC�����ź�
//======================================OLED�������߽���==========================================//
//��ģ��������������ΪIIC������Ҫ�ӿ����ź���    
//=========================================����������=========================================//
//��ģ�鱾��������������Ҫ�Ӵ�������
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
#include "oled.h"


uint8_t sw1keycount=0;
extern digitalSignal DigitalSignal;
extern AD_VALUE ad_value;
const unsigned char  Chinese_text_16x16[] = 
{
	"ȫ�������ṩ���׽������Һ����ʾģ��ϵ�г�����ȫ�ӽǲ�Ʒ����������˫��ɫ�Է���͹��ĳ�����׼��������Ϊרע����רҵ����֧�ּ��Ҷ�"

};


#if OLED_MODE
/********************************************
// fill_Picture
********************************************/
static void fill_picture(unsigned char fill_Data)
{
	unsigned char m,n;
	for(m=0;m<8;m++)
	{
		OLED_WR_Byte(0xb0+m,0);		//page0-page1
		OLED_WR_Byte(0x00,0);		//low column start address
		OLED_WR_Byte(0x10,0);		//high column start address
		for(n=0;n<128;n++)
		{
			OLED_WR_Byte(fill_Data,1);
		}
	}
}
#endif


//��������
static void OLED_Set_Pos(unsigned char x, unsigned char y) 
{ 	
#if OLED_USER_0_96
	OLED_WR_Byte(0xb0+y,OLED_CMD);
	OLED_WR_Byte(((x&0xf0)>>4)|0x10,OLED_CMD);
	OLED_WR_Byte((x&0x0f),OLED_CMD); 
#endif
#if OLED_USER_1_5
	OLED_WR_Byte(0x75,OLED_CMD);
	OLED_WR_Byte(x,OLED_CMD);
	OLED_WR_Byte(x+7,OLED_CMD);

	OLED_WR_Byte(0x15,OLED_CMD);
	OLED_WR_Byte(y,OLED_CMD);
	OLED_WR_Byte(y,OLED_CMD);
#endif
}   	
#if OLED_MODE
//����OLED��ʾ    
static void OLED_Display_On(void)
{
	OLED_WR_Byte(0X8D,OLED_CMD);  //SET DCDC����
	OLED_WR_Byte(0X14,OLED_CMD);  //DCDC ON
	OLED_WR_Byte(0XAF,OLED_CMD);  //DISPLAY ON
}
//�ر�OLED��ʾ     
static void OLED_Display_Off(void)
{
	OLED_WR_Byte(0X8D,OLED_CMD);  //SET DCDC����
	OLED_WR_Byte(0X10,OLED_CMD);  //DCDC OFF
	OLED_WR_Byte(0XAE,OLED_CMD);  //DISPLAY OFF
}	
#endif
//��������,������,������Ļ�Ǻ�ɫ��!��û����һ��!!!	  
void OLED_Clear(unsigned dat)  
{  
#if OLED_USER_0_96
	u8 i,n;		    
	for(i=0;i<8;i++)  
	{  
		OLED_WR_Byte (0xb0+i,OLED_CMD);    //����ҳ��ַ��0~7��
		OLED_WR_Byte (0x00,OLED_CMD);      //������ʾλ�á��е͵�ַ
		OLED_WR_Byte (0x10,OLED_CMD);      //������ʾλ�á��иߵ�ַ   
		for(n=0;n<128;n++)OLED_WR_Byte(dat,OLED_DATA); 
	} //������ʾ
#endif
#if OLED_USER_1_5
	unsigned int i,j;
	OLED_WR_Byte(0x15,OLED_CMD);
	OLED_WR_Byte(0x00,OLED_CMD);
	OLED_WR_Byte(0x3f,OLED_CMD);

	OLED_WR_Byte(0x75,OLED_CMD);
	OLED_WR_Byte(0x00,OLED_CMD);
	OLED_WR_Byte(0x7f,OLED_CMD);

	for(i=0;i<128;i++) 
	{
		for(j=0;j<64;j++)
		{
		  OLED_WR_Byte(dat,OLED_DATA);//RAM data write
		}
	}
#endif

}
#if OLED_USER_1_5
//******************************************************************
//��������  OLED_SetWindows
//���ߣ�    xiao��@ȫ������
//���ڣ�    2018-07-23
//���ܣ�    ������ʾ����  
//���������xStart��yStart�������������
//          xEnd��yEnd�������յ�����
//����ֵ��  ��
//�޸ļ�¼����
//******************************************************************
#if	OLED_MODE
static void OLED_SetWindows(u16 xStar, u16 yStar,u16 xEnd,u16 yEnd)
{
	OLED_WR_Byte(0x15,OLED_CMD);
	OLED_WR_Byte(xStar,OLED_CMD);
	OLED_WR_Byte(xEnd,OLED_CMD);

	OLED_WR_Byte(0x75,OLED_CMD);
	OLED_WR_Byte(yStar,OLED_CMD);
	OLED_WR_Byte(yEnd,OLED_CMD);
}
#endif
//******************************************************************
//��������  LCD_SetContrast
//���ߣ�    xiao��@ȫ������
//���ڣ�    2018-07-23
//���ܣ�    ��������Ҷ�ֵ  
//���������arr���Ҷ�ֵ
//����ֵ��  ��
//�޸ļ�¼����
//******************************************************************
#if	OLED_MODE
static void LCD_SetContrast(u16 arr)
{
	 OLED_WR_Byte(0x81,OLED_CMD);  // set contrast control
   OLED_WR_Byte(arr,OLED_CMD);
}
#endif
//******************************************************************
//��������  OLED_transfer_col_data
//���ߣ�    xiao��@ȫ������
//���ڣ�    2018-07-23
//���ܣ�    ���õ�������ɫֵ	  
//���������data1����һ������ɫ����
//          data2���ڶ�������ɫ����
//����ֵ��  ��
//�޸ļ�¼����
//****************************************************************** 
static void OLED_transfer_col_data(unsigned char data1,unsigned char data2)
{
  unsigned char i; 
	 	for(i=0;i<8;i++)
	{
          if ((data1& 0x01)&&(data2& 0x01))	
		    { 	
		  			OLED_WR_Byte(0xFF,OLED_DATA);              //4BIT��ʾ���ȣ���������0~F��16���Ҷ�
							//LCD_WR_DATA(0xF8);              //4BIT��ʾ���ȣ���������0~F��16���Ҷ�
			  }
        else  if  ((data1& 0x01)==1&&(data2& 0x01)==0)				
		    {
			       
              OLED_WR_Byte(0xF0,OLED_DATA);   
        }
        else  if  ((data1& 0x01)==0&&(data2& 0x01)==1)				
		    {
			       
              OLED_WR_Byte(0x0F,OLED_DATA);  
        }
        else  if  ((data1& 0x01)==0&&(data2& 0x01)==0)				
		    {
			       
              OLED_WR_Byte(0x00,OLED_DATA);  
        }
 
        data1>>= 1;
        data2>>= 1;
    }
}
//******************************************************************
//��������  OLED_transfer_col_data2
//���ߣ�    xiao��@ȫ������
//���ڣ�    2018-07-23
//���ܣ�    ���õ�������ɫֵ	  
//���������data1����һ������ɫ����
//          data2���ڶ�������ɫ����
//					gray_value���Ҷ�ֵ
//����ֵ��  ��
//�޸ļ�¼����
//****************************************************************** 
static void OLED_transfer_col_data2(unsigned char data1,unsigned char data2,unsigned char gray_value)
{
  unsigned char i; 
	 	for(i=0;i<8;i++)
	{
          if ((data1& 0x01)&&(data2& 0x01))	
		    { 	
						OLED_WR_Byte((gray_value<<4)|gray_value,OLED_DATA);//4BIT��ʾ���ȣ���������0~F��16���Ҷ�		  			            
			  }
        else  if  ((data1& 0x01)==1&&(data2& 0x01)==0)				
		    {
			       OLED_WR_Byte((gray_value<<4)&0xF0,OLED_DATA); 
        }
        else  if  ((data1& 0x01)==0&&(data2& 0x01)==1)				
		    {
			       OLED_WR_Byte((gray_value)&0x0F,OLED_DATA);   
        }
        else  if  ((data1& 0x01)==0&&(data2& 0x01)==0)				
		    {
			       
              OLED_WR_Byte(0x00,OLED_DATA);  
        }
        data1>>= 1;
        data2>>= 1;
    } 
}
//******************************************************************
//��������  display_full_gray
//���ߣ�    xiao��@ȫ������
//���ڣ�    2018-07-23
//���ܣ�    ��ʾȫ���Ҷ�ֵ  
//���������data1���Ҷ�ֵ
//����ֵ��  ��
//�޸ļ�¼����
//****************************************************************** 
#if	OLED_MODE
static void display_full_gray(u8 data1)
{
  int i,j;
  OLED_WR_Byte(0x15,OLED_CMD);
  OLED_WR_Byte(0x00,OLED_CMD);
  OLED_WR_Byte(0x3f,OLED_CMD);

  OLED_WR_Byte(0x75,OLED_CMD);
  OLED_WR_Byte(0x00,OLED_CMD);
  OLED_WR_Byte(0x7f,OLED_CMD);
  for(i=0;i<128;i++) 
  {
    for(j=0;j<64;j++)
     {
      OLED_WR_Byte(data1,OLED_DATA);
     }
  }
}
#endif
//******************************************************************
//��������  OLED_draw_point
//���ߣ�    xiao��@ȫ������
//���ڣ�    2018-07-23
//���ܣ�    ����  
//���������x,y��������
//            d����ɫֵ
//����ֵ��  ��
//�޸ļ�¼����
//******************************************************************
void OLED_draw_point(u8 x,u8 y,u8 d)
{
	OLED_Set_Pos(y, x);
	OLED_WR_Byte(d,OLED_DATA);
}
//******************************************************************
//��������  OLED_draw_line
//���ߣ�    xiao��@ȫ������
//���ڣ�    2018-07-23
//���ܣ�    ����
//���������x1,y1���������
//          x2,y2���յ�����
//����ֵ��  ��
//�޸ļ�¼����
//******************************************************************
void OLED_draw_line(u8 x1, u8 y1, u8 x2, u8 y2)
{
	u16 t; 
	int xerr=0,yerr=0,delta_x,delta_y,distance; 
	int incx,incy,uRow,uCol; 
  u8 color;
	delta_x=x2-x1; //������������ 
	delta_y=y2-y1; 
	uRow=x1; 
	uCol=y1; 
	if(delta_x>0)incx=1; //���õ������� 
	else if(delta_x==0)incx=0;//��ֱ�� 
	else {incx=-1;delta_x=-delta_x;} 
	if(delta_y>0)incy=1; 
	else if(delta_y==0)incy=0;//ˮƽ�� 
	else{incy=-1;delta_y=-delta_y;} 
	if( delta_x>delta_y)distance=delta_x; //ѡȡ�������������� 
	else distance=delta_y; 
	
	for(t=0;t<=distance+1;t++ )//������� 
	{  
	xerr+=delta_x ; 
		yerr+=delta_y ; 
		if(xerr>distance) //����
		{ 
			xerr-=distance; 
			color=0xFF;
			OLED_draw_point(uRow,uCol,color);//���� 
			uRow+=incx; 
			
		} 
		if(yerr>distance) //����
		{ 
			yerr-=distance; 
			color=0xF0;
			OLED_draw_point(uRow,uCol,color);//���� 
			uCol+=incy; 
			/*
			color=0x00;
			GUI_Draw_2_pixel(uRow,uCol/2,color);//���� 
			uCol+=incy; */
			
		} 	
	}  
} 
//******************************************************************
//��������  OLED_draw_rectangle
//���ߣ�    xiao��@ȫ������
//���ڣ�    2018-07-23
//���ܣ�    ������
//���������x1,y1�������������
//          x2,y2�������յ�����
//����ֵ��  ��
//�޸ļ�¼����
//******************************************************************
void OLED_draw_rectangle(u16 x1, u16 y1, u16 x2, u16 y2)
{
	OLED_draw_line(x1,y1,x2,y1);
	OLED_draw_line(x1,y1+1,x1,y2);
	OLED_draw_line(x1,y2,x2,y2);
	OLED_draw_line(x2,y1,x2,y2);
}
//******************************************************************
//��������  OLED_Fill
//���ߣ�    xiao��@ȫ������
//���ڣ�    2018-07-23
//���ܣ�    ������
//���������sx,sy�������������
//          ex,ey�������յ�����
//          color����ɫֵ
//����ֵ��  ��
//�޸ļ�¼����
//******************************************************************
#if OLED_MODE
static void OLED_Fill(u16 sx,u16 sy,u16 ex,u16 ey,u16 color)
{  	

	u16 i,j;			
	u16 width=ex/2-sx/2+1; 		//�õ����Ŀ��
	u16 height=ey-sy+1;		//�߶�
	OLED_SetWindows(sx/2,sy,(ex-1)/2,ey-1);//������ʾ����
	for(i=0;i<height;i++)
	{
		for(j=0;j<width;j++)
		 OLED_WR_Byte(color,OLED_DATA);	//д������ 	 
	}

//	OLED_SetWindows(0,0,lcddev.width-1,lcddev.height-1);//�ָ���������Ϊȫ��
}
#endif
void _draw_circle_8(int xc, int yc, int x, int y, u16 c)
{
	OLED_draw_point(xc + x, yc + y, c);

	OLED_draw_point(xc - x, yc + y, c);

	OLED_draw_point(xc + x, yc - y, c);

	OLED_draw_point(xc - x, yc - y, c);

	OLED_draw_point(xc + y, yc + x, c);

	OLED_draw_point(xc - y, yc + x, c);

	OLED_draw_point(xc + y, yc - x, c);

	OLED_draw_point(xc - y, yc - x, c);
}
//******************************************************************
//��������  gui_circle
//���ߣ�    xiao��@ȫ������
//���ڣ�    2018-07-23
//���ܣ�    ��ָ��λ�û�һ��ָ����С��Բ(���)
//���������(xc,yc) :Բ��������
//         	c:������ɫ
//		 	r:Բ�뾶
//		 	fill:����жϱ�־��1-��䣬0-�����
//����ֵ��  ��
//�޸ļ�¼����
//******************************************************************  
void OLED_gui_circle(int xc, int yc,u16 c,int r, int fill)
{
	int x = 0, y = r, yi, d;

	d = 3 - 2 * r;


	if (fill) 
	{
		// �����䣨��ʵ��Բ��
		while (x <= y) {
			for (yi = x; yi <= y; yi++)
				_draw_circle_8(xc, yc, x, yi, c);

			if (d < 0) {
				d = d + 4 * x + 6;
			} else {
				d = d + 4 * (x - y) + 10;
				y--;
			}
			x++;
		}
	} else 
	{
		// �������䣨������Բ��
		while (x <= y) {
			_draw_circle_8(xc, yc, x, y, c);
			if (d < 0) {
				d = d + 4 * x + 6;
			} else {
				d = d + 4 * (x - y) + 10;
				y--;
			}
			x++;
		}
	}
}
//******************************************************************
//��������  OLED_display_string_5x8
//���ߣ�    xiao��@ȫ������
//���ڣ�    2018-07-23
//���ܣ�    ��ʾ�ַ���Ϊ5����Ϊ8���ַ���  
//���������column������ʾ�ַ�����������
//          row������ʾ�ַ�����������
//          text������ʾ�ַ�����ָ��
//          reverse��0-������ʾ��1-��ɫ��ʾ
//����ֵ��  ��
//�޸ļ�¼����
//****************************************************************** 
void OLED_display_string_5x8(unsigned char column,unsigned char row,unsigned char reverse,unsigned char *text)
{
	unsigned int i=0,j,k,disp_data1,disp_data2;
	while(text[i]>0x00)
	{	
		if((text[i]>=0x20)&&(text[i]<=0x7e))
		{
			j=text[i]-0x20;

						for(k=0;k<2;k++)
						{
									if(reverse==1)
									{
										disp_data1=~ascii_table_5x8[j][2*k];
										disp_data2=~ascii_table_5x8[j][2*k+1];
									}
									else
									{
										 disp_data1=ascii_table_5x8[j][2*k];
										 disp_data2=ascii_table_5x8[j][2*k+1];
									}
									OLED_Set_Pos(row,column+k);
									OLED_transfer_col_data(disp_data1,disp_data2);
						}
						OLED_Set_Pos(row,column+2);

             if(reverse==1)
						{
									disp_data1=~ascii_table_5x8[j][4];
									disp_data2=0xff;
						}
						else
						{
							 disp_data1=ascii_table_5x8[j][4];
               disp_data2=0x00;                    //д��һ�пհ��У�ʹ��5x8���ַ����ַ�֮����һ�м����������
						}  
           OLED_transfer_col_data(disp_data1,disp_data2);
			i++;
			column+=3;
			if(column>63)
			{
				column=0;
	
			}
		}
		else
		i++;
	}
}
//******************************************************************
//��������  OLED_display_string_8x16
//���ߣ�    xiao��@ȫ������
//���ڣ�    2018-07-23
//���ܣ�    ��ʾ�ַ���Ϊ8����Ϊ16���ַ���  
//���������column������ʾ�ַ�����������
//          row������ʾ�ַ�����������
//          text������ʾ�ַ�����ָ��
//����ֵ��  ��
//�޸ļ�¼����
//****************************************************************** 
void OLED_display_string_8x16(u8 column,u8 row,u8 *text)
{
	u16 i=0,j,k,n;
	if(column>63)
	{
		column=0;

	}
	while(text[i]>0x00)
	{	
		if((text[i]>=0x20)&&(text[i]<=0x7e))
		{
			j=text[i]-0x20;
			for(n=0;n<2;n++)
			{			
				for(k=0;k<4;k++)   //����128�㣬ʵ��ֻ��64���У�ÿһ�п���2����
				{					
				OLED_Set_Pos(n*8+row,column+k);
				OLED_transfer_col_data(ascii_table_8x16[j][2*k+8*n],ascii_table_8x16[j][2*k+1+8*n]);	//д���ݵ�LCD  
				}
			}
			i++;
			column+=4;
		}
		else
		i++;
	}
}
//******************************************************************
//��������  OLED_display_string_12x24
//���ߣ�    xiao��@ȫ������
//���ڣ�    2018-07-23
//���ܣ�    ��ʾ�ַ���Ϊ12����Ϊ24���ַ���  
//���������column������ʾ�ַ�����������
//          row������ʾ�ַ�����������
//          text������ʾ�ַ�����ָ��
//����ֵ��  ��
//�޸ļ�¼����
//****************************************************************** 
void OLED_display_string_12x24(u8 column,u8 row,u8 *text)
{
	u16 i=0,j,k,n;
	if(column>63)
	{
		column=0;

	}
	while(text[i]>0x00)
	{	
		if((text[i]>=0x30)&&(text[i]<=0x39))
		{
			j=text[i]-0x30;
			for(n=0;n<3;n++)
			{			
				for(k=0;k<6;k++)   //����128�㣬ʵ��ֻ��64���У�ÿһ�п���2����
				{					
					OLED_Set_Pos(n*8+row,column+k);
				OLED_transfer_col_data(code_12x24[j][2*k+12*n],code_12x24[j][2*k+1+12*n]);	//д���ݵ�LCD  
				}
			}
			i++;
			column+=6;
		}
		else
		i++;
	}
}

//******************************************************************
//��������  OLED_display_string_16x32
//���ߣ�    xiao��@ȫ������
//���ڣ�    2018-07-23
//���ܣ�    ��ʾ�ַ���Ϊ16����Ϊ32���ַ���  
//���������column������ʾ�ַ�����������
//          row������ʾ�ַ�����������
//          text������ʾ�ַ�����ָ��
//����ֵ��  ��
//�޸ļ�¼����
//****************************************************************** 
void OLED_display_string_16x32(u8 column,u8 row,u8 *text)
{
	u16 i=0,j,k,n;
	if(column>63)
	{
		column=0;

	}
	while(text[i]>0x00)
	{	
		if((text[i]>=0x30)&&(text[i]<=0x39))
		{
			j=text[i]-0x30;
			for(n=0;n<4;n++)
			{			
				for(k=0;k<8;k++)   //����128�㣬ʵ��ֻ��64���У�ÿһ�п���2����
				{					
					OLED_Set_Pos(n*8+row,column+k);
				OLED_transfer_col_data(code_16x32[j][2*k+16*n],code_16x32[j][2*k+1+16*n]);	//д���ݵ�LCD  
				}
			}
			i++;
			column+=8;
		}
		else
		i++;
	}
}

//******************************************************************
//��������  OLED_display_string_16x16
//���ߣ�    xiao��@ȫ������
//���ڣ�    2018-07-23
//���ܣ�    ��ʾ�ַ���Ϊ16����Ϊ16���ַ���  
//���������column������ʾ�ַ�����������
//          row������ʾ�ַ�����������
//          text������ʾ�ַ�����ָ��
//����ֵ��  ��
//�޸ļ�¼����
//****************************************************************** 
void OLED_display_string_16x16(u8 column,u8 row,u8 *text)
{
    u8 i,j,k;
    u32 address; 
    
    j = 0;
    while(text[j] != '\0')
    {
        i = 0;
        address = 1;
        while(Chinese_text_16x16[i] > 0x7e)	  // >0x7f��˵������ASCII���ַ�
        {
            if(Chinese_text_16x16[i] == text[j])
            {
                if(Chinese_text_16x16[i + 1] == text[j + 1])
                {
                    address = i * 16;
                    break;
                }
            }
            i += 2;            
        }
        
        if(column >63)
        {
            column = 0;

        }
        
        if(address != 1)// ��ʾ����                   
        {
            
						for(k=0;k<2;k++)
						{
						
										for(i = 0; i < 8; i++)               
										{
									
                        OLED_Set_Pos(k*8+row,column+i);						       
                        OLED_transfer_col_data(Chinese_code_16x16[address],Chinese_code_16x16[address+1]);          								
											  address+=2;
											//delay_ms(10);
										}
										
						}
									j += 2;        
        }
        else              //û�����ֿ�ĺ��֣�����ʾ�հ��ַ�            
        {
						for(k=0;k<2;k++)
						{
						
										for(i = 0; i < 8; i++)      //8��col�൱��16����         
										{
											  OLED_Set_Pos(k*8+row,column+i);
									      OLED_transfer_col_data(0x00,0x00);   
										}
						}
									
									j++;
        }
        
        column+=7;
    }
}

//******************************************************************
//��������  OLED_disp_string_8x16_16x16
//���ߣ�    xiao��@ȫ������
//���ڣ�    2018-07-23
//���ܣ�    ��ʾ�ַ��ͺ��ֻ���ַ����������ַ����Ϊ8���߶�Ϊ16�����ֿ��Ϊ16���߶�Ϊ16 
//���������column������ʾ�ַ�������ʼ������
//          row������ʾ�ַ�������ʼ������
//          text������ʾ�ַ�����ָ��
//����ֵ��  ��
//�޸ļ�¼����
//****************************************************************** 
void OLED_disp_string_8x16_16x16(u8 column,u8 row,u8 *text)
{
    u8 temp[3];
    u8 i = 0;    
    
    while(text[i] != '\0')
    {
        if(text[i] > 0x7e)
        {
            temp[0] = text[i];
            temp[1] = text[i + 1];
            temp[2] = '\0';          //����Ϊ�����ֽ���
            OLED_display_string_16x16(column,row,temp);  //��ʾ����
            column += 8;
            i += 2;
        }
        else
        {
            temp[0] = text[i];    
            temp[1] = '\0';          //��ĸռһ���ֽ�
            OLED_display_string_8x16(column, row, temp);  //��ʾ��ĸ
            column += 4;             //4��8������
            i++;
        }
    }
}

u8 grayH= 0xF0;
u8   grayL= 0x0F;
#define pgm_read_byte(STR) STR

//******************************************************************
//��������  OLED_put_Char
//���ߣ�    xiao��@ȫ������
//���ڣ�    2018-07-23
//���ܣ�    ��ʾһ���ַ�
//���������C������ʾ���ַ�
//����ֵ��  ��
//�޸ļ�¼����
//****************************************************************** 
void OLED_put_Char(unsigned char C)
{
	char c,bit1,bit2;
	char i,j;
    if(C < 32 || C > 127) //Ignore non-printable ASCII characters. This can be modified for multilingual font.
    {
        C=' '; //Space
    }   

    
    for(i=0;i<8;i=i+2)
    {
        for(j=0;j<8;j++)
        {
            // Character is constructed two pixel at a time using vertical mode from the default 8x8 font
            c=0x00;
            bit1=((BasicFont[C-32][i])>>j)  & 0x01;  
            bit2=((BasicFont[C-32][i+1])>>j) & 0x01;
           // Each bit is changed to a nibble
            c|=(bit1)?grayH:0x00;
            c|=(bit2)?grayL:0x00;
            OLED_WR_Byte(c,OLED_DATA);
        }
    }
}

//******************************************************************
//��������  OLED_put_String
//���ߣ�    xiao��@ȫ������
//���ڣ�    2018-07-23
//���ܣ�    ��ʾ�ַ���
//���������String������ʾ���ַ���
//����ֵ��  ��
//�޸ļ�¼����
//****************************************************************** 
void OLED_put_String(const char *String)
{
    unsigned char i=0;
    while(String[i])
    {
        OLED_put_Char(String[i]);     
        i++;
    }
}

//******************************************************************
//��������  OLED_disp_pic_BMP
//���ߣ�    xiao��@ȫ������
//���ڣ�    2018-07-23
//���ܣ�    ��ʾBMP��ɫͼƬ
//���������x,y������ʾ��BMPͼƬ��ʼ����
//          pic������ʾ��BMPͼƬ����������ָ��
//          value���Ҷ�ֵ
//          len������ʾ��BMPͼƬ���������鳤��
//����ֵ��  ��
//�޸ļ�¼����
//****************************************************************** 
void OLED_disp_pic_BMP(u8 x, u8 y,const unsigned char *pic,char value,u32 len)
{ 
	u32 i=0,k=0,address=0;
	for(k=0;k<16;k++)
	{
		for(i = 0; i < 64; i++) 
		{		
			OLED_Set_Pos(k*8+y,i+x);
			//transfer_col_data(gImage_21[address],gImage_21[address+1]); 
			OLED_transfer_col_data2(pic[address],pic[address+1],value);  
			address+=2;
			if(address>=len)
			{
				return;
			}
		}
	}
}	

//******************************************************************
//��������  OLED_modify_num
//���ߣ�    xiao��@ȫ������
//���ڣ�    2018-07-23
//���ܣ�    �޸�UI������ʾ������
//���������x,y�����޸ĵ���������
//          num�����޸ĵ�����ֵ
//          type�����ִ�С
//����ֵ��  ��
//�޸ļ�¼����
//****************************************************************** 
void OLED_modify_num(u8 x, u8 y,u32 num ,u8 type)
{
	char buf[32]= {0};
	sprintf(buf,"%d ",num);
	if(type == 8)
	{
		OLED_display_string_5x8(x,y,0,(u8 *)buf);
	}
	else if(type == 16)
	{
		OLED_display_string_8x16(x,y,(u8 *)buf);
	}
	else if(type == 24)
	{
		OLED_display_string_12x24(x,y,(u8 *)buf);
	}
	else if(type == 32)
	{
		OLED_display_string_16x32(x,y,(u8 *)buf);
	}
}

//******************************************************************
//��������  OLED_show_run_page
//���ߣ�    xiao��@ȫ������
//���ڣ�    2018-07-23
//���ܣ�    ��ʾUI����
//�����������
//����ֵ��  ��
//�޸ļ�¼����
//****************************************************************** 
void OLED_show_run_page(void)
{
	OLED_draw_line(5, 37, 59, 37);
	OLED_draw_line(5, 84, 59, 84);
	OLED_draw_line(25, 38, 25, 83);
	OLED_display_string_5x8(7,20,0,"DEPTH");
	OLED_display_string_16x32(30,5,"100");
	OLED_display_string_5x8(54,20,0,"ft");
	OLED_display_string_12x24(6,54,"25");
	OLED_display_string_5x8(18,58,0,"18");
	OLED_display_string_5x8(28,48,0,"NEXT");
	OLED_display_string_5x8(47,48,0,"STOP");
	OLED_display_string_8x16(27,59,"115");
//	OLED_display_string_5x8(38,64,0,"ft");
	OLED_disp_string_8x16_16x16(44,59,"3:10");
	OLED_display_string_5x8(7,95,0,"CCVME");
	OLED_display_string_5x8(36,95,0,"NDL");
	OLED_display_string_5x8(50,95,0,"TTS");
	OLED_disp_string_8x16_16x16(7,105,"CC 18");
	OLED_disp_string_8x16_16x16(38,105,"0");
	OLED_disp_string_8x16_16x16(48,105,"148");
}

#endif

void OLED_On(void)  
{  
	u8 i,n;		    
	for(i=0;i<8;i++)  
	{  
		OLED_WR_Byte (0xb0+i,OLED_CMD);    //����ҳ��ַ��0~7��
		OLED_WR_Byte (0x00,OLED_CMD);      //������ʾλ�á��е͵�ַ
		OLED_WR_Byte (0x10,OLED_CMD);      //������ʾλ�á��иߵ�ַ   
		for(n=0;n<128;n++)OLED_WR_Byte(1,OLED_DATA); 
	} //������ʾ
}
//��ָ��λ����ʾһ���ַ�,���������ַ�
//x:0~127
//y:0~63
//mode:0,������ʾ;1,������ʾ
//size:ѡ������ 16/12 
static void OLED_ShowChar(u8 x,u8 y,u8 chr,u8 Char_Size)
{      	
	unsigned char c=0,i=0;	
		c=chr-' ';//�õ�ƫ�ƺ��ֵ			
		if(x>Max_Column-1){x=0;y=y+2;}
		if(Char_Size ==16)
			{
			OLED_Set_Pos(x,y);	
			for(i=0;i<8;i++)
			OLED_WR_Byte(F8X16[c*16+i],OLED_DATA);
			OLED_Set_Pos(x,y+1);
			for(i=0;i<8;i++)
			OLED_WR_Byte(F8X16[c*16+i+8],OLED_DATA);
			}
			else {	
				OLED_Set_Pos(x,y);
				for(i=0;i<6;i++)
				OLED_WR_Byte(F6x8[c][i],OLED_DATA);
				
			}
}
//m^n����
static u32 oled_pow(u8 m,u8 n)
{
	u32 result=1;	 
	while(n--)result*=m;    
	return result;
}				  
//��ʾ2������
//x,y :�������	 
//len :���ֵ�λ��
//size:�����С
//mode:ģʽ	0,���ģʽ;1,����ģʽ
//num:��ֵ(0~4294967295);	 		  
 void OLED_ShowNum(u8 x,u8 y,u32 num,u8 len,u8 size2)
{         	
	
	u8 t,temp;
	u8 enshow=0;						   
	for(t=0;t<len;t++)
	{
		temp=(num/oled_pow(10,len-t-1))%10;
		if(enshow==0&&t<(len-1))
		{
			if(temp==0)
			{
				OLED_ShowChar(x+(size2/2)*t,y,' ',size2);
				continue;
			}else enshow=1; 
		 	 
		}
	 	OLED_ShowChar(x+(size2/2)*t,y,temp+'0',size2); 
	}
} 
//��ʾһ���ַ��Ŵ�
 void OLED_ShowString(u8 x,u8 y,u8 *chr,u8 Char_Size)
{
	unsigned char j=0;
	while (chr[j]!='\0')
	{		OLED_ShowChar(x,y,chr[j],Char_Size);
			x+=8;
		if(x>120){x=0;y+=2;}
			j++;
	}
}
#if OLED_MODE
//��ʾ����
static void OLED_ShowCHinese(u8 x,u8 y,u8 no)
{      			    
	u8 t,adder=0;
	OLED_Set_Pos(x,y);
    for(t=0;t<16;t++)
		{
				OLED_WR_Byte(Hzk[2*no][t],OLED_DATA);
				adder+=1;
     }	
		OLED_Set_Pos(x,y+1);	
    for(t=0;t<16;t++)
			{	
				OLED_WR_Byte(Hzk[2*no+1][t],OLED_DATA);
				adder+=1;
      }					
}
#endif
#if OLED_MODE
/***********������������ʾ��ʾBMPͼƬ128��64��ʼ������(x,y),x�ķ�Χ0��127��yΪҳ�ķ�Χ0��7*****************/
static void OLED_DrawBMP(unsigned char x0, unsigned char y0,unsigned char x1, unsigned char y1,unsigned char BMP[])
{ 	
 unsigned int j=0;
 unsigned char x,y;
  
  if(y1%8==0) y=y1/8;      
  else y=y1/8+1;
	for(y=y0;y<y1;y++)
	{
		OLED_Set_Pos(x0,y);
    for(x=x0;x<x1;x++)
	    {      
	    	OLED_WR_Byte(BMP[j++],OLED_DATA);	    	
	    }
	}
} 
#endif

//��ʼ��SSD1306					    
static void OLED_Init(void)
{ 
#if OLED_USER_0_96
	OLED_WR_Byte(0xAE,OLED_CMD);//--display off
	OLED_WR_Byte(0x00,OLED_CMD);//---set low column address
	OLED_WR_Byte(0x10,OLED_CMD);//---set high column address
	OLED_WR_Byte(0x40,OLED_CMD);//--set start line address  
	OLED_WR_Byte(0xB0,OLED_CMD);//--set page address
	OLED_WR_Byte(0x81,OLED_CMD); // contract control
	OLED_WR_Byte(0xFF,OLED_CMD);//--128   
	OLED_WR_Byte(0xA1,OLED_CMD);//set segment remap 
	OLED_WR_Byte(0xA6,OLED_CMD);//--normal / reverse
	OLED_WR_Byte(0xA8,OLED_CMD);//--set multiplex ratio(1 to 64)
	OLED_WR_Byte(0x3F,OLED_CMD);//--1/32 duty
	OLED_WR_Byte(0xC8,OLED_CMD);//Com scan direction
	OLED_WR_Byte(0xD3,OLED_CMD);//-set display offset
	OLED_WR_Byte(0x00,OLED_CMD);//
	
	OLED_WR_Byte(0xD5,OLED_CMD);//set osc division
	OLED_WR_Byte(0x80,OLED_CMD);//
	
	OLED_WR_Byte(0xD8,OLED_CMD);//set area color mode off
	OLED_WR_Byte(0x05,OLED_CMD);//
	
	OLED_WR_Byte(0xD9,OLED_CMD);//Set Pre-Charge Period
	OLED_WR_Byte(0xF1,OLED_CMD);//
	
	OLED_WR_Byte(0xDA,OLED_CMD);//set com pin configuartion
	OLED_WR_Byte(0x12,OLED_CMD);//
	
	OLED_WR_Byte(0xDB,OLED_CMD);//set Vcomh
	OLED_WR_Byte(0x30,OLED_CMD);//
	
	OLED_WR_Byte(0x8D,OLED_CMD);//set charge pump enable
	OLED_WR_Byte(0x14,OLED_CMD);//
	
	OLED_WR_Byte(0xAF,OLED_CMD);//--turn on oled panel
#endif
#if OLED_USER_1_5
   OLED_WR_Byte(0xae,OLED_CMD);     //display off
   OLED_WR_Byte(0x15,OLED_CMD);    //   set column address
   OLED_WR_Byte(0x00,OLED_CMD);    //  start column   0
   OLED_WR_Byte(0x7f,OLED_CMD);    //  end column   127
   OLED_WR_Byte(0x75,OLED_CMD);    //   set row address
   OLED_WR_Byte(0x00,OLED_CMD);    //  start row   0
   OLED_WR_Byte(0x7f,OLED_CMD);    //  end row   127
   OLED_WR_Byte(0x81,OLED_CMD);  // set contrast control
   OLED_WR_Byte(0x80,OLED_CMD);
   OLED_WR_Byte(0xa0,OLED_CMD);    // gment remap
   OLED_WR_Byte(0x51,OLED_CMD);   //
   OLED_WR_Byte(0xa1,OLED_CMD);  // start line
   OLED_WR_Byte(0x00,OLED_CMD);
   OLED_WR_Byte(0xa2,OLED_CMD);  // display offset
   OLED_WR_Byte(0x00,OLED_CMD);
   OLED_WR_Byte(0xa4,OLED_CMD);    // rmal display
   OLED_WR_Byte(0xa8,OLED_CMD);    // set multiplex ratio
   OLED_WR_Byte(0x7f,OLED_CMD); 
   OLED_WR_Byte(0xb1,OLED_CMD);  // set phase leghth
   OLED_WR_Byte(0xf1,OLED_CMD);
   OLED_WR_Byte(0xb3,OLED_CMD);  // set dclk
   OLED_WR_Byte(0x00,OLED_CMD);  //80Hz:0xc1 90Hz:0xe1   100Hz:0x00   110Hz:0x30 120Hz:0x50   130Hz:0x70     01
   OLED_WR_Byte(0xab,OLED_CMD);  // 
   OLED_WR_Byte(0x01,OLED_CMD);  //
   OLED_WR_Byte(0xb6,OLED_CMD);  // set phase leghth
   OLED_WR_Byte(0x0f,OLED_CMD);
   OLED_WR_Byte(0xbe,OLED_CMD);
   OLED_WR_Byte(0x0f,OLED_CMD);
   OLED_WR_Byte(0xbc,OLED_CMD);
   OLED_WR_Byte(0x08,OLED_CMD);
   OLED_WR_Byte(0xd5,OLED_CMD);
   OLED_WR_Byte(0x62,OLED_CMD);  
   OLED_WR_Byte(0xfd,OLED_CMD);
   OLED_WR_Byte(0x12,OLED_CMD);
   OLED_WR_Byte(0xaf,OLED_CMD); 
#endif
}



void oled_show(int keycount)
{
#if OLED_USER_0_96
	switch(keycount)
	{
	case 0:
	OLED_ShowString(0,0,"lek:",16);//��ת�򿪹�
		OLED_ShowNum(50,0,Task1.KeyStatus.leftturnkey,4,16);
		
		OLED_ShowString(0,2,"rik",16);//��ת�򿪹�
		OLED_ShowNum(50,2,Task1.KeyStatus.rightturnkey,4,16);
		
		OLED_ShowString(0,4,"fak",16);//Զ�⿪��
		OLED_ShowNum(50,4,Task1.KeyStatus.farlightkey,4,16);
		
		OLED_ShowString(0,6,"nek",16);//���⿪��
		OLED_ShowNum(50,6,Task1.KeyStatus->nearlightkey,4,16);
	break;
	case 1:
	OLED_ShowString(0,0,"hik",16);
		OLED_ShowNum(50,0,Task1.KeyStatus->highspeedkey,4,16);
		
		OLED_ShowString(0,2,"mek",16);
		OLED_ShowNum(50,2,Task1.KeyStatus->medspeedkey,4,16);
		
		OLED_ShowString(0,4,"lok:",16);
		OLED_ShowNum(50,4,Task1.KeyStatus->lowspeedkey,4,16);
		
		OLED_ShowString(0,6,"back",16);
		OLED_ShowNum(50,6,Task1.KeyStatus->backspeedkey,4,16);
	break;

	case 2:
	OLED_ShowString(0,0,"setk",16);
		OLED_ShowNum(50,0,Task1.KeyStatus->seatlockkey,4,16);
		
		OLED_ShowString(0,2,"frok",16);
		OLED_ShowNum(50,2,Task1.KeyStatus->froPoslockkey,4,16);
		
		OLED_ShowString(0,4,"sw2",16);
		OLED_ShowNum(50,4,Task1.KeyStatus->sw2,4,16);
		
		OLED_ShowString(0,6,"sw1",16);
		OLED_ShowNum(50,6,Task1.KeyStatus->sw1,4,16);
	break;

	case 3:
	OLED_ShowString(0,0,"hork",16);
		OLED_ShowNum(50,0,Task1.KeyStatus->hornkey,4,16);
		
		OLED_ShowString(0,2,"fogk",16);
		OLED_ShowNum(50,2,Task1.KeyStatus->foglightkey,4,16);
		
		OLED_ShowString(0,4,"brk",16);
		OLED_ShowNum(50,4,Task1.KeyStatus->brakelightkey,4,16);
		
		OLED_ShowString(0,6,"prok",16);
		OLED_ShowNum(50,6,Task1.KeyStatus->profilelightkey,4,16);
	break;

	case 4:
	OLED_ShowString(0,0,"tak",16);
		OLED_ShowNum(50,0,Task1.KeyStatus->taillightkey,4,16);
		
		OLED_ShowString(0,2,"back",16);
		OLED_ShowNum(50,2,Task1.KeyStatus->backspeedkey|Task1.KeyStatus->backlightkey,4,16);
	break;
	case 5:
	OLED_ShowString(0,0,"braKn:",16);
		OLED_ShowNum(50,0,Task2.ADvalue->brake_Knob,4,16);
		
		OLED_ShowString(0,2,"AccKn:",16);
		OLED_ShowNum(50,2,Task2.ADvalue->accelerator_Knob,4,16);

		OLED_ShowString(0,4,"PreKn:",16);
		OLED_ShowNum(50,4,Task2.ADvalue->pressure_Knob,4,16);

		OLED_ShowString(0,6,"IllKn:",16);
		OLED_ShowNum(50,6,Task2.ADvalue->illumination_Knob,4,16);
	break;
	case 6:
		OLED_ShowString(0,0,"InfKn:",16);
		OLED_ShowNum(50,0,Task2.ADvalue->Infrared_Knob,4,16);
	break;	
	default:
	break;
	}
#endif
#if	OLED_USER_1_5
	switch(keycount)
	{
		case 0:
		OLED_display_string_8x16(0,0,"lek:");//��ת�򿪹�
		OLED_modify_num(40,0,DigitalSignal.KeyStatus.leftturnkey,16);
		
		OLED_display_string_8x16(0,16,"rik:");//��ת�򿪹�
		OLED_modify_num(40,16,DigitalSignal.KeyStatus.rightturnkey,16);
		
		OLED_display_string_8x16(0,32,"fak:");//Զ�⿪��
		OLED_modify_num(40,32,DigitalSignal.KeyStatus.farlightkey,16);
		
		OLED_display_string_8x16(0,48,"nek:");//���⿪��
		OLED_modify_num(40,48,DigitalSignal.KeyStatus.nearlightkey,16);
		break;
		case 1:
		OLED_display_string_8x16(0,0,"hik:");
		OLED_modify_num(40,0,DigitalSignal.KeyStatus.highspeedkey,16);
		
		OLED_display_string_8x16(0,16,"mek:");
		OLED_modify_num(40,16,DigitalSignal.KeyStatus.medspeedkey,16);
		
		OLED_display_string_8x16(0,32,"lok:");
		OLED_modify_num(40,32,DigitalSignal.KeyStatus.lowspeedkey,16);
		
		OLED_display_string_8x16(0,48,"back:");
		OLED_modify_num(40,48,DigitalSignal.KeyStatus.backspeedkey,16);
		break;
		case 2:
		OLED_display_string_8x16(0,0,"setk:");
		OLED_modify_num(40,0,DigitalSignal.KeyStatus.seatlockkey,16);
		
		OLED_display_string_8x16(0,16,"frok:");
		OLED_modify_num(40,16,DigitalSignal.KeyStatus.froPoslockkey,16);
		
		OLED_display_string_8x16(0,32,"sw2:");
		OLED_modify_num(40,32,DigitalSignal.KeyStatus.sw2,16);
		
		OLED_display_string_8x16(0,48,"sw1:");
		OLED_modify_num(40,48,DigitalSignal.KeyStatus.sw1,16);
		break;
		case 3:
		OLED_display_string_8x16(0,0,"hork:");
		OLED_modify_num(40,0,DigitalSignal.KeyStatus.hornkey,16);
		
		OLED_display_string_8x16(0,16,"fogk:");
		OLED_modify_num(40,16,DigitalSignal.KeyStatus.foglightkey,16);
		
		OLED_display_string_8x16(0,32,"brk:");
		OLED_modify_num(40,32,DigitalSignal.KeyStatus.brakelightkey,16);
		
		OLED_display_string_8x16(0,48,"prok:");
		OLED_modify_num(40,48,DigitalSignal.KeyStatus.profilelightkey,16);
		break;
		case 4:
		OLED_display_string_8x16(0,0,"tak:");
		OLED_modify_num(40,0,DigitalSignal.KeyStatus.taillightkey,16);
		
		OLED_display_string_8x16(0,16,"back:");
		OLED_modify_num(40,16,DigitalSignal.KeyStatus.backlightkey,16);
		break;
		
		case 5:
		OLED_display_string_8x16(0,0,"braKn:");
		OLED_modify_num(40,0,ad_value.brake_Knob,16);
		
		OLED_display_string_8x16(0,16,"AccKn:");
		OLED_modify_num(40,16,ad_value.accelerator_Knob,16);

		OLED_display_string_8x16(0,32,"PreKn:");
		OLED_modify_num(40,32,ad_value.pressure_Knob,16);

		OLED_display_string_8x16(0,48,"IllKn:");
		OLED_modify_num(40,48,ad_value.illumination_Knob,16);

		OLED_display_string_8x16(0,64,"InfKn:");
		OLED_modify_num(40,64,ad_value.Infrared_Knob,16);
		break;
		
	default:
	break;
	}
	
#endif
}

void OLED_ConfigInit(void)
{
	IIC_Init();
	OLED_Init();
	OLED_Clear(0);
}
/*�����߳�:oled��ʾ*/
void oled_thread_entry(void *parameter)
{
	while(1)
	{
		if(DigitalSignal.KeyStatus.sw1==1)
		{
			DigitalSignal.KeyStatus.sw1=0;
			OLED_Clear(0);
			sw1keycount++;
			if(sw1keycount>=6) sw1keycount=0;
		}
		oled_show(sw1keycount);
		rt_thread_delay(100); /* ��ʱ 100 �� tick */
	}
}








#include "IIC.h"
#if	OLED_USER_0_96
static void i2c_Delay(void)
{
	uint16_t i;

	/*　
		可用逻辑分析仪测量I2C通讯时的频率
    工作条件：CPU主频72MHz ，MDK编译环境，1级优化
  
		//经测试，循环次数为20~250时都能通讯正常

	*/
	for (i = 0; i<10 ; i++);
}
#endif
/**********************************************
//IIC Start
**********************************************/
void IIC_Start(void)
{
	OLED_SCLK_Set();
	OLED_SDIN_Set();
	OLED_SDIN_Clr();
	OLED_SCLK_Clr();
}

/**********************************************
//IIC Stop
**********************************************/
void IIC_Stop(void)
{
	OLED_SCLK_Set() ;
	OLED_SDIN_Clr();
	OLED_SDIN_Set();
}
/*配置I2C*/
void IIC_Init(void)
{
 	GPIO_InitTypeDef  GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE); //使能PB端口时钟
	GPIO_InitStructure.GPIO_Pin = I2C_SCL_PIN | I2C_SDA_PIN;	
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
 	GPIO_Init(GPIOB, &GPIO_InitStructure);	  //初始化GPIOB
	GPIO_SetBits(GPIOB,I2C_SDA_PIN|I2C_SCL_PIN);
}

/*
*********************************************************************************************************
*	函 数 名: i2c_WaitAck
*	功能说明: CPU产生一个时钟，并读取器件的ACK应答信号
*	形    参：无
*	返 回 值: 返回0表示正确应答，1表示无器件响应
*********************************************************************************************************
*/
uint8_t IIC_Wait_Ack(void)
{
#if	OLED_USER_0_96
	uint8_t re;
	OLED_SDIN_Set();
	i2c_Delay();
	OLED_SCLK_Set() ;
	i2c_Delay();
	if(GPIO_ReadInputDataBit(I2C_SDA_PORT, I2C_SDA_PIN))
	{
		re = 1;
	}
	else
	{
		re = 0;
	}
	OLED_SCLK_Clr();
	i2c_Delay();
	return re;
#endif
#if	OLED_USER_1_5
	OLED_SCLK_Set() ;
	OLED_SCLK_Clr();
	return 0;
#endif
}

/*
*********************************************************************************************************
*	函 数 名: i2c_Ack
*	功能说明: CPU产生一个ACK信号
*	形    参：无
*	返 回 值: 无
*********************************************************************************************************
*/
#if OLED_MODE
void i2c_Ack(void)
{
	OLED_SDIN_Clr();
	i2c_Delay();
	OLED_SCLK_Set() ;	/* CPU产生1个时钟 */
	i2c_Delay();
	OLED_SCLK_Clr();
	i2c_Delay();
	OLED_SDIN_Set();	/* CPU释放SDA总线 */
}
#endif
/*
*********************************************************************************************************
*	函 数 名: i2c_NAck
*	功能说明: CPU产生1个NACK信号
*	形    参：无
*	返 回 值: 无
*********************************************************************************************************
*/
#if OLED_MODE
void i2c_NAck(void)
{
	OLED_SDIN_Set();	/* CPU驱动SDA = 1 */
	i2c_Delay();
	OLED_SCLK_Set() ;	/* CPU产生1个时钟 */
	i2c_Delay();
	OLED_SCLK_Clr();
	i2c_Delay();	
}
#endif
/**********************************************
// IIC Write byte
**********************************************/
void Write_IIC_Byte(unsigned char _ucByte)
{
#if	OLED_USER_0_96
	uint8_t i;

	/* 先发送字节的高位bit7 */
	for (i = 0; i < 8; i++)
	{		
		if (_ucByte & 0x80)
		{
			OLED_SDIN_Set();
		}
		else
		{
			OLED_SDIN_Clr();
		}
		i2c_Delay();
		OLED_SCLK_Set() ;
		i2c_Delay();	
		OLED_SCLK_Clr() ;
		if (i == 7)
		{
			OLED_SDIN_Set();// 释放总线
		}
		_ucByte <<= 1;	/* 左移一个bit */
		i2c_Delay();
	}
#endif
#if	OLED_USER_1_5
	unsigned char i;
	unsigned char m,da;
	da=_ucByte;
	OLED_SCLK_Clr();
	for(i=0;i<8;i++)		
	{
			m=da;
		m=m&0x80;
		if(m==0x80)
		{OLED_SDIN_Set();}
		else OLED_SDIN_Clr();
			da=da<<1;
		OLED_SCLK_Set();
		OLED_SCLK_Clr();
		}
#endif
}
/*
*********************************************************************************************************
*	函 数 名: i2c_SendByte
*	功能说明: CPU向I2C总线设备发送8bit数据
*	形    参：_ucByte ： 等待发送的字节
*	返 回 值: 无
*********************************************************************************************************
*/
#if OLED_MODE
void i2c_SendByte(uint8_t _ucByte)
{
	uint8_t i;

	/* 先发送字节的高位bit7 */
	for (i = 0; i < 8; i++)
	{		
		if (_ucByte & 0x80)
		{
			OLED_SDIN_Set();
		}
		else
		{
			OLED_SDIN_Clr();
		}
		i2c_Delay();
		OLED_SCLK_Set() ;
		i2c_Delay();	
		OLED_SCLK_Clr() ;
		if (i == 7)
		{
			OLED_SDIN_Set();// 释放总线
		}
		_ucByte <<= 1;	/* 左移一个bit */
		i2c_Delay();
	}
}
#endif
/*
*********************************************************************************************************
*	函 数 名: i2c_ReadByte
*	功能说明: CPU从I2C总线设备读取8bit数据
*	形    参：无
*	返 回 值: 读到的数据
*********************************************************************************************************
*/
#if OLED_MODE
uint8_t i2c_ReadByte(void)
{
	uint8_t i;
	uint8_t value;

	/* 读到第1个bit为数据的bit7 */
	value = 0;
	for (i = 0; i < 8; i++)
	{
		value <<= 1;
		OLED_SCLK_Set() ;
		i2c_Delay();
		if (GPIO_ReadInputDataBit(I2C_SDA_PORT, I2C_SDA_PIN))
		{
			value++;
		}
		OLED_SCLK_Clr() ;
		i2c_Delay();
	}
	return value;
}
#endif
/*
*********************************************************************************************************
*	函 数 名: i2c_CheckDevice
*	功能说明: 检测I2C总线设备，CPU向发送设备地址，然后读取设备应答来判断该设备是否存在
*	形    参：_Address：设备的I2C总线地址
*	返 回 值: 返回值 0 表示正确， 返回1表示未探测到
*********************************************************************************************************
*/
#if OLED_MODE
uint8_t I2C_CheckDevice(uint8_t _Address)
{
	uint8_t ucAck;

	IIC_Init();		/* 配置GPIO */

	
	IIC_Start();		/* 发送启动信号 */

	/* 发送设备地址+读写控制bit（0 = w， 1 = r) bit7 先传 */
	i2c_SendByte(_Address | OLED_CMD);
	ucAck = IIC_Wait_Ack();	/* 检测设备的ACK应答 */

	IIC_Stop();			/* 发送停止信号 */

	return ucAck;
}
#endif

/**********************************************
// IIC Write Command
**********************************************/
void Write_IIC_Command(unsigned char IIC_Command)
{
   IIC_Start();
   Write_IIC_Byte(IIC_SLAVE_ADDR);            //Slave address,SA0=0
	IIC_Wait_Ack();	
   Write_IIC_Byte(0x00);			//write command
	IIC_Wait_Ack();	
   Write_IIC_Byte(IIC_Command); 
	IIC_Wait_Ack();	
   IIC_Stop();
}
/**********************************************
// IIC Write Data
**********************************************/
void Write_IIC_Data(unsigned char IIC_Data)
{
   IIC_Start();
   Write_IIC_Byte(IIC_SLAVE_ADDR);			//D/C#=0; R/W#=0
	 IIC_Wait_Ack();	
   Write_IIC_Byte(0x40);			//write data
	 IIC_Wait_Ack();	
   Write_IIC_Byte(IIC_Data);
	 IIC_Wait_Ack();	
   IIC_Stop();
}
void OLED_WR_Byte(unsigned char dat,unsigned char cmd)
{
	if(cmd)
	{
	Write_IIC_Data(dat);
	}
	else {
	Write_IIC_Command(dat);
		
	}
}












#include "IIC.h"
#if	OLED_USER_0_96
static void i2c_Delay(void)
{
	uint16_t i;

	/*��
		�����߼������ǲ���I2CͨѶʱ��Ƶ��
    ����������CPU��Ƶ72MHz ��MDK���뻷����1���Ż�
  
		//�����ԣ�ѭ������Ϊ20~250ʱ����ͨѶ����

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
/*����I2C*/
void IIC_Init(void)
{
 	GPIO_InitTypeDef  GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE); //ʹ��PB�˿�ʱ��
	GPIO_InitStructure.GPIO_Pin = I2C_SCL_PIN | I2C_SDA_PIN;	
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
 	GPIO_Init(GPIOB, &GPIO_InitStructure);	  //��ʼ��GPIOB
	GPIO_SetBits(GPIOB,I2C_SDA_PIN|I2C_SCL_PIN);
}

/*
*********************************************************************************************************
*	�� �� ��: i2c_WaitAck
*	����˵��: CPU����һ��ʱ�ӣ�����ȡ������ACKӦ���ź�
*	��    �Σ���
*	�� �� ֵ: ����0��ʾ��ȷӦ��1��ʾ��������Ӧ
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
*	�� �� ��: i2c_Ack
*	����˵��: CPU����һ��ACK�ź�
*	��    �Σ���
*	�� �� ֵ: ��
*********************************************************************************************************
*/
#if OLED_MODE
void i2c_Ack(void)
{
	OLED_SDIN_Clr();
	i2c_Delay();
	OLED_SCLK_Set() ;	/* CPU����1��ʱ�� */
	i2c_Delay();
	OLED_SCLK_Clr();
	i2c_Delay();
	OLED_SDIN_Set();	/* CPU�ͷ�SDA���� */
}
#endif
/*
*********************************************************************************************************
*	�� �� ��: i2c_NAck
*	����˵��: CPU����1��NACK�ź�
*	��    �Σ���
*	�� �� ֵ: ��
*********************************************************************************************************
*/
#if OLED_MODE
void i2c_NAck(void)
{
	OLED_SDIN_Set();	/* CPU����SDA = 1 */
	i2c_Delay();
	OLED_SCLK_Set() ;	/* CPU����1��ʱ�� */
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

	/* �ȷ����ֽڵĸ�λbit7 */
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
			OLED_SDIN_Set();// �ͷ�����
		}
		_ucByte <<= 1;	/* ����һ��bit */
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
*	�� �� ��: i2c_SendByte
*	����˵��: CPU��I2C�����豸����8bit����
*	��    �Σ�_ucByte �� �ȴ����͵��ֽ�
*	�� �� ֵ: ��
*********************************************************************************************************
*/
#if OLED_MODE
void i2c_SendByte(uint8_t _ucByte)
{
	uint8_t i;

	/* �ȷ����ֽڵĸ�λbit7 */
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
			OLED_SDIN_Set();// �ͷ�����
		}
		_ucByte <<= 1;	/* ����һ��bit */
		i2c_Delay();
	}
}
#endif
/*
*********************************************************************************************************
*	�� �� ��: i2c_ReadByte
*	����˵��: CPU��I2C�����豸��ȡ8bit����
*	��    �Σ���
*	�� �� ֵ: ����������
*********************************************************************************************************
*/
#if OLED_MODE
uint8_t i2c_ReadByte(void)
{
	uint8_t i;
	uint8_t value;

	/* ������1��bitΪ���ݵ�bit7 */
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
*	�� �� ��: i2c_CheckDevice
*	����˵��: ���I2C�����豸��CPU�����豸��ַ��Ȼ���ȡ�豸Ӧ�����жϸ��豸�Ƿ����
*	��    �Σ�_Address���豸��I2C���ߵ�ַ
*	�� �� ֵ: ����ֵ 0 ��ʾ��ȷ�� ����1��ʾδ̽�⵽
*********************************************************************************************************
*/
#if OLED_MODE
uint8_t I2C_CheckDevice(uint8_t _Address)
{
	uint8_t ucAck;

	IIC_Init();		/* ����GPIO */

	
	IIC_Start();		/* ���������ź� */

	/* �����豸��ַ+��д����bit��0 = w�� 1 = r) bit7 �ȴ� */
	i2c_SendByte(_Address | OLED_CMD);
	ucAck = IIC_Wait_Ack();	/* ����豸��ACKӦ�� */

	IIC_Stop();			/* ����ֹͣ�ź� */

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












#include "usart.h"

BUFF_ARRAY buff_array={0};
SendDetail sendDetail;
static u16 dma_rx_length=0;

extern digitalSignal DigitalSignal;
extern AD_VALUE ad_value;
static void USART1_Config(void)
{
	GPIO_InitTypeDef	GPIO_InitStructure;
	USART_InitTypeDef	USART_InitStructure;
	USB_USART_GPIO_APBxClkCmd(USB_USART_GPIO_CLK,ENABLE);
	USB_USART_APBxClkCmd(USB_USART_CLK,ENABLE);
	RCC_APB1PeriphClockCmd( RCC_APB1Periph_USB, ENABLE) ;

	GPIO_InitStructure.GPIO_Pin  = USB_USART_TX_GPIO_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(USB_USART_TX_GPIO_PORT,&GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin  = USB_USART_RX_GPIO_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_Init(USB_USART_RX_GPIO_PORT,&GPIO_InitStructure);
	
	USART_InitStructure.USART_BaudRate = USB_USART_BAUDRATE;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No ;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_Init(USB_USARTx,&USART_InitStructure);
	USART_ClearFlag(USART1,USART_FLAG_IDLE|USART_FLAG_RXNE);
#if DMA_Define
	USART_ITConfig(USB_USARTx,USART_IT_IDLE,ENABLE);
	USART_Cmd(USB_USARTx,DISABLE);
#else
	USART_ITConfig(USB_USARTx,USART_IT_RXNE,ENABLE);
	USART_Cmd(USB_USARTx,ENABLE);
#endif
}

#if DMA_Define
static void DMA_Uart_RX_Init(void)
{
	DMA_InitTypeDef DMA_InitStructure; 
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1,ENABLE);
	DMA_Cmd(USB_RX_DMA_Channel,DISABLE);
	DMA_DeInit(USB_RX_DMA_Channel);
	DMA_InitStructure.DMA_PeripheralBaseAddr = USB_DMA_BASE_ADDR;
	DMA_InitStructure.DMA_MemoryBaseAddr = (u32)(&buff_array.RxBuffer);
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;
	DMA_InitStructure.DMA_BufferSize = USB_buffer_size;
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
	DMA_InitStructure.DMA_PeripheralDataSize= DMA_PeripheralDataSize_Byte;
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
	DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;
	DMA_InitStructure.DMA_Priority = DMA_Priority_VeryHigh;
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
	DMA_Init(USB_RX_DMA_Channel,&DMA_InitStructure);
	DMA_ClearFlag(DMA1_FLAG_GL5);
	DMA_Cmd(USB_RX_DMA_Channel,DISABLE);
	USART_DMACmd(USB_USARTx,USART_DMAReq_Rx,DISABLE);// 开启串口DMA接收
}
static void USART_RX_DMA_enable(void)
{
	DMA_Cmd(USB_RX_DMA_Channel,ENABLE);
	USART_DMACmd(USB_USARTx,USART_DMAReq_Rx,ENABLE);// 开启串口DMA接收
	USART_Cmd(USB_USARTx,ENABLE);
}
void DMAget_rx_byte(void)
{
	dma_rx_length= USB_buffer_size-DMA_GetCurrDataCounter(USB_RX_DMA_Channel);
	if(dma_rx_length>=50)
	{
		memset(buff_array.RxBuffer,0,dma_rx_length);	
	}
}
#endif
//发送一个字节
void Usart_SendByte(USART_TypeDef * pUSARTx,uint8_t ch)
{
	USART_SendData(pUSARTx,ch);
	while(USART_GetFlagStatus(pUSARTx,USART_FLAG_TXE) == RESET);
}
//重定向c库函数printf到串口，重定向后可使用printf函数
int fputc(int ch,FILE *f)
{
	//发送一个字节数据到串口
	Usart_SendByte(USB_USARTx,(u8)ch);
	//等待发送完毕
	while(USART_GetFlagStatus(USB_USARTx,USART_FLAG_TXE) == RESET);
	return ch;
}
//重定向c库函数scanf到串口，重写向后使用scanf，getchar等函数
int fgetc(FILE *f)
{
	//等待串口输入数据
	while(USART_GetFlagStatus(USB_USARTx,USART_FLAG_RXNE) == RESET);
	return (int)USART_ReceiveData(USB_USARTx);
}

static void USART1_NVIC_Init(void)
{
	NVIC_InitTypeDef	NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel = USB_USART_IRQ;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE ;
	NVIC_Init(&NVIC_InitStructure);
}
/*flatbed初始化任务变量*/
void InitSendDetail(void)
{
	sendDetail.CBerror=0;
	sendDetail.CBstatus=0;
	sendDetail.Lightstatus=0;
	sendDetail.loadstatus=0;
	sendDetail.speedpulse=0;
}

void USB_USART_Config(void)
{
	USART1_NVIC_Init();
#if DMA_Define
	DMA_Uart_RX_Init();
#endif
	USART1_Config();
#if DMA_Define
	USART_RX_DMA_enable();
#endif
	InitSendDetail();
}

/*====================================================flatbed========================================================*/

static void Usart1_Send(USART_TypeDef * pUSARTx,u8 *str,u8 length)
{
	unsigned int k = 0;
	do{
		Usart_SendByte(pUSARTx,(u8)*(str+k));
		k++;
	}while(--length);
	while(USART_GetFlagStatus(USB_USARTx,USART_FLAG_TC) == RESET);
}
static void DT_Send_Data(u8* dataToSend,u8 length)
{
	/*串口发送要发送的数据组数据长度*/
		Usart1_Send(USB_USARTx,dataToSend,length);
}

static void command10_sendFun(void)
{
	u8 i=0, crc =0;
	buff_array.data_to_send[0] = command10_send_addr;
	buff_array.data_to_send[1] = fralength;
	buff_array.data_to_send[2] = command10Num;
	buff_array.data_to_send[3] = handshakeAddr;
	buff_array.data_to_send[4] = lordversionNum;
	buff_array.data_to_send[5] = boutversionNum;
	buff_array.data_to_send[6] = Timeday;
	buff_array.data_to_send[7] = Timemonth;
	buff_array.data_to_send[8] = Timeyear;
	
	for(i=0;i<15;++i)
		crc ^= buff_array.data_to_send[i];
	buff_array.data_to_send[15] = crc;
	/*发送数据组*/
	DT_Send_Data(buff_array.data_to_send,16);
}

static void Command21_sendFun(void)
{
	u8 i=0, crc =0;

	buff_array.data_to_send[0] = command21_send_addr;
	buff_array.data_to_send[1] = fralength;
	buff_array.data_to_send[2] = command21Num;
	buff_array.data_to_send[3] = sendDetail.CBstatus;
	buff_array.data_to_send[4] = sendDetail.CBerror;
	buff_array.data_to_send[5] = sendDetail.Lightstatus; 
	buff_array.data_to_send[6] = 0X00;//BYTE1(sendDetail.speedpulse);
	buff_array.data_to_send[7] = BYTE0(sendDetail.speedpulse);
	buff_array.data_to_send[8] = sendDetail.loadstatus;

	for(i=0;i<15;++i)
		crc ^= buff_array.data_to_send[i];
	buff_array.data_to_send[15] = crc;
	/*发送数据组*/
	DT_Send_Data(buff_array.data_to_send,16);
}

void DT_Data_Receive_Prepare(void)
{
	/*局部静态变量:接收缓存*/
	static u8 _data_len = 0,_data_cnt = 0;
	static u8 state =0;
	u8 crc=0,i=0;
	if(0==state&&(buff_array.RxBuffer[0]==command1_rev_addr||buff_array.RxBuffer[0]==0x01))
	{
		state =1;
		buff_array.Memory_Pool[0]=buff_array.RxBuffer[0];
	}else if(1==state&&buff_array.RxBuffer[1]==fralength)
	{
		state =2;
		buff_array.Memory_Pool[1]=buff_array.RxBuffer[1];
		_data_len = buff_array.RxBuffer[1];
		_data_cnt = 0;
	}else if(2==state&&_data_len>0)
	{
		_data_len--;
		buff_array.Memory_Pool[2+_data_cnt++]=buff_array.RxBuffer[2];
		if(_data_len==0)
			state = 3;
	}else if(state==3)
	{
		state = 0;
		for(i=0;i<fralength-1;++i)
			crc ^= buff_array.Memory_Pool[i];
		if(crc==buff_array.RxBuffer[15])
		buff_array.Memory_Pool[2+_data_cnt]= buff_array.RxBuffer[15];
	}else{
	state = 0;
	}
}
#if !DMA_Define
static void APPTest(void)
{
	u8 t,len,i,j,k,*p;
	if(buff_array.USART_RX_STA&0x8000)
	{
		len = buff_array.USART_RX_STA&0X3FFF;
		p=buff_array.RxBuffer;
		for(i=0;i<16;++i) buff_array.Memory_Pool[i] = *p++;
		t = buff_array.Memory_Pool[5];
		k = buff_array.Memory_Pool[0];
		j = 16;
		switch(t)
		{
			case 0:
			{
				if(k==1) command10_sendFun();
			}
			break;
			case 1:
			{
				Command21_sendFun();
			}
			break;
			default: break;
		}
		buff_array.USART_RX_STA=0;
		memset(buff_array.Memory_Pool,0,USB_memorypool_size);		
	}
}
#endif
static void APPdata_Analysis(u8 *CB_HeadAddr,u8 *askmode)
{
	u8 i,*p;
	p=buff_array.RxBuffer;
	for(i=0;i<USB_memorypool_size;++i) buff_array.Memory_Pool[i] =*p++;
	*CB_HeadAddr = buff_array.Memory_Pool[0];
	*askmode = buff_array.Memory_Pool[5];
	sendDetail.Lightstatus = DigitalSignal.KeyStatus.farlightkey|
						(DigitalSignal.KeyStatus.nearlightkey<<1)|
						(DigitalSignal.KeyStatus.leftturnkey<<2)|
						(DigitalSignal.KeyStatus.rightturnkey<<3)|
						(DigitalSignal.Light_LockStatus.tail_light<<4)|
						(DigitalSignal.Light_LockStatus.brake_light<<5)|
						(DigitalSignal.Light_LockStatus.froPos_lock<<6)|
						(DigitalSignal.Light_LockStatus.carbrand_light<<7);
	
	sendDetail.loadstatus = 0x00;
	sendDetail.CBstatus = 0x00;
	sendDetail.speedpulse = ad_value.accelerator_Knob;
}

void APPdata_deal(void)
{
#if DMA_Define
	static u8 flag=0,flag2=0;
	u8 CB_HeadAddr=0;
	u8 CB_askmode=0;
	APPdata_Analysis(&CB_HeadAddr,&CB_askmode);
	if(!CB_askmode)
	{
		if(CB_HeadAddr)
		{
			flag2=1;
			if(flag2==1){
			command10_sendFun();
			flag2=0;
			memset(buff_array.Memory_Pool,0,USB_buffer_size);
			}
		}
	}else if(CB_askmode){
		flag=1;
		if(flag==1){
		Command21_sendFun();
		flag=0;
		memset(buff_array.Memory_Pool,0,USB_memorypool_size);
		}
	}
#else 
	APPTest();
#endif
}

/*任务线程:flatbed 的USB通讯*/
void flatbed_thread_entry(void *parameter)
{
	while(1)
	{
		APPdata_deal();	
		rt_thread_delay(50);/*延时50个systick*/
	}
}































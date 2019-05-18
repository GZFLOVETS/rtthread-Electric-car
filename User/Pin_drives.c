#include "Pin_drives.h"


static const struct pin_irq_map pin_irq_map[] =
{
    {GPIO_Pin_0, EXTI0_IRQn},
    {GPIO_Pin_1, EXTI1_IRQn},
    {GPIO_Pin_2, EXTI2_IRQn},
    {GPIO_Pin_3, EXTI3_IRQn},
    {GPIO_Pin_4, EXTI4_IRQn},
    {GPIO_Pin_5, EXTI9_5_IRQn},
    {GPIO_Pin_6, EXTI9_5_IRQn},
    {GPIO_Pin_7, EXTI9_5_IRQn},
    {GPIO_Pin_8, EXTI9_5_IRQn},
    {GPIO_Pin_9, EXTI9_5_IRQn},
    {GPIO_Pin_10, EXTI15_10_IRQn},
    {GPIO_Pin_11, EXTI15_10_IRQn},
    {GPIO_Pin_12, EXTI15_10_IRQn},
    {GPIO_Pin_13, EXTI15_10_IRQn},
    {GPIO_Pin_14, EXTI15_10_IRQn},
    {GPIO_Pin_15, EXTI15_10_IRQn},
};
struct rt_pin_irq_hdr pin_irq_hdr_tab[] =
{
    { -1, 1, RT_NULL, RT_NULL},
    { -1, 1, RT_NULL, RT_NULL},
    { -1, 1, RT_NULL, RT_NULL},
    { -1, 1, RT_NULL, RT_NULL},
    { -1, 1, RT_NULL, RT_NULL},
    { -1, 1, RT_NULL, RT_NULL},
    { -1, 1, RT_NULL, RT_NULL},
    { -1, 1, RT_NULL, RT_NULL},
    { -1, 1, RT_NULL, RT_NULL},
    { -1, 1, RT_NULL, RT_NULL},
    { -1, 1, RT_NULL, RT_NULL},
    { -1, 1, RT_NULL, RT_NULL},
    { -1, 1, RT_NULL, RT_NULL},
    { -1, 1, RT_NULL, RT_NULL},
    { -1, 1, RT_NULL, RT_NULL},
    { -1, 1, RT_NULL, RT_NULL},
};
static const struct pin_index pins[] =
{
#if(STM32F103RBT6 == 64)
    __STM32_PIN_DEFAULT,
    __STM32_PIN_DEFAULT,
    __STM32_PIN(2, C, 13),
    __STM32_PIN(3, C, 14),
	__STM32_PIN(4, C, 15),
	__STM32_PIN_DEFAULT,
	__STM32_PIN_DEFAULT,
	__STM32_PIN_DEFAULT,
	__STM32_PIN(8, C, 0),
	__STM32_PIN(9, C, 1),
	__STM32_PIN(10, C, 2),
	__STM32_PIN(11, C, 3),
	__STM32_PIN_DEFAULT,
	__STM32_PIN_DEFAULT,
	__STM32_PIN(14, A, 0),
	__STM32_PIN(15, A, 1),
	__STM32_PIN(16, A, 2),
	__STM32_PIN(17, A, 3),
	__STM32_PIN_DEFAULT,
	__STM32_PIN_DEFAULT,
	__STM32_PIN(20, A, 4),
	__STM32_PIN(21, A, 5),
	__STM32_PIN(22, A, 6),
	__STM32_PIN(23, A, 7),
	__STM32_PIN(24, C, 4),
	__STM32_PIN(25, C, 5),
	__STM32_PIN(26, B, 0),
	__STM32_PIN(27, B, 1),
	__STM32_PIN(28, B, 2),
	__STM32_PIN(29, B, 10),
	__STM32_PIN(30, B, 11),
	__STM32_PIN_DEFAULT,
	__STM32_PIN_DEFAULT,
	__STM32_PIN(33, B, 12),
	__STM32_PIN(34, B, 13),
	__STM32_PIN(35, B, 14),
	__STM32_PIN(36, B, 15),
	__STM32_PIN(37, C, 6),
	__STM32_PIN(38, C, 7),
	__STM32_PIN(39, C, 8),
	__STM32_PIN(40, C, 9),
	__STM32_PIN(41, A, 8),
	__STM32_PIN(42, A, 9),
	__STM32_PIN(43, A, 10),
	__STM32_PIN(44, A, 11),
	__STM32_PIN(45, A, 12),
	__STM32_PIN(46, A, 13),
	__STM32_PIN_DEFAULT,
	__STM32_PIN_DEFAULT,
	__STM32_PIN(49, A, 14),
	__STM32_PIN(50, A, 15),
	__STM32_PIN(51, C, 10),
	__STM32_PIN(52, C, 11),
	__STM32_PIN(53, C, 12),
	__STM32_PIN(54, D, 2),
	__STM32_PIN(55, B, 3),
	__STM32_PIN(56, B, 4),
	__STM32_PIN(57, B, 5),
	__STM32_PIN(58, B, 6),
	__STM32_PIN(59, B, 7),
	__STM32_PIN_DEFAULT,
	__STM32_PIN(61, B, 8),
	__STM32_PIN(62, B, 9),
	__STM32_PIN_DEFAULT,
	__STM32_PIN_DEFAULT,
#endif
};
void GPIOA_CLK_ENABLE(void)
{
   RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
}

void GPIOB_CLK_ENABLE(void)
{
   RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
}

void GPIOC_CLK_ENABLE(void)
{
   RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC,ENABLE);
}

void GPIOD_CLK_ENABLE(void)
{
   RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD,ENABLE);
}

const struct pin_index *get_pin(uint8_t pin)
{
    const struct pin_index *index;
    if (pin < ITEM_NUM(pins))
    {
        index = &pins[pin];
        if (index->index == -1)
            index = 0;
    }
    else
    {
        index = 0;
    }
    return index;
};

/*PIN设备管理接口*/

void pin_mode(long pin, GPIOMode_TypeDef mode)
{
	const struct pin_index *index;
	GPIO_InitTypeDef	GPIO_InitStructure;
	index = get_pin(pin);
	if(index == 0)
	{
		return ;
	}
	index->rcc();
	GPIO_InitStructure.GPIO_Pin = index->pin;
	GPIO_InitStructure.GPIO_Mode = mode;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(index->gpio,&GPIO_InitStructure);
}

int pin_write(long pin,u16 status)
{
	int ret;
	const struct pin_index *index;
	index = get_pin(pin);
	if(index == 0)
	{
		ret = 0;
	}
	if(status==1)
	{
		GPIO_SetBits(index->gpio,index->pin);
		ret = 1;
	}else if(status==0)
	{
		GPIO_ResetBits(index->gpio,index->pin);
		ret = 0;
	}
	return ret;
}

int pin_read(long pin)
{
	int value;
	const struct pin_index *index;
	value = PIN_LOW;
	index = get_pin(pin);
	if(index == 0)
	{
		return value;
	}
	value = GPIO_ReadInputDataBit(index->gpio,index->pin);
	return value;
}


static __inline long bit2bitno(long bit)
{
    int i;
    for (i = 0; i < 32; i++)
    {
        if ((0x01 << i) == bit)
        {
            return i;
        }
    }
    return -1;
}
static __inline const struct pin_irq_map *get_pin_irq_map(uint32_t pinbit)
{
    long mapindex = bit2bitno(pinbit);
    if (mapindex < 0 || mapindex >= ITEM_NUM(pin_irq_map))
    {
        return RT_NULL;
    }
    return &pin_irq_map[mapindex];
};

long stm32_pin_attach_irq(int32_t pin,uint32_t mode, void (*hdr)(void *args), void *args)
{
    const struct pin_index *index;
    long level;
    int32_t irqindex = -1;
    index = get_pin(pin);
    if (index == RT_NULL)
    {
        return RT_ENOSYS;
    }
    irqindex = bit2bitno(index->pin);
    if (irqindex < 0 || irqindex >= ITEM_NUM(pin_irq_map))
    {
        return RT_ENOSYS;
    }
    __set_PRIMASK(1);//关总中断
    if (pin_irq_hdr_tab[irqindex].pin == pin &&
            pin_irq_hdr_tab[irqindex].hdr == hdr &&
            pin_irq_hdr_tab[irqindex].mode == mode &&
            pin_irq_hdr_tab[irqindex].args == args)
    {
        __set_PRIMASK(0);//open总中断
        return RT_EOK;
    }
    if (pin_irq_hdr_tab[irqindex].pin != -1)
    {
        __set_PRIMASK(0);//open总中断
        return RT_EBUSY;
    }
    pin_irq_hdr_tab[irqindex].pin = pin;
    pin_irq_hdr_tab[irqindex].hdr = hdr;
    pin_irq_hdr_tab[irqindex].mode = mode;
    pin_irq_hdr_tab[irqindex].args = args;
    __set_PRIMASK(0);//open总中断
    return RT_EOK;
}

long stm32_pin_irq_enable(long pin,uint32_t enabled)
{
    const struct pin_index *index;
    const struct pin_irq_map *irqmap;
    long level;
    int32_t irqindex = -1;
    GPIO_InitTypeDef GPIO_InitStruct;
    EXTI_InitTypeDef EXTI_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);
    index = get_pin(pin);
    if (index == RT_NULL)
    {
        return RT_ENOSYS;
    }
    if (enabled == PIN_IRQ_ENABLE)
    {
        irqindex = bit2bitno(index->pin);
        if (irqindex < 0 || irqindex >= ITEM_NUM(pin_irq_map))
        {
            return RT_ENOSYS;
        }
        __set_PRIMASK(1);//close总中断
        if (pin_irq_hdr_tab[irqindex].pin == -1)
        {
            __set_PRIMASK(0);//open总中断
            return RT_ENOSYS;
        }
        irqmap = &pin_irq_map[irqindex];
        /* GPIO Periph clock enable */
        index->rcc();
        /* Configure GPIO_InitStructure */
        GPIO_InitStruct.GPIO_Pin = index->pin;
        GPIO_InitStruct.GPIO_Speed = GPIO_Speed_10MHz;
        switch (pin_irq_hdr_tab[irqindex].mode)
        {
        case PIN_IRQ_MODE_RISING:
            GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN_FLOATING;
            break;
        case PIN_IRQ_MODE_FALLING:
            GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN_FLOATING;
            break;
        case PIN_IRQ_MODE_RISING_FALLING:
            GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN_FLOATING;
            break;
        }
        GPIO_Init(index->gpio, &GPIO_InitStruct);
        __set_PRIMASK(0);//open总中断
    }
    else if (enabled == PIN_IRQ_DISABLE)
    {
        irqmap = get_pin_irq_map(index->pin);
        if (irqmap == RT_NULL)
        {
            return RT_ENOSYS;
        }
    }
    else
    {
        return RT_ENOSYS;
    }
    return RT_EOK;
}






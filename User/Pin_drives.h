#ifndef __Pin_drives_H
#define __Pin_drives_H 

#include "stm32f10x.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_rcc.h"
#define NORTTUseing
#define STM32F103RBT6 64
#define __STM32_PIN(index, gpio, gpio_index) {index, GPIO##gpio##_CLK_ENABLE, GPIO##gpio, GPIO_Pin_##gpio_index}
#define __STM32_PIN_DEFAULT  {-1, 0, 0, 0}

#define PIN_LOW                 0x00
#define PIN_HIGH                0x01
#define RT_NULL                         (0)

#define PIN_IRQ_MODE_RISING             0x00
#define PIN_IRQ_MODE_FALLING            0x01
#define PIN_IRQ_MODE_RISING_FALLING     0x02
#define PIN_IRQ_MODE_HIGH_LEVEL         0x03
#define PIN_IRQ_MODE_LOW_LEVEL          0x04

#define PIN_IRQ_DISABLE                 0x00
#define PIN_IRQ_ENABLE                  0x01

/* RT-Thread error code definitions */
#define RT_EOK                          0               /**< There is no error */
#define RT_ERROR                        1               /**< A generic error happens */
#define RT_ETIMEOUT                     2               /**< Timed out */
#define RT_EFULL                        3               /**< The resource is full */
#define RT_EEMPTY                       4               /**< The resource is empty */
#define RT_ENOMEM                       5               /**< No memory */
#define RT_ENOSYS                       6               /**< No system */
#define RT_EBUSY                        7               /**< Busy */
#define RT_EIO                          8               /**< IO error */
#define RT_EINTR                        9               /**< Interrupted system call */
#define RT_EINVAL                       10              /**< Invalid argument */

/* STM32 GPIO driver */
struct pin_index
{
    int index;
    void (*rcc)(void);
    GPIO_TypeDef *gpio;
    uint32_t pin;
};
struct pin_irq_map
{
    unsigned short pinbit;
    IRQn_Type irqno;
};
struct rt_pin_irq_hdr
{
    int16_t        pin;
    uint16_t       mode;
    void (*hdr)(void *args);
    void             *args;
};
void GPIOA_CLK_ENABLE(void);
void GPIOB_CLK_ENABLE(void);
void GPIOC_CLK_ENABLE(void);
void GPIOD_CLK_ENABLE(void);


#define ITEM_NUM(items) sizeof(items) / sizeof(items[0])


void pin_mode(long pin, GPIOMode_TypeDef mode);
int pin_write(long pin,u16 status);
int pin_read(long pin);
const struct pin_index *get_pin(uint8_t pin);








#endif

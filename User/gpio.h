#ifndef	__GPIO_H
#define 	__GPIO_H
#include "stm32f10x.h"



#define left_turnkey 			3
#define right_turnkey 			4
#define far_lightkey			26
#define near_lightkey			27
#define SW1					30
#define high_speedkey			37
#define med_speedkey			38
#define low_speedkey			39
#define back_speedkey			40
#define seat_lockkey			51
#define froPos_lockkey			52
#define SW2					53
#define horn_key				54
#define fog_lightkey			55
#define brake_lightkey			56
#define profile_lightkey 		57
#define tail_lightkey			61
#define back_lightkey			62

#define LED_TEST	 			25
#define brake_lightput			2
#define right_lightput			14
#define left_lightput			17
#define profile_lightput		20
#define carbrand_lightput		21
#define fog_lightput			29
#define tail_lightput			33
#define back_lightput			34
#define seat_lockput			35
#define froPos_lockput			36
#define Trunk_lockput			50
#define far_lightput			15
#define near_lightput			16

#define DIGITAL_GPIO_CLK		(RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOB|RCC_APB2Periph_GPIOC|RCC_APB2Periph_GPIOD)

#define success			1
#define fail				0

#define KEYON				1
#define KEYOFF			0

#define digitalToggle(p,i)	 {p->ODR ^=i;}		//Êä³ö·´×ª×´Ì¬




typedef struct keystatus{
	uint8_t leftturnkey   :1;
	uint8_t rightturnkey  :1;
	uint8_t farlightkey   :1;
	uint8_t nearlightkey:1	;
	uint8_t backlightkey:1	;
	uint8_t highspeedkey:1	;
	uint8_t medspeedkey:1	;
	uint8_t lowspeedkey:1	;
	uint8_t backspeedkey:1	;
	uint8_t seatlockkey:1	;
	uint8_t froPoslockkey:1	;
	uint8_t sw2		:1	;
	uint8_t sw1		:1	;
	uint8_t hornkey:1		;
	uint8_t foglightkey:1	;
	uint8_t brakelightkey:1	;
	uint8_t profilelightkey:1 ;
	uint8_t taillightkey:1	;
	uint8_t redlightkey:1	;
	uint8_t greenlightkey:1	;
	uint8_t bluelightkey:1	;
	uint8_t carbrandlightkey:1 ;
}__attribute__((packed)) keystatus;

typedef struct light_lockstatus{
	uint8_t brake_light:1;
	uint8_t right_light:1;
	uint8_t LEDTEST:1;
	uint8_t left_light:1;
	uint8_t profile_light:1;
	uint8_t carbrand_light:1;
	uint8_t fog_light:1;
	uint8_t tail_light:1;
	uint8_t back_light:1;
	uint8_t seat_lock:1;
	uint8_t froPos_lock:1;
	uint8_t Trunk_lock:1;
	uint8_t near_light:1;
	uint8_t far_light:1;
	uint8_t red_light:1;
	uint8_t green_light:1;
	uint8_t blue_light:1;
}__attribute__((packed)) light_lockstatus;
typedef struct myTask1{
	 keystatus KeyStatus;
	 light_lockstatus Light_LockStatus;
	 u8 left_turn_flag;
	 u8 right_turn_flag;
}__attribute__((packed)) digitalSignal;


static void Digital_In(void);
static void Digital_Out(void);

static uint8_t Digital_longmove_status(GPIO_TypeDef* GPIOx,uint16_t GPIO_Pin);
static uint8_t Digital_pointmove_status(GPIO_TypeDef* GPIOx,uint16_t GPIO_Pin);
static uint8_t Set_Digital_Put(GPIO_TypeDef * GPIOx,uint16_t PIN,uint8_t bool);
static void GPIO_Out_ConfigInit(void);
static void EXTI_KEY_Init(void);
static u8 Set_LeftTurnLight_control(long pin,int Bool);
static u8 Set_RightTurnLight_control(long pin,int Bool);

void Digital_GPIO_Config(void);
void GPIO_In_ConfigInit(void);
void SW1EXIT_Control(FunctionalState newStart);
void SeatLockEXIT_Control(FunctionalState newStart);
void FroPosLockEXIT_Control(FunctionalState newStart);








#endif

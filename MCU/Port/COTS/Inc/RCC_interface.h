/*
 * RCC_interface.h
 *
 *  Created on:	Jul 26, 2022
 *  Author:		Ali Emad Ali
 *
 *  References:
 *  	STM RM0368:	6.	Reset and clock control (RCC) for STM32F401xB/C and STM32F401xD/E
 */

#ifndef RCC_INTERFACE_H_
#define RCC_INTERFACE_H_


/*	AHB peripherals	*/
#define RCC_PERIPHERAL_DMA1			0
#define RCC_PERIPHERAL_DMA2			1
#define RCC_PERIPHERAL_SRAM			2
#define RCC_PERIPHERAL_FLITF		4
#define RCC_PERIPHERAL_CRC			6
#define RCC_PERIPHERAL_OTG			12
#define RCC_PERIPHERAL_ETHMAC		14
#define RCC_PERIPHERAL_ETHMACTX		15
#define RCC_PERIPHERAL_ETHMACRX		16

/*	APB1 peripherals	*/
#define RCC_PERIPHERAL_TIM2			0
#define RCC_PERIPHERAL_TIM3			1
#define RCC_PERIPHERAL_TIM4			2
#define RCC_PERIPHERAL_TIM5			3
#define RCC_PERIPHERAL_TIM6			4
#define RCC_PERIPHERAL_TIM7			5
#define RCC_PERIPHERAL_WWDG			11
#define RCC_PERIPHERAL_SPI2			14
#define RCC_PERIPHERAL_SPI3			15
#define RCC_PERIPHERAL_USART2		17
#define RCC_PERIPHERAL_USART3		18
#define RCC_PERIPHERAL_UART4		19
#define RCC_PERIPHERAL_UART5		20
#define RCC_PERIPHERAL_I2C1			21
#define RCC_PERIPHERAL_I2C2			22
#define RCC_PERIPHERAL_CAN1			25
#define RCC_PERIPHERAL_CAN2			26
#define RCC_PERIPHERAL_BKP			27
#define RCC_PERIPHERAL_PWR			28
#define RCC_PERIPHERAL_DAC			29

/*	APB2 peripherals	*/
#define RCC_PERIPHERAL_AFIO			0
#define RCC_PERIPHERAL_EXTI			1
#define RCC_PERIPHERAL_IOPA			2
#define RCC_PERIPHERAL_IOPB			3
#define RCC_PERIPHERAL_IOPC			4
#define RCC_PERIPHERAL_IOPD			5
#define RCC_PERIPHERAL_IOPE			6
#define RCC_PERIPHERAL_ADC1			9
#define RCC_PERIPHERAL_ADC2			10
#define RCC_PERIPHERAL_TIM1			11
#define RCC_PERIPHERAL_SPI1			12
#define RCC_PERIPHERAL_TIM8			13
#define RCC_PERIPHERAL_USART1		14
#define RCC_PERIPHERAL_ADC3			15
#define RCC_PERIPHERAL_TIM9			19
#define RCC_PERIPHERAL_TIM10		20
#define RCC_PERIPHERAL_TIM11		21


typedef enum{
	RCC_Bus_AHB,
	RCC_Bus_APB1,
	RCC_Bus_APB2
}RCC_Bus_t;

#define RCC_SYSCLOCKSWITCH_HSI	0
#define RCC_SYSCLOCKSWITCH_HSE	1
#define RCC_SYSCLOCKSWITCH_PLL	2

typedef enum{
	RCC_AHB_Prescaler_1 		= 0,
	RCC_AHB_Prescaler_2 		= 8,
	RCC_AHB_Prescaler_4 		= 9,
	RCC_AHB_Prescaler_8 		= 10,
	RCC_AHB_Prescaler_16		= 11,
	RCC_AHB_Prescaler_64		= 12,
	RCC_AHB_Prescaler_128		= 13,
	RCC_AHB_Prescaler_256		= 14,
	RCC_AHB_Prescaler_512		= 15,
}RCC_AHB_Prescaler_t;

typedef enum{
	RCC_APB_Prescaler_1 		= 0,
	RCC_APB_Prescaler_2 		= 4,
	RCC_APB_Prescaler_4 		= 5,
	RCC_APB_Prescaler_8 		= 6,
	RCC_APB_Prescaler_16 		= 7,
}RCC_APB_Prescaler_t;

typedef enum{
	RCC_ADC_Prescaler_PCLK2_by2,
	RCC_ADC_Prescaler_PCLK2_by4,
	RCC_ADC_Prescaler_PCLK2_by6,
	RCC_ADC_Prescaler_PCLK2_by8
}RCC_ADC_Prescaler_t;


typedef enum{
	RCC_MCO_ClockSource_NoClock		= 0,
	RCC_MCO_ClockSource_SYSCLK		= 4,
	RCC_MCO_ClockSource_HSI			= 5,
	RCC_MCO_ClockSource_HSE			= 6,
	RCC_MCO_ClockSource_PLL_by2		= 7
}RCC_MCO_ClockSource_t;


#define RCC_PLLSOURCE_HSI_by2		0
#define RCC_PLLSOURCE_PLLXTPRE		1

#define RCC_PLLXTPRESOURCE_HSE		0
#define RCC_PLLXTPRESOURCE_HSE_by2	1


#define RCC_HSI_CLK		8000000
#define RCC_LSI_CLK		40000

/*	inits the RCC HW according to the chosen configuration in "RCC_config.h".	*/
void RCC_voidSysClockInit(void);




#endif		/*	RCC_INTERFACE_H_	*/


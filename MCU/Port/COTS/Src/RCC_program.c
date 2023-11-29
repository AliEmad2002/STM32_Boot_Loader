/*
 * RCC_program.c
 *
 *  Created on:	Jul 26, 2022
 *  Author:		Ali Emad Ali
 *
 *  References:
 *  	STM RM0368:	6.	Reset and clock control (RCC) for STM32F401xB/C and STM32F401xD/E
 */

/*	LIB	*/
#include <stdint.h>
#include "BitMath.h"

/*	PORT	*/
#include "Flash.h"

/*	SELF	*/
#include "RCC_interface.h"
#include "RCC_config.h"
#include "RCC_private.h"



#if RCC_PLL_EN
/*
 * to store PLL frequency calculated in init function,
 * to be used - when needed - in the GetClk function.
 */
volatile uint32_t pllOutputFreq;

/*	inits PLL (target dependent)	*/
void RCC_voidInitPLL(void);
#endif

/*	inits the RCC HW according to the chosen configuration in "RCC_config.h".	*/
void RCC_voidSysClockInit(void)
{
	/*	Enabling selected sources, and clock security system	*/
	#if RCC_HSE_EN
		/*	Setting HSE bypass option before enabling it	*/
		WRT_BIT(RCC->CR, RCC_HSEBYP, RCC_HSE_BYPASSED);
		_SET_BIT(RCC->CR, RCC_HSEON);
		while(!GET_BIT(RCC->CR, RCC_HSERDY));
	#endif	/*	RCC_HSE_EN	*/

	#if RCC_HSI_EN
		_SET_BIT(RCC->CR, RCC_HSION);
		while(!GET_BIT(RCC->CR, RCC_HSIRDY));
	#endif	/*	RCC_HSI_EN	*/

	#if RCC_PLL_EN
		/*	configuring PLL first before enabling it	*/
		RCC_voidInitPLL();
		_SET_BIT(RCC->CR, RCC_PLLON);
		while(!GET_BIT(RCC->CR, RCC_PLLRDY));
	#endif	/*	RCC_HSE_EN	*/

	#if RCC_CSS_EN
		_SET_BIT(RCC->CR, RCC_CSSON);
	#else
		//CLR_BIT(RCC->CR, RCC_CSSON);
	#endif	/*	RCC_CSS_EN	*/
	/*	~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~		*/
	/*
	 * before switching system clock to the configured source,
	 * there are some restrictions that must be checked and encountered.
	 */

	/*
	 * Set FPEC prefetch buffer latency.
	 *
	 * Notice: At this point, frequency checking must be performed to decide which
	 * latency option to be selected. But as this code is ported for the Bootlader
	 * project, the frequency is known to be 72MHz, frequency calculation and checking
	 * is skipped to save code size.
	 */
	/*	two wait state	*/
	vPort_Flash_unlock();
	__HAL_FLASH_SET_LATENCY(FLASH_LATENCY_2);
	vPort_Flash_lock();

	/*
	 * checking APB1 clock to set its prescaler
	 * (it's of maximum value of 36MHz)
	 *
	 * Notice: This part is skipped to save code size. APB1 prescaler is 2.
	 */
	EDT_REG(RCC->CFGR, RCC_PPRE1_0, 4, 3);

	/*	Setting system clock switch	*/
	EDT_REG(RCC->CFGR, RCC_SW_0, RCC_SYS_SOURCE, 2);
	while(
		(GET_BIT(RCC->CFGR, RCC_SWS_0) | (GET_BIT(RCC->CFGR, RCC_SWS_1)<<1)) !=
		RCC_SYS_SOURCE);
	/*	~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~		*/
}

/*	inits PLL (target dependent)	*/
#if RCC_PLL_EN == 1
								/*	! ! ! ! ! ! N O T E ! ! ! ! ! !	*/
/*	THIS FUUNCTION DOES NOT IMPLEMENT PLL2 & PLL3 CONFIGURATION, TO BE IMPLEMENTED NEXT VERSION	*/
void RCC_voidInitPLL(void)
{
	/*	if auto configure mode	*/
	#ifdef RCC_PLL_OUT
		#if RCC_PLL_CLK_SOURCE != RCC_PLLSOURCE_PLLXTPRE
		#error "Auto PLL config mode works when only when RCC_PLL_CLK_SOURCE == RCC_PLLSOURCE_PLLXTPRE"
		#endif

		/*	try PLLXTPRE = 1	*/
		#define PLLMUL1 	(RCC_PLL_OUT / (RCC_HSE_CLK / 2))
		#if PLLMUL1 < 2 || PLLMUL1 > 16
		#define F1			0
		#else
		#define F1			(PLLMUL1 * (RCC_HSE_CLK / 2))
		#endif
		/*	try PLLXTPRE = 0	*/
		#define PLLMUL0 	(RCC_PLL_OUT / RCC_HSE_CLK)
		#if PLLMUL0 < 2 || PLLMUL0 > 16
		#define F0			0
		#else
		#define F0			(PLLMUL0 * RCC_HSE_CLK)
		#endif
		/*	if impossible	*/
		#if F0 == 0 && F1 == 0
		#error "selected PLL output freq is impossible!"
		#endif
		/*	compare	*/
		#if (RCC_PLL_OUT - F1) < 0
			#define D1			(-(RCC_PLL_OUT - F1))
		#else
			#define D1			(RCC_PLL_OUT - F1)
		#endif

		#if (RCC_PLL_OUT - F0) < 0
			#define D0			(-(RCC_PLL_OUT - F0))
		#else
			#define D0			(RCC_PLL_OUT - F0)
		#endif

		#if D0 < D1
			#define RCC_PLLMUL			(RCC_PLL_OUT / RCC_HSE_CLK)
			#define RCC_PLLXTPRESOURCE	0
		#else
			#define RCC_PLLMUL			(RCC_PLL_OUT / (RCC_HSE_CLK / 2))
			#define RCC_PLLXTPRESOURCE	1
		#endif
		#undef	PLLMUL1
		#undef	PLLMUL0
		#undef	F1
		#undef	F0
		#undef	D1
		#undef	D0
	#endif

	/*	setting PLLXTPRE clock source (if chosen)	*/
	#if RCC_PLL_CLK_SOURCE == RCC_PLLSOURCE_PLLXTPRE
		WRT_BIT(RCC->CFGR, RCC_PLLSRC, RCC_PLLSOURCE_PLLXTPRE);
		WRT_BIT(RCC->CFGR, RCC_PLLXTPRE, RCC_PLLXTPRESOURCE);
		#define PLL_SRC		RCC_HSE_CLK / (1 + RCC_PLLXTPRESOURCE)
	/*	else, PLL source is HSI by 2	*/
	#else
		WRT_BIT(RCC->CFGR, RCC_PLLSRC, 0);
		#define PLL_SRC		RCC_HSI_CLK / 2
	#endif

	/*	setting PLL MUL	*/
	#define PLL_OUT	(PLL_SRC * RCC_PLLMUL)
	#if PLL_OUT > 72000000
		#error "configured frequency is above maximum possible!"
	#endif
	EDT_REG(RCC->CFGR, RCC_PLLMUL_0, RCC_PLLMUL - 2, 4);

	/*	storing PLL output freq, to be used in sime functions	*/
	//pllOutputFreq = PLL_OUT;

	/*undef un-necessary defines	*/
	#undef  PLL_SRC
	#undef  PLL_OUT
}
#endif

















/*
 * RCC_config.h
 *
 *  Created on:	Jul 26, 2022
 *  Author:		Ali Emad Ali
 *
 *  References:
 *  	STM RM0368:	6.	Reset and clock control (RCC) for STM32F401xB/C and STM32F401xD/E
 */

#ifndef RCC_CONFIG_H_
#define RCC_CONFIG_H_


/*	values of external clocks (if used)	*/
#define RCC_HSE_CLK		8000000
#define RCC_LSE_CLK		40000
/*	~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~		*/

/*	enable switches of clock sources	*/
#define RCC_HSI_EN		0
#define RCC_HSE_EN		1
#define RCC_PLL_EN		1
#define RCC_CSS_EN		0
/*	~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~		*/

/**
 * the clock source to use as system clock source
 * remember:
 * typedef enum{
 *	 RCC_SYSCLOCKSWITCH_HSI = 0,
 *	 RCC_SYSCLOCKSWITCH_HSE = 1,
 *	 RCC_SYSCLOCKSWITCH_PLL = 2,
 * }RCC_SYSCLOCKSWITCH;
 */
#define RCC_SYS_SOURCE	RCC_SYSCLOCKSWITCH_PLL
/*	~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~		*/

/*	HSE bypass (if used)	*/
#if RCC_HSE_EN
#define RCC_HSE_BYPASSED	0
#endif	/*	RCC_HSE_EN	*/
/*	~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~		*/

/**
 * PLL configuration (if used)
 * remember:
 *  typedef enum{
 *	 RCC_PLLSOURCE_HSI = 0,
 *	 RCC_PLLSOURCE_HSE = 1
 * }RCC_PLLSOURCE;
 * and:
 *  typedef enum{
 *   RCC_PLLDEVISIONFACTOR_2 = 0,
 *   RCC_PLLDEVISIONFACTOR_4 = 1,
 *   RCC_PLLDEVISIONFACTOR_6 = 2,
 *   RCC_PLLDEVISIONFACTOR_8 = 3,
 * }RCC_PLLSYSDEVISIONFACTOR;
 */
#if RCC_PLL_EN
#define RCC_PLL_CLK_SOURCE	RCC_PLLSOURCE_PLLXTPRE

/*
 * uncomment "RCC_PLL_OUT", and comment
 * "RCC_PLLMUL", "RCC_PLLXTPRESOURCE" to automatically
 * select PLL parameters to the ones which achieve the
 * nearest output frequency possible.
 *
 * the real running PLL frequency could be obtained during
 * runtime using "RCC_u32GetPllClk()"
 */
//#define RCC_PLL_OUT							72000000
/*	from 2 to 16	*/
#define RCC_PLLMUL						9
#define RCC_PLLXTPRESOURCE				RCC_PLLXTPRESOURCE_HSE

#endif /* RCC_PLL_EN */
/*	~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~		*/



#endif /* RCC_CONFIG_H_ */

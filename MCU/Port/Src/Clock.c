/*
 * Clock.c
 *
 *  Created on: Sep 25, 2023
 *      Author: Ali Emad
 */

#include <stdint.h>
#include "Assert.h"
#include "Clock.h"
#include "RCC_interface.h"


void vPort_Clock_initCpuClock(void)
{
	/*	Initialize Backup-domain clock	*/
	__HAL_RCC_PWR_CLK_ENABLE();
	__HAL_RCC_BKP_CLK_ENABLE();

	/*	Initialize flash controller clock	*/
	__HAL_RCC_FLITF_CLK_ENABLE();

	/*
	 * Initialize CPU clock source to PLL @72Mhz using the external 8MHz crystal
	 *
	 * Notice:
	 * For CPU buses clock initialization, custom pre-calculated operations are
	 * done. This saves 2KBytes when compared to the case at which STM32_HAL driver
	 * is used.
	 */
	RCC_voidSysClockInit();
}

void vPort_Clock_initPeriphClock(void)
{
	/*	Initialize GPIO	clock	*/
	__HAL_RCC_GPIOA_CLK_ENABLE();
	__HAL_RCC_GPIOB_CLK_ENABLE();
	__HAL_RCC_GPIOC_CLK_ENABLE();
	__HAL_RCC_AFIO_CLK_ENABLE();

	/*	Initialize USB clock	*/
	RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};
	PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_USB;
	PeriphClkInit.UsbClockSelection = RCC_USBCLKSOURCE_PLL_DIV1_5;
	vLib_ASSERT(HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) == HAL_OK, 0);
	__HAL_RCC_USB_CLK_ENABLE();
}













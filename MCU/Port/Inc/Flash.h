/*
 * Flash.h
 *
 *  Created on: Oct 24, 2023
 *      Author: Ali Emad
 *
 */

#ifndef PORT_FLASH_H_
#define PORT_FLASH_H_

#include "stm32f1xx_hal_flash_ex.h"
#include "stm32f1xx_hal_flash.h"


#define puiPORT_FLASH_START_ADDRESS					((uint32_t*)0x08000000)
#define uiPORT_FLASH_PAGE_SIZE_IN_BYTES				((uint32_t)1024)
#define uiPORT_FLASH_TOTAL_NUMBER_OF_PAGES			((uint32_t)64)
#define uiPORT_FLASH_BOOTLOADER_SIZE_IN_PAGES		((uint32_t)25)

#define ucPORT_FLASH_IS_BUSY()	\
	(READ_BIT(FLASH->SR, FLASH_SR_BSY))

void vPort_Flash_unlock(void);

void vPort_Flash_lock(void);

void vPort_Flash_erasePage(uint32_t uiPageNumber);

void vPort_Flash_programWord(uint32_t* puiAddress, uint32_t uiWord);



#endif /* PORT_FLASH_H_ */

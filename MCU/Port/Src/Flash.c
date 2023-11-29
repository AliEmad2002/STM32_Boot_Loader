/*
 * Flash.c
 *
 *  Created on: Oct 24, 2023
 *      Author: Ali Emad
 */

#include "Flash.h"

void vPort_Flash_unlock(void)
{
	/*	While flash is still locked	*/
	while(READ_BIT(FLASH->CR, FLASH_CR_LOCK) != 0)
	{
		/*	Write the unlocking seq.	*/
	    WRITE_REG(FLASH->KEYR, FLASH_KEY1);
	    WRITE_REG(FLASH->KEYR, FLASH_KEY2);
	}
}


void vPort_Flash_lock(void)
{
	while(ucPORT_FLASH_IS_BUSY());

	/*	Lock flash	*/
	SET_BIT(FLASH->CR, FLASH_CR_LOCK);
}

void vPort_Flash_erasePage(uint32_t uiPageNumber)
{
	/*	Wait until FPEC is free	*/
	while(ucPORT_FLASH_IS_BUSY());

	/*	Disable programming  mode	*/
	CLEAR_BIT(FLASH->CR, FLASH_CR_PG);

	/*	Enable page erase mode	*/
	SET_BIT(FLASH->CR, FLASH_CR_PER);

	/*	Write page number	*/
	FLASH->AR = uiPageNumber * uiPORT_FLASH_PAGE_SIZE_IN_BYTES + (uint32_t)puiPORT_FLASH_START_ADDRESS;

	/*	Start erase operation	*/
	SET_BIT(FLASH->CR, FLASH_CR_STRT);
}

void vPort_Flash_programWord(uint32_t* puiAddress, uint32_t uiWord)
{
	/*	Wait until FPEC is free	*/
	while(ucPORT_FLASH_IS_BUSY());

	/*	Disable page erase mode	*/
	CLEAR_BIT(FLASH->CR, FLASH_CR_PER);

	/*	Enable programming  mode	*/
	SET_BIT(FLASH->CR, FLASH_CR_PG);

	((uint16_t*)(puiAddress))[0] = (uint16_t)((uiWord) & 0xFFFF);

	while(ucPORT_FLASH_IS_BUSY());
	((uint16_t*)(puiAddress))[1] = (uint16_t)((uiWord) >> 16);
}

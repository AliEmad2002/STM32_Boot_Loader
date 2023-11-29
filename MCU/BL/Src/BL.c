/*
 * BL.c
 *
 * Created: 	22/10/2023
 * Author : 	Ali Emad Ali
 */

/*	LIB	*/
#include <stdint.h>

/*	Port	*/
#include "Clock.h"
#include "DIO.h"
#include "Flash.h"
#include "Inline.h"
#include "Host_Communication.h"

/*	SELF	*/
#include "BL.h"

/*******************************************************************************
 * Static (Private) functions and variables:
 ******************************************************************************/
static char pcBuffer[MAX_BUFFER_SIZE];

/*******************************************************************************
 * API functions:
 ******************************************************************************/
/*
 * See header for info.
 */
PORT_INLINE_ALWAYS
void vBL_initStartup(void)
{
	vPort_DIO_initProgrammingEnablePin();
}

/*
 * See header for info.
 */
void vBL_initProgramming(void)
{
	/*	Clock	*/
	vPort_Clock_initCpuClock();
	vPort_Clock_initPeriphClock();

	/*	Host communication	*/
	vPort_HostCom_init();

	/*	Flash controller	*/
	vPort_Flash_unlock();
}

/*
 * See header for info.
 */
uint32_t uiBL_receiveProgram(void)
{
	Hex_Record_t record;
	uint32_t startingExecutionAddress = 0;
	uint32_t highPartAddress = 0;
	uint32_t address;
	uint32_t i;
	char cByte;

	/*	erase pages from end of the bootloader section to end of the flash	*/
	for (	i = uiPORT_FLASH_BOOTLOADER_SIZE_IN_PAGES;
			i < uiPORT_FLASH_TOTAL_NUMBER_OF_PAGES;
			i++	)
	{
		vPort_Flash_erasePage(i);
	}

	while(1)
	{
		/*	receive until '\n'	*/
		for (i = 0; i < MAX_BUFFER_SIZE; i++)
		{
			cByte
		}

		/*	parse	*/
		if (!Hex_Parser_b8Parse(str, &record))
		{
			/*	ask for re-send	*/
			(void)UART_enumSendByte(UART_UnitNumber_2, 'F');
			continue;
		}

		switch(record.type)
		{
		case Hex_Record_Type_Data:
			/*	flash	*/
			address = highPartAddress | (u32)record.lowPartAddress;
			FPEC_voidProgram(address, record.data, record.charCount / 2);
			if (record.charCount % 2 != 0)
			{
				address += (record.charCount / 2) * 2;
				FPEC_voidProgramHalfWord(
					address, record.data[record.charCount / 2] | 0xFF00);
			}
			break;

		case Hex_Record_Type_HighPartAddress:
			highPartAddress = (u32)record.data[0] << 16;
			break;

		case Hex_Record_Type_StartingExecutionAddress:
			startingExecutionAddress =
				((u32)record.data[0] << 16) | (u32)record.data[1];
			/*	store to flash	*/
			address = FPEC_PAGE_ADDRESS(BOOT_LOADER_SIZE_IN_KB) - 4;
			FPEC_voidProgramWord(address, startingExecutionAddress);
			break;

		case Hex_Record_Type_EndOfFile:
			/*	ack on receiving last record	*/
			(void)UART_enumSendByte(UART_UnitNumber_2, 'N');
			/*	little delay before disabling UART	*/
			Delay_voidBlockingDelayMs(50);
			/*	disable programming mode	*/
			FPEC_voidDisableProgrammingMode();
			/*	lock FPEC	*/
			FPEC_voidLock();
			/*	disable used peripherals	*/
			RCC_voidDisablePeripheralClk(RCC_Bus_AHB, RCC_PERIPHERAL_FLITF);
			RCC_voidDisablePeripheralClk(RCC_Bus_APB2, RCC_PERIPHERAL_IOPA);
			RCC_voidDisablePeripheralClk(RCC_Bus_APB2, RCC_PERIPHERAL_AFIO);
			RCC_voidDisablePeripheralClk(RCC_Bus_APB1, RCC_PERIPHERAL_USART2);
			/*	return starting execution address	*/
			return startingExecutionAddress;
		}

		/*	ask for next record	*/
		(void)UART_enumSendByte(UART_UnitNumber_2, 'N');
	}
}
//
//u32 Boot_Loader_u32GetStoredStartingExecutionAddress(void)
//{
//	/*	enable FPEC	*/
//	RCC_voidEnablePeripheralClk(RCC_Bus_AHB, RCC_PERIPHERAL_FLITF);
//
//	/*	unlock FPEC	*/
//	FPEC_voidUnlock();
//
//	/*	enter programming mode	*/
//	FPEC_voidEnableProgrammingMode();
//
//	/*	read address	*/
//	u32 address = FPEC_PAGE_ADDRESS(BOOT_LOADER_SIZE_IN_KB) - 4;
//	address = FPEC_u32ReadWord(address);
//
//	/*	disable programming mode	*/
//	FPEC_voidDisableProgrammingMode();
//
//	/*	lock FPEC	*/
//	FPEC_voidLock();
//
//	/*	disable FPEC	*/
//	RCC_voidDisablePeripheralClk(RCC_Bus_AHB, RCC_PERIPHERAL_FLITF);
//
//	return address;
//}
//
///*
// * gives host flasher time to enter programming key, disables sysTick on exit.
// */
//b8 Boot_Loader_b8GiveChanceToUnlock(u16 msTimeout)
//{
//	char str[50];
//
//	b8 unlocked = UART_b8ReceiveStringTimeout(
//		UART_UnitNumber_2, str, msTimeout, KEY_STRING);
//
//	STK_voidStopTickMeasure(STK_TickMeasureType_OverflowCount);
//	STK_voidDisableSysTick();
//
//	return unlocked;
//}

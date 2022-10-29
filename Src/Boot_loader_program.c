/*
 * Boot_loader_program.c
 *
 * Created: 	26/10/2022
 * Author : 	Ali Emad Ali
 */ 

/*	LIB	*/
#include "STD_TYPES.h"
#include "BIT_MATH.h"
#include "Hex_Parser.h"
#include "Delay_interface.h"
#include "Error_Handler_interface.h"

/*	MCAL	*/
#include "RCC_interface.h"
#include "GPIO_interface.h"
#include "NVIC_interface.h"
#include "UART_interface.h"
#include "FPEC_interface.h"
#include "STK_interface.h"

/*	SELF	*/
#include "Boot_loader_interface.h"
#include "Boot_Loader_config.h"
#include "Boot_Loader_private.h"

/*
 * inits system clock to HSI, not disabling or configuring any of RCC's related
 * clock selection configurations, to avoid conflicting user's - to come - SW.
 *
 * inits UART2 with the baudrate configured in the ".config" file.
 */
void Boot_Loader_voidInit(void)
{
	/*	will not change RCC clock default settings	*/

	/*	enable AFIO, GPIOA, UART2, BKP, PWR	*/
	RCC_voidEnablePeripheralClk(RCC_Bus_APB2, RCC_PERIPHERAL_IOPA);
	RCC_voidEnablePeripheralClk(RCC_Bus_APB2, RCC_PERIPHERAL_AFIO);
	RCC_voidEnablePeripheralClk(RCC_Bus_APB1, RCC_PERIPHERAL_USART2);

	/*	init UART corresponding pins as AF pins	*/
	GPIO_voidSetPinMode(		GPIO_PortName_A, 2, GPIO_Mode_AF_PushPull);
	GPIO_voidSetPinOutputSpeed(	GPIO_PortName_A, 2, GPIO_OutputSpeed_50MHz);
	GPIO_voidSetPinMode(		GPIO_PortName_A, 3, GPIO_Mode_AF_PushPull);
	GPIO_voidSetPinOutputSpeed(	GPIO_PortName_A, 3, GPIO_OutputSpeed_Null);
	AFIO_voidRemap(AFIO_Peripheral_USART2, AFIO_Usart2_Map_TxA2_RxA3);

	/*	init UART unit	*/
	UART_voidFastInit(UART_UnitNumber_2, BAUD_RATE);

	/*	enable STK in tick measure mode	*/
	STK_voidInit(STK_ClockSource_AHB_by8, 0);
	STK_voidEnableSysTick();
	STK_voidStartTickMeasure(STK_TickMeasureType_OverflowCount);
}

/*
 * enables FPEC, erases all pages following the
 * boot-loader section, receives, checks and parses data into flash memory.
 * then disables FPEC, UART, GPIOA, and AFIO
 * returns starting execution address.
 */
u32 Boot_Loader_u32EnterProgrammingMode(void)
{
	/*	enable FPEC	*/
	RCC_voidEnablePeripheralClk(RCC_Bus_AHB, RCC_PERIPHERAL_FLITF);

	/*	unlock FPEC	*/
	FPEC_voidUnlock();

	/*	enable erase mode	*/
	FPEC_voidEnablePageEraseMode();

	/*	erase pages from after the boot-loader to 63	*/
	FPEC_voidEraseMultiplePages(BOOT_LOADER_SIZE_IN_KB, 63);

	/*	disable erase mode	*/
	FPEC_voidDisablePageEraseMode();

	/*	enable programming mode	*/
	FPEC_voidEnableProgrammingMode();

	char str[MAX_STR_LEN];
	Hex_Record_t record;
	u32 startingExecutionAddress = 0x08000000;
	u32 highPartAddress = 0x08000000;
	u32 address;
	while(1)
	{
		/*	receive until '\n'	*/
		UART_voidReceiveUntilByte(UART_UnitNumber_2, str, '\n');

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

u32 Boot_Loader_u32GetStoredStartingExecutionAddress(void)
{
	/*	enable FPEC	*/
	RCC_voidEnablePeripheralClk(RCC_Bus_AHB, RCC_PERIPHERAL_FLITF);

	/*	unlock FPEC	*/
	FPEC_voidUnlock();

	/*	enter programming mode	*/
	FPEC_voidEnableProgrammingMode();

	/*	read address	*/
	u32 address = FPEC_PAGE_ADDRESS(BOOT_LOADER_SIZE_IN_KB) - 4;
	address = FPEC_u32ReadWord(address);

	/*	disable programming mode	*/
	FPEC_voidDisableProgrammingMode();

	/*	lock FPEC	*/
	FPEC_voidLock();

	/*	disable FPEC	*/
	RCC_voidDisablePeripheralClk(RCC_Bus_AHB, RCC_PERIPHERAL_FLITF);

	return address;
}

/*
 * gives host flasher time to enter programming key, disables sysTick on exit.
 */
b8 Boot_Loader_b8GiveChanceToUnlock(u16 msTimeout)
{
	char str[50];

	b8 unlocked = UART_b8ReceiveStringTimeout(
		UART_UnitNumber_2, str, msTimeout, KEY_STRING);

	STK_voidStopTickMeasure(STK_TickMeasureType_OverflowCount);
	STK_voidDisableSysTick();

	return unlocked;
}

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
#include "USB.h"
#include "Inline.h"

/*	SELF	*/
#include "BL.h"


/*
 * See header for info.
 */
PORT_INLINE_ALWAYS
void vBL_Init_Startup(void)
{
	/*	Initialize programming mode enable pin as pulled down input	*/
	vPort_DIO_initPinInput(	confPROGRAMMING_MODE_EN_PORT,
							confPROGRAMMING_MODE_EN_PIN,
							2	);
}

/*
 * See header for info.
 */
void vBL_Init_Programming(void)
{
	/*	Clock	*/
	vPort_Clock_initCpuClock();
	vPort_Clock_initPeriphClock();

	/*	USB	*/
	vPort_USB_initHardware();

	/*	Flash controller	*/

}

///*
// * enables FPEC, erases all pages following the
// * boot-loader section, receives, checks and parses data into flash memory.
// * then disables FPEC, UART, GPIOA, and AFIO
// * returns starting execution address.
// */
//u32 Boot_Loader_u32EnterProgrammingMode(void)
//{
//	/*	enable FPEC	*/
//	RCC_voidEnablePeripheralClk(RCC_Bus_AHB, RCC_PERIPHERAL_FLITF);
//
//	/*	unlock FPEC	*/
//	FPEC_voidUnlock();
//
//	/*	enable erase mode	*/
//	FPEC_voidEnablePageEraseMode();
//
//	/*	erase pages from after the boot-loader to 63	*/
//	FPEC_voidEraseMultiplePages(BOOT_LOADER_SIZE_IN_KB, 63);
//
//	/*	disable erase mode	*/
//	FPEC_voidDisablePageEraseMode();
//
//	/*	enable programming mode	*/
//	FPEC_voidEnableProgrammingMode();
//
//	char str[MAX_STR_LEN];
//	Hex_Record_t record;
//	u32 startingExecutionAddress = 0x08000000;
//	u32 highPartAddress = 0x08000000;
//	u32 address;
//	while(1)
//	{
//		/*	receive until '\n'	*/
//		UART_voidReceiveUntilByte(UART_UnitNumber_2, str, '\n');
//
//		/*	parse	*/
//		if (!Hex_Parser_b8Parse(str, &record))
//		{
//			/*	ask for re-send	*/
//			(void)UART_enumSendByte(UART_UnitNumber_2, 'F');
//			continue;
//		}
//
//		switch(record.type)
//		{
//		case Hex_Record_Type_Data:
//			/*	flash	*/
//			address = highPartAddress | (u32)record.lowPartAddress;
//			FPEC_voidProgram(address, record.data, record.charCount / 2);
//			if (record.charCount % 2 != 0)
//			{
//				address += (record.charCount / 2) * 2;
//				FPEC_voidProgramHalfWord(
//					address, record.data[record.charCount / 2] | 0xFF00);
//			}
//			break;
//
//		case Hex_Record_Type_HighPartAddress:
//			highPartAddress = (u32)record.data[0] << 16;
//			break;
//
//		case Hex_Record_Type_StartingExecutionAddress:
//			startingExecutionAddress =
//				((u32)record.data[0] << 16) | (u32)record.data[1];
//			/*	store to flash	*/
//			address = FPEC_PAGE_ADDRESS(BOOT_LOADER_SIZE_IN_KB) - 4;
//			FPEC_voidProgramWord(address, startingExecutionAddress);
//			break;
//
//		case Hex_Record_Type_EndOfFile:
//			/*	ack on receiving last record	*/
//			(void)UART_enumSendByte(UART_UnitNumber_2, 'N');
//			/*	little delay before disabling UART	*/
//			Delay_voidBlockingDelayMs(50);
//			/*	disable programming mode	*/
//			FPEC_voidDisableProgrammingMode();
//			/*	lock FPEC	*/
//			FPEC_voidLock();
//			/*	disable used peripherals	*/
//			RCC_voidDisablePeripheralClk(RCC_Bus_AHB, RCC_PERIPHERAL_FLITF);
//			RCC_voidDisablePeripheralClk(RCC_Bus_APB2, RCC_PERIPHERAL_IOPA);
//			RCC_voidDisablePeripheralClk(RCC_Bus_APB2, RCC_PERIPHERAL_AFIO);
//			RCC_voidDisablePeripheralClk(RCC_Bus_APB1, RCC_PERIPHERAL_USART2);
//			/*	return starting execution address	*/
//			return startingExecutionAddress;
//		}
//
//		/*	ask for next record	*/
//		(void)UART_enumSendByte(UART_UnitNumber_2, 'N');
//	}
//}
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

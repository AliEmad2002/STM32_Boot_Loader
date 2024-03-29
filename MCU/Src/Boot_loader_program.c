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
#include <stdlib.h>

/*	MCAL	*/
#include "RCC_interface.h"
#include "GPIO_interface.h"
#include "NVIC_interface.h"
#include "UART_interface.h"
#include "FPEC_interface.h"
#include "STK_interface.h"

/*	HAL	*/
#include "WiFi_interface.h"

/*	SELF	*/
#include "Boot_loader_interface.h"
#include "Boot_Loader_config.h"
#include "Boot_Loader_private.h"

WiFi_t* esp8266Ptr;

/*
 * After getting new version number from server, it can't be immediately stored
 * in flash in location of "current version on flash" (because it is not yet,
 * and by doing that, MCU would believe it is updated even if the update failed!).
 * Instead, it is stored temporarily in RAM, and if update succeeds, it is flashed.
 */
u16 Global_tempOnlineVersionNumber;

void Boot_Loader_voidInit(void)
{
	/*
	 * Remember to configure RCC on HSI, to give APP code freedom of configuring
	 * RCC later.
	 */
	RCC_voidSysClockInit();

	/*	power stabilization delay	*/
	Delay_voidBlockingDelayMs(1000);

	/*	enable FPEC	*/
	RCC_voidEnablePeripheralClk(RCC_Bus_AHB, RCC_PERIPHERAL_FLITF);

	/*	enable STK in tick measure mode	*/
	STK_voidInit();
	STK_voidEnableSysTick();
	STK_voidStartTickMeasure(STK_TickMeasureType_OverflowCount);

	/*	init WiFi module	*/
	/**
	 * As the WiFi module uses large buffer space, allocating its object dynamically
	 * and free-ing it at end of the bootloader would save RAM space for the application
	 * program.
	 *
	 * "They" say: "don't use dynamic allocation in embedded systems!"... get
	 * a solid "why" about that topic. TODO
	 **/
	esp8266Ptr = malloc(sizeof(WiFi_t));

	WiFi_voidInit(
		esp8266Ptr, ESP8266_RST_PIN, ESP8266_UART_UNIT_NUMBER,
		115200, ESP8266_UART_AFIO_MAP);
}

b8 Boot_Loader_b8ConnectToFtpServer(void)
{
	/*	wait for module to be ready	*/
	while(!WiFi_b8IsModuleAvailable(esp8266Ptr));

	Delay_voidBlockingDelayMs(2);	//	Avoiding "non-valid echo" problem.

	/*	reset module	*/
	while(!WiFi_b8SoftReset(esp8266Ptr));

	Delay_voidBlockingDelayMs(500);	//	Avoiding "non-valid echo" problem.

	/*	connect to WiFi network	*/
	while(!WiFi_b8SelectMode(esp8266Ptr, WiFi_Mode_SoftAP_Station, true));

	Delay_voidBlockingDelayMs(2);	//	Avoiding "non-valid echo" problem.

	while(!WiFi_b8ConnectToAP(esp8266Ptr, WIFI_SSID, WIFI_PASS, true));

	Delay_voidBlockingDelayMs(100);	//	Avoiding "non-valid echo" problem.

	/*	enable multiple connections	*/
	while(!WiFi_b8SetMultipleConnections(esp8266Ptr, true));

	Delay_voidBlockingDelayMs(2);

	volatile b8 commandSuccess;

	/*	connect to FTP server	*/
	commandSuccess =
		WiFi_b8ConnectToFTP(esp8266Ptr, FTP_IP, FTP_PORT, FTP_USER, FTP_PASS, 0);

	if (!commandSuccess)
		return false;

	else
		return true;
}

b8 Boot_Loader_b8IsOnlineUpdateAvailable(void)
{
	/**
	 * get number of current version available on server.
	 * (stored in "Global_tempOnlineVersionNumber").
	 **/
	if(!Boot_Loader_b8GetVersionAvailableOnline())
	{
		/*	failed to get version from server	*/
		return false;
	}

	/**	get number of current version on flash memory	**/
	u16 onFlashVersion = Boot_Loader_u16GetVersionAvailableOnFlash();

	/**	compare	**/
	return (Global_tempOnlineVersionNumber > onFlashVersion);
}

void Boot_Loader_voidEnterProgrammingMode(void)
{
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

	u8 i = 0;

	while(1)
	{
		/*	download i-th ".hex" chunk	*/
		Boot_Loader_voidDownloadChunk(i);

		/*	parse whole chunk	*/
		BootLoader_ChunkParsingResult_t parseResult =
			Boot_Loader_enumParseLastDownloadedChunk();

		switch(parseResult)
		{
		/*	if the chunk parsed was the last one, return	*/
		case BootLoader_ChunkParsingResult_WasLast:
			return;

		/*	if parsing succeeded, increment 'i' to download the next chunk	*/
		case BootLoader_ChunkParsingResult_Successful:
			i++;
			break;

		/*	if parsing failed, re-download the same chunk (don't increment 'i')	*/
		case BootLoader_ChunkParsingResult_Failed:
			break;
		}

		/*	wait before downloading next chunk	*/
		Delay_voidBlockingDelayMs(100);
	}
}

void Boot_Loader_voidUpdateVersionNumberOnFlash(void)
{
	/*	unlock flash	*/
	FPEC_voidUnlock();

	/*	copy last page to RAM	*/
	FPEC_voidEnableProgrammingMode();

	/// TODO: dynamic here is used!
	u16* lastPagePtr = malloc(2 * 1024 * sizeof(u16));

	for (u16 i = 0; i < 2 * 1024; i++)
	{
		lastPagePtr[i] =
			FPEC_u32ReadWord(
				FPEC_HALF_WORD_ADDRESS(BOOT_LOADER_SIZE_IN_KB - 1, i));
	}

	FPEC_voidDisableProgrammingMode();

	/*	erase last page on flash	*/
	FPEC_voidEnablePageEraseMode();

	FPEC_voidErasePage(BOOT_LOADER_SIZE_IN_KB - 1);

	FPEC_voidDisablePageEraseMode();

	/*	write new version number to the page stored on RAM	*/
	/**
	 * Notice that it is  stored negated to result in zero version number if the
	 * half-word was empty.
	 **/
	lastPagePtr[509] = ~Global_tempOnlineVersionNumber;

	/*	write the page stored on RAM to flash	*/
	FPEC_voidEnableProgrammingMode();

	for (u16 i = 0; i < 2 * 1024; i++)
	{
		FPEC_voidProgramHalfWord(
			FPEC_HALF_WORD_ADDRESS(BOOT_LOADER_SIZE_IN_KB - 1, i),
			lastPagePtr[i]);
	}

	FPEC_voidDisableProgrammingMode();

	/*	delete page stored on RAM	*/
	free(lastPagePtr);

	/*	lock flash	*/
	FPEC_voidLock();
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

void Boot_Loader_voidFreeMem(void)
{
	free(esp8266Ptr);
}

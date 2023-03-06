/*
 * Boot_Loader_Private.c
 *
 *  Created on: Mar 5, 2023
 *      Author: Ali Emad Ali
 */

/*	LIB	*/
#include "STD_TYPES.h"
#include "BIT_MATH.h"
#include "Hex_Parser.h"
#include "Delay_interface.h"
#include "Error_Handler_interface.h"
#include "My_String.h"
#include <stdlib.h>
#include <stdio.h>

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


extern WiFi_t esp8266;
extern u16 Global_tempOnlineVersionNumber;


b8 Boot_Loader_b8GetVersionAvailableOnline(void)
{
	/*	check connection to server	*/
	WiFi_Status_t s;
	WiFi_b8GetStatus(&esp8266, &s);

	/*	if server was not connected, reconnect	*/
	if (!s.link[0].isUsed)
	{
		/*	try connection	*/
		b8 connected =
			WiFi_b8ConnectToFTP(
				&esp8266, FTP_IP, FTP_PORT, FTP_USER, FTP_PASS, 0);

		/*	if can't connect	*/
		if (!connected)
			return false;
	}

	/*	download content of "ver.txt"	*/
	b8 downloaded =
		WiFi_b8DownloadSmallFtpFile(
			&esp8266, 0, 1, WiFi_FtpFile_Ascii, "ver.txt");

	/*	if could not download	*/
	if (!downloaded)
		return false;

	/*	if connect, and download done successfully	*/
	Global_tempOnlineVersionNumber = atoi(esp8266.buffer);

	return true;
}

u16 Boot_Loader_u16GetVersionAvailableOnFlash(void)
{
	/*	unlock flash	*/
	FPEC_voidUnlock();

	/*	enter programming mode	*/
	FPEC_voidEnableProgrammingMode();

	/*	get address of version number	*/
	u32 address = FPEC_HALF_WORD_ADDRESS(BOOT_LOADER_SIZE_IN_KB - 1, 509);

	/*	get version number	*/
	/**
	 * Notice that it is  stored negated to result in zero version number if the
	 * half-word was empty.
	 **/
	u16 ver = ~FPEC_u16ReadHalfWord(address);

	/*	disable programming mode	*/
	FPEC_voidDisableProgrammingMode();

	/*	lock flash	*/
	FPEC_voidLock();

	return ver;
}

void Boot_Loader_voidDownloadChunk(u8 chunkIndex)
{
	/*	create string of file name	*/
	char fileNameStr[8];

	sprintf(fileNameStr, "%u.hex", (u32)chunkIndex);

	/*	try downloading that file, while it is not, try again	*/
	while(1)
	{
		b8 downloadSuccessful =
			WiFi_b8DownloadSmallFtpFile(
				&esp8266, 0, 1, WiFi_FtpFile_Binary, fileNameStr);

		/*
		 * if file could not be downloaded, restart FTP connection.
		 */
		if (!downloadSuccessful)
		{
			while(!Boot_Loader_b8ConnectToFtpServer());
		}

		/*	otherwise if downloaded successfully, return	*/
		else
		{
			return;
		}
	}
}

BootLoader_ChunkParsingResult_t Boot_Loader_enumParseLastDownloadedChunk(void)
{
	s16 recordStartingIndex = 0;
	u32 address;
	u32 executionStartingAddress = 0x08000000;
	static u32 highPartAddress = 0x08000000;

	while(1)
	{
		/*
		 * find starting index of the record (first ':' after the last index).
		 */
		recordStartingIndex =
			String_s16Find(esp8266.buffer, ':', recordStartingIndex);

		/*	if there's no next record, chunk is ended	*/
		if (recordStartingIndex == -1)
			return BootLoader_ChunkParsingResult_Successful;

		/*	try to parse the record	*/
		Hex_Record_t record;

		b8 recordParsedSuccessfully =
			Hex_Parser_b8Parse(&esp8266.buffer[recordStartingIndex], &record);

		/*	if record could not be parsed	*/
		if (!recordParsedSuccessfully)
		{
			return BootLoader_ChunkParsingResult_Failed;
		}

		/*	Otherwise, if record was successfully parsed	*/
		switch(record.type)
		{
		case Hex_Record_Type_Data:
			/*	flash	*/
			address = highPartAddress | (u32)record.lowPartAddress;

			FPEC_voidProgram(address, record.data, record.charCount / 2);

			/*
			 * edge case: odd number of bytes require a zero at the byte next
			 * to the last. Because bytes in flash are set to 0xFF by default
			 * after erased.
			 */
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
			executionStartingAddress =
				((u32)record.data[0] << 16) | (u32)record.data[1];

			/*
			 * store to flash (to be used at startup to know where APP code
			 * exists, in case user flash a code that starts somewhere after the
			 * default address).
			 */
			address = FPEC_PAGE_ADDRESS(BOOT_LOADER_SIZE_IN_KB) - 4;

			FPEC_voidProgramWord(address, executionStartingAddress);

			break;

		case Hex_Record_Type_EndOfFile:
			/*	disable programming mode	*/
			FPEC_voidDisableProgrammingMode();

			/*	lock FPEC	*/
			FPEC_voidLock();

			return BootLoader_ChunkParsingResult_WasLast;
		}

		/*	increment index, to not stumble in the same ':' twice in a row	*/
		recordStartingIndex++;
	}
}






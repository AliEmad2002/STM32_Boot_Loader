/*
 * Boot_loader_main.c
 *
 * Created: 		26 Oct 2022
 * Author:			Ali Emad.
 */

/*	LIB	*/
#include "Std_Types.h"
#include "Bit_Math.h"

/*	MCAL	*/
#include "RCC_interface.h"
#include "BKP_interface.h"
#include "SCB_interface.h"
#include "GPIO_interface.h"
#include "UART_interface.h"

/*	APP	*/
#include "Boot_loader_interface.h"
#include "Boot_Loader_config.h"
#include "Boot_Loader_private.h"


int main(void)
{
	/*	init boot loader	*/
	Boot_Loader_voidInit();

	/*	try connecting to update FTP server	*/
	b8 updateServerConnected = Boot_Loader_b8ConnectToFtpServer();

	/*	if successfully connected, check for update	*/
	if (updateServerConnected)
	{
		b8 updateAvailable = Boot_Loader_b8IsOnlineUpdateAvailable();

		/*	if there's an update	*/
		if (updateAvailable)
		{
			/*	download and flash it	*/
			Boot_Loader_voidEnterProgrammingMode();

			/*
			 * after successful update of the program, update version number
			 * stored.
			 */
			Boot_Loader_voidUpdateVersionNumberOnFlash();

			/*	end bootloader	*/
			Boot_Loader_voidFreeMem();
		}
	}

	/*
	 * let SCB know that vector table in flash is now shifted by size of the
	 * bootloader program.
	 */
	SCB_voidWriteVectorTableOffset(
		0x08000000 + 1024 * BOOT_LOADER_SIZE_IN_KB, SCB_VTOR_Code);

	/*	start APP code	*/
	//vvFunc_t start = *(vvFunc_t*)(0x08000004 + 1024 * BOOT_LOADER_SIZE_IN_KB);
	vvFunc_t start = *(vvFunc_t*)Boot_Loader_u32GetStoredStartingExecutionAddress();

	start();

	return 0;
}

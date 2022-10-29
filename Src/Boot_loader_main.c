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

	/*	send start of connection ack to the flasher	*/
	UART_enumSendByte(UART_UnitNumber_2, 'S');

	/*	wait for key sequence, timeout = 100ms	*/
	if (Boot_Loader_b8GiveChanceToUnlock(100))
	{
		/*	flash new program	*/
		(void)Boot_Loader_u32EnterProgrammingMode();
	}

	MSCB_voidWriteVectorTableOffset(
		0x08000000 + 1024 * BOOT_LOADER_SIZE_IN_KB, MSCB_VTOR_Code);

	vvFunc_t start = *(vvFunc_t*)(0x08000004 + 1024 * BOOT_LOADER_SIZE_IN_KB);

	start();

	return 0;
}

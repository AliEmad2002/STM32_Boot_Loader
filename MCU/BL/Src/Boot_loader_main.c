/*
 * Boot_loader_main.c
 *
 * Created: 		26 Oct 2022
 * Author:			Ali Emad.
 */

/*	LIB	*/
#include <stdint.h>

/*	MCAL	*/

/*	APP	*/
//#include "Boot_loader_interface.h"
//#include "Boot_Loader_config.h"
//#include "Boot_Loader_private.h"


int main(void)
{
//	/*	init boot loader	*/
//	Boot_Loader_voidInit();
//
//	/*	send start of connection ack to the flasher	*/
//	UART_enumSendByte(UART_UnitNumber_2, 'S');
//
//	/*	wait for key sequence, timeout = 100ms	*/
//	if (Boot_Loader_b8GiveChanceToUnlock(100))
//	{
//		/*	flash new program	*/
//		(void)Boot_Loader_u32EnterProgrammingMode();
//	}
//
//	/*
//	 * Give SCB the new address of the vector table.
//	 * Why? here's an example:
//	 * User uses the vector of address 0x5 + Flash start address defined in
//	 * his linker script. As this definition is not the default one, vector
//	 * table hence is not in its default place.
//	 * Hence, it is shifted it here to avoid user mistake and reduce overhead
//	 * of every time programming.
//	 */
//	MSCB_voidWriteVectorTableOffset(
//		0x08000000 + 1024 * BOOT_LOADER_SIZE_IN_KB, MSCB_VTOR_Code);
//
//	/*	start user's code	*/
//	vvFunc_t start = *(vvFunc_t*)(0x08000004 + 1024 * BOOT_LOADER_SIZE_IN_KB);
//
//	start();
//
//	return 0;
}

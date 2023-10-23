/*
 * main.c
 *
 * Created: 		24 Oct 2023
 * Author:			Ali Emad.
 */

/*	LIB	*/
#include <stdint.h>

/*	BL	*/
#include "BL.h"


int main(void)
{
	/*	Initialize boot loader startup	*/
	vBL_Init_Startup();

	/*	Check programming mode enable pin	*/
	if (ucBL_IS_PROGRAMMING_MODE_ENABLED())
	{
		/*	Initialize programming mode	*/
		vBL_Init_Programming();
	}




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

/*******************************************************************************
 * Callbacks:
 ******************************************************************************/
void Error_Handler(void)
{
	while(1);
}







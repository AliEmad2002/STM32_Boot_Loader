/*
 * BL.h
 *
 *  Created on: Oct 22, 2023
 *      Author: Ali Emad Ali
 *
 * Notes:
 *	-	As this bootloader uses USB Virtual COM port, there's no point of compiling
 *		low level USB drivers twice, once in the bootloader and another time in user
 *		app. Therefore, a USB Virtual COM port interface is provided for use in user 
 *		app. It has pointers to USB functions in the flash, which were previously 
 *		flashed when flashing the bootloader.
 *
 *	-	Same as the above is added for flash controller (FPEC).
 *
 *	-	Start-up code should not be compiled within the user app, too.
 */

#ifndef BL_H_
#define BL_H_

#include "DIO.h"

/*
 * Initializes bootloader's startup needed recourses:
 * 		-	Programming mode enable pin.
 */
void vBL_Init_Startup(void);

/*
 * Initializes bootloader's programming needed recourses:
 * 		-	USB virtual COM port.
 * 		-	CPU-Flash interface.
 */
void vBL_Init_Programming(void);

/*
 * Enables FPEC, erases all pages except for the boot-loader section, receives, checks,
 * parses and flashes data into flash memory.
 *
 * Returns starting execution address.
 */
uint32_t uiBL_EnterProgrammingMode(void);

uint32_t uiBL_GetStoredStartingExecutionAddress(void);

/*
 * Checks whether programming mode is enabled or not.
 */
#define ucBL_IS_PROGRAMMING_MODE_ENABLED() \
	(ucPORT_DIO_READ_PIN(confPROGRAMMING_MODE_EN_PORT, confPROGRAMMING_MODE_EN_PIN))



#endif /* BL_H_ */

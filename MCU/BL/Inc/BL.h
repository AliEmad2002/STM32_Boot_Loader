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
 *
 *	-	When compiling, it's recommended to set optimization for size.
 *
 *	-	Please consider that USB SW libraries consume - most often - large code size.
 *		Hence, if the application does not make use of the USB functions later, then
 *		another version of the bootloader (relying on UART for example) is recommended.
 */

#ifndef BL_H_
#define BL_H_


#include "BL_config.h"

#include "DIO.h"

/*
 * Initializes bootloader's startup needed recourses:
 * 		-	Programming mode enable pin.
 */
void vBL_initStartup(void);

/*
 * Initializes bootloader's programming needed recourses:
 * 		-	Clock configuration (Could be edited in the port folder).
 * 		-	USB virtual COM port.
 * 		-	CPU-Flash interface.
 */
void vBL_initProgramming(void);

/*
 * Erases all pages except for the boot-loader section, receives, checks, parses
 * and flashes data into flash memory.
 *
 * Returns starting execution address.
 */
uint32_t uiBL_receiveProgram(void);

uint32_t uiBL_getStoredStartingExecutionAddress(void);

/*
 * Checks whether programming mode is enabled or not.
 */
#define ucBL_IS_PROGRAMMING_MODE_ENABLED() \
	(ucPORT_DIO_READ_PIN(confPROGRAMMING_MODE_EN_PORT, confPROGRAMMING_MODE_EN_PIN))



#endif /* BL_H_ */

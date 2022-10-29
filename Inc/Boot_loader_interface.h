/*
 * Boot_loader_interface.h
 *
 *  Created on: Oct 26, 2022
 *      Author: Ali Emad Ali
 *
 * Notes:
 * This software is targeted for STM32f103, it uses UART2 peripheral at a
 * baudrate configurable in ".config" file Bit per second, user should be 
 * aware that they can not use/modify any of the peripheral's
 * configurations.
 */

#ifndef _BOOT_LOADER_INTERFACE_H_
#define _BOOT_LOADER_INTERFACE_H_


/*
 * inits system clock to HSI, not disabling or configuring any of RCC's related
 * clock selection configurations, to avoid conflicting user's - to come - SW.
 *
 * inits UART2 with the baudrate configured in the ".config" file.
 */
void Boot_Loader_voidInit(void);

/*
 * disables sysTick, enables FPEC, erases all pages following the
 * boot-loader section, receives, checks and parses data into flash memory.
 * returns starting execution address.
 */
u32 Boot_Loader_u32EnterProgrammingMode(void);

u32 Boot_Loader_u32GetStoredStartingExecutionAddress(void);

/*
 * gives host flasher time to enter programming key, disables sysTick on exit.
 */
b8 Boot_Loader_b8GiveChanceToUnlock(u16 msTimeout);

#endif /* _BOOT_LOADER_INTERFACE_H_ */

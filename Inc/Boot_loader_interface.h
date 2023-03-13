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
 * inits SysTick.
 *
 * inits esp8266 WiFi module.
 */
void Boot_Loader_voidInit(void);

/*
 * Connects to the configured FTP server.
 */
b8 Boot_Loader_b8ConnectToFtpServer(void);

/*
 * Checks if update is available on server.
 */
b8 Boot_Loader_b8IsOnlineUpdateAvailable(void);

/*
 * Enables FPEC, erases all pages following the boot-loader section, downloads,
 * checks and parses data into flash memory.
 */
void Boot_Loader_voidEnterProgrammingMode(void);

/*
 * Updated version number stored in flash to the one just downloaded and flashed.
 */
void Boot_Loader_voidUpdateVersionNumberOnFlash(void);

u32 Boot_Loader_u32GetStoredStartingExecutionAddress(void);

/*	frees dynamically allocated memory used in bootloader	*/
void Boot_Loader_voidFreeMem(void);


#endif /* _BOOT_LOADER_INTERFACE_H_ */

/*
 * Port_USB.c
 *
 *  Created on: Sep 11, 2023
 *      Author: Ali Emad
 */

#include "USB.h"



/*
 * Rx buffer info:
 */
extern uint8_t UserRxBufferFS[APP_RX_DATA_SIZE];
uint8_t* pucPortUsbRxBuffer = UserRxBufferFS;

extern uint32_t UserRxBufferFSLen;
uint32_t* puiPortUsbRxBufferLen = &UserRxBufferFSLen;

/*
 * IRQ handler:
 */
void USB_LP_CAN1_RX0_IRQHandler(void)
{
	/*	USB driver ISR	*/
	extern PCD_HandleTypeDef hpcd_USB_FS;
	HAL_PCD_IRQHandler(&hpcd_USB_FS);

	/*	Bootloader USB ISR:	*/

}


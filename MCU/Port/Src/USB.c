/*
 * Port_USB.c
 *
 *  Created on: Sep 11, 2023
 *      Author: Ali Emad
 */

#include "USB.h"



/*******************************************************************************
 * Rx buffer info:
 ******************************************************************************/
extern uint8_t UserRxBufferFS[APP_RX_DATA_SIZE];
uint8_t* pucPortUsbRxBuffer = UserRxBufferFS;

extern uint32_t UserRxBufferFSLen;
uint32_t* puiPortUsbRxBufferLen = &UserRxBufferFSLen;

/*******************************************************************************
 * API functions:
 ******************************************************************************/
/*
 * See header for info.
 */
void vPort_USB_initHardware(void)
{
	vLib_ASSERT(USBD_Init(&hUsbDeviceFS, &FS_Desc, DEVICE_FS) == USBD_OK, 0);
	vLib_ASSERT(USBD_RegisterClass(&hUsbDeviceFS, &USBD_CDC) == USBD_OK, 0);
	vLib_ASSERT(USBD_CDC_RegisterInterface(&hUsbDeviceFS, &USBD_Interface_fops_FS) == USBD_OK, 0);
	vLib_ASSERT(USBD_Start(&hUsbDeviceFS) == USBD_OK, 0);
}

/*
 * See header for info.
 */
uint8_t ucPort_USB_send(uint8_t* pucBuffer, uint16_t usLen)
{
	return (CDC_Transmit_FS(pucBuffer, usLen) == USBD_OK);
}

/*
 * See header for info.
 */
uint8_t ucPort_USB_isRxne(void)
{
	return (UserRxBufferFSLen > 0);
}

uint8_t ucPort_USB_receive_line(uint8_t* pucBuffer, uint16_t usMaxLen)
{
	uint16_t i, usCount = 0;

	while(1)
	{
		/*	Wait for data to be received	*/
		while(UserRxBufferFSLen == 0);

		/*	Copy all received data into "pucBuffer"	*/
		for (i = 0; i < UserRxBufferFSLen; i++)
		{
			pucBuffer[usCount++] = UserRxBufferFS[usCount];

		}
	}

	return 0;
}

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


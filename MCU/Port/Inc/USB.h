/*
 * USB.h
 *
 *  Created on: Sep 11, 2023
 *      Author: Ali Emad
 *
 * This driver - so far - implements only the virtual COM port functionality.
 */

#ifndef PORT_USB_H_
#define PORT_USB_H_

#include "stm32f1xx.h"
#include "stm32f1xx_hal.h"
#include "usbd_def.h"
#include "usbd_core.h"
#include "usbd_cdc.h"
#include "usb_device.h"
#include "usbd_desc.h"
#include "usbd_cdc_if.h"

#include "Assert.h"


/*******************************************************************************
 * Externs from USB library:
 ******************************************************************************/
extern uint8_t UserRxBufferFS[];
extern uint32_t UserRxBufferFSLen;
extern uint32_t UserRxBufferFSAlreadyReadLen;
extern uint8_t UserRxBufferFSOREFlag;

/*******************************************************************************
 * Rx buffer parameters:
 ******************************************************************************/
uint8_t* pcRxBuffer = ;
uint32_t UserRxBufferFSLen = 0;
uint32_t UserRxBufferFSAlreadyReadLen = 0;
uint8_t UserRxBufferFSOREFlag = 0;











extern USBD_HandleTypeDef hUsbDeviceFS;

/*
 * Rx buffer info:
 */
extern uint8_t* pucPortUsbRxBuffer;
extern uint32_t* puiPortUsbRxBufferLen;


/*
 * Initializes USB hardware.
 */
void vPort_USB_initHardware(void);

/*
 * Start a transmission operation.
 * Returns 1 if operation started successfully. Otherwise it returns 0. (For example
 * if previously initiated transmission has not yet ended.
 */
uint8_t ucPort_USB_send(uint8_t* pucBuffer, uint16_t usLen);

/*
 * Checks if the Rx buffer has new data.
 */
uint8_t ucPort_USB_isRxne(void);

uint8_t ucPort_USB_receive_line(uint8_t* pucBuffer, uint16_t usMaxLen);


#endif /* PORT_USB_H_ */

/*
 * Host_Communication.c
 *
 *  Created on: Nov 6, 2023
 *      Author: Ali Emad
 */

/*	PORT	*/
#include "Inline.h"
#include "USB.h"

#include "Host_Communication.h"

/*******************************************************************************
 * API functions:
 ******************************************************************************/
/*
 * See header for info.
 */
void vPort_HostCom_SendToHost(uint8_t* pucByteArr, uint16_t uiLen)
{

}

/*
 * See header for info.
 */
uint8_t ucReceiveFromHost(uint8_t* pucByteArr, uint16_t* puiLen)
{

}

/*
 * See header for info.
 */
PORT_INLINE_ALWAYS
void vPort_HostCom_init()
{
	vPort_USB_initHardware();
}

/*
 * Host_Communication.h
 *
 *  Created on: Nov 6, 2023
 *      Author: Ali Emad
 */

#ifndef INC_HOST_COMMUNICATION_H_
#define INC_HOST_COMMUNICATION_H_


/*
 * Sends byte array to host machine. In this wrapper, the used communication method
 * is called to achieve message sending. This could include encryption, error checking
 * calculation, acknowledge mechanism,.. etc.
 */
void vPort_HostCom_sendToHost(uint8_t* pucByteArr, uint16_t uiLen);

/*
 * Receives byte array from host machine within a given timeout. In this wrapper,
 * the used communication method is called to achieve message receiving. This
 * could include decryption, error checking calculation, acknowledge mechanism,.. etc.
 *
 * Returns 1 if successful, 0 otherwise.
 */
uint8_t ucPort_HostCom_receiveFromHost(uint8_t* pucByteArr, uint16_t* puiLen);

/*
 * Initializes host communication.
 */
void vPort_HostCom_init();


#endif /* INC_HOST_COMMUNICATION_H_ */

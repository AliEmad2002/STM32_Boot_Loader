/*
 * Clock.h
 *
 *  Created on: Jun 14, 2023
 *      Author: Ali Emad
 */

#ifndef PORT_CLOCK_H_
#define PORT_CLOCK_H_

#include "stm32f103xb.h"
#include "stm32f1xx_hal.h"

#define uiPORT_CLOCK_MAIN_HZ		72000000


void vPort_Clock_initCpuClock(void);

void vPort_Clock_initPeriphClock(void);




#endif /* PORT_CLOCK_H_ */

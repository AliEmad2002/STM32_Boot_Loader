/*
 * BL_config.h
 *
 *  Created on: Oct 22, 2023
 *      Author: Ali Emad Ali
 */

#ifndef BL_CONFIG_H_
#define BL_CONFIG_H_

/*
 * Configure the pin which makes the MCU enter the programming mode on startup.
 *
 * Notes:
 * 		-	This pin is configured as a pull down input (in bootloader initialization).
 * 		-	If the used target does not support pull-down on the selected pin, it
 * 			should be pulled down externally.
 */
#define confPROGRAMMING_MODE_EN_PORT	1
#define confPROGRAMMING_MODE_EN_PIN		2




#define KEY_STRING		"ivC4WfVHnMU!0g3a"

#define MAX_STR_LEN			60


#endif /* BL_CONFIG_H_ */

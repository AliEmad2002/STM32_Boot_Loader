/*
 * Inline.h
 *
 *  Created on: Oct 22, 2023
 *      Author: Ali Emad
 */

#ifndef INC_INLINE_H_
#define INC_INLINE_H_


/*
 * Define inline forcing attribute here. As it is compiler dependent, and hence ported.
 */
#define PORT_INLINE_ALWAYS	inline __attribute__((always_inline))


#endif /* INC_INLINE_H_ */

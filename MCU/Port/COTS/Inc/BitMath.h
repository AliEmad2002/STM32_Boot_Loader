/*
 * BitMath.h
 *
 *  Created on: Oct 24, 2023
 *      Author: Ali Emad
 */

#ifndef COTS_INC_BITMATH_H_
#define COTS_INC_BITMATH_H_

#define _SET_BIT(reg, n)					((reg) |= 1<<(n))

#define CLR_BIT(reg, n)					((reg) &= ~(1<<(n)))

#define TGL_BIT(reg, n)					((reg) ^= 1<<(n))

#define GET_BIT(reg, n)					(((reg)>>(n)) & 1)

#define WRT_BIT(reg, n, val)			((val) ? _SET_BIT((reg), (n)) : CLR_BIT((reg), (n)))

/*	2^n	*/
#define POW_TWO(n)						(1 << (n))

/*	maximum of n-bits (2^n - 1) */
#define MAX_N_BITS(n)					(POW_TWO((n)) - 1)

#define GET_REG_SEC(reg, start, len)	(((reg) >> (start)) & (POW_TWO((len)) - 1))

/*
 * Edit a block of bytes in a register to a new value
 * reg:		the register to be edited.
 * start:	number of the bit to start editing upwards from.
 * val:		value that is going to replace whatever is in "reg" starting from "start" bit.
 * len:		length of val.
 */
static inline void BitMath_voidEditRegister(uint32_t* regPtr, uint8_t start, uint32_t val, uint8_t len)
{
	*regPtr = (*regPtr & ~((POW_TWO(len)-1) << start)) | (val << start);
}

#define EDT_REG(reg, start, val, len)	BitMath_voidEditRegister((uint32_t*)&(reg), start, val, len)



#endif /* COTS_INC_BITMATH_H_ */
